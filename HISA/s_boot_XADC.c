#include "uart.h"
#include "printf.h"
#include "timer.h"
#include "common.h"
#include "xsysmon.h"

#define XPAR_XADC_WIZ_0_DEVICE_ID 0
#define XPAR_SYSMON_0_DEVICE_ID XPAR_XADC_WIZ_0_DEVICE_ID
#define SYSMON_DEVICE_ID 	XPAR_SYSMON_0_DEVICE_ID
#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
#define XST_DEVICE_NOT_FOUND            2L
#define XST_DEVICE_BLOCK_NOT_FOUND      3L
#define XST_INVALID_VERSION             4L
#define XST_DEVICE_IS_STARTED           5L
#define XST_DEVICE_IS_STOPPED           6L
#define XST_FIFO_ERROR                  7L
#define NULL		0U

static int SysMonPolledPrintfExample(u16 SysMonDeviceId);

static XSysMon SysMonInst;

extern void monitorInit();

volatile unsigned long * baseaddr_input = (unsigned long*)0x00024000;
volatile unsigned long * output = (unsigned long*)0x0002a000;

typedef struct Users {
	unsigned long id;
	int access;
};

struct Users a[10] = {{0xa3e980c4, 10}, {0xcd23e300, 5}, {0x0034a197, 23}, {0x31aa9872, 54}, {0x19840335, 2}, {0x214a9845, 7}, {0xcdef1239, 113}, {0xb8c0349a, 104}, {0x9e802246, 1}, {0x239f9234, 0}};

int whitelist_size = 10;

int whitelist(unsigned long id)
{
	int i;
	for (i = 0; i < whitelist_size; i++)
	{
		if (id == a[i].id)
		{
			return i;
		}
	}
	return -1;
}

int frequency(struct Users input, int index)
{
	int i;
	if (input.access <= 10000000000)
	{
		return 1;
	}
	else
	{
		for (i = index; i < whitelist_size - 1; i++)
		{
			a[i].id = a[i+1].id;
			a[i].access = a[i+1].access;
		}
		whitelist_size--;
		return 0;
	}
}


int secure_main(void)
{	
	int t[10] = {0};
	//int count = 0;
	int i, size, index;
	int Status;

	//unsigned long last_user = 0x00000000; 

	uart_init();
	timer_init();
	
	// set for non-secure can access some coprocessor reg
	CP15_SET_NSACR(0x00073fff);
	CP15_SET_CPACR(0x0fffffff);

	// set for SCR
	CP15_SET_SCR(0b110000);

	writel(0xdf0d, 0xf8000008);	//unlock
//	writel(0x0, 0xe0200018);	//config uart to secure
	writel(0xffff, 0xf8000404);	//config ocmram2 to non-secure
//	writel(0x0, 	   0xf8000400); //config ocmram1 to secure
//	writel(0x767b,	0xf8000910);	//lock
	
	asm volatile ("isb");
	asm volatile ("dsb");
	
	// Install monitor
//  char* dest = (char*)0x20100;
//	char* src  = (char*)0x1000;
// 	for(i=0; i<500; i++)
//	 	*dest++ = *src++;

	writel((0x1 << 7), 0xe0200018);	//config uart to secure
	monitorInit();

#if 1
	index = whitelist(*baseaddr_input);
	if (index == -1) return 0;
	size = *(baseaddr_input + 1);
	for (i = 0; i < size; i++) {
		Status = SysMonPolledPrintfExample(SYSMON_DEVICE_ID);	// Call the XADC IP
		if (Status != XST_SUCCESS)
		{
				return XST_FAILURE;
		}
	}
	a[index].access += size;
	frequency(a[index], index);
	asm volatile ("smc #0\n\t");		// Switch to the normal world
#endif
	return XST_SUCCESS;
}

int SysMonPolledPrintfExample(u16 SysMonDeviceId)
{
	int Status;
	XSysMon_Config *ConfigPtr;
	u32 TempRawData;
	u32 VccAuxRawData;
	u32 VccIntRawData;
	float TempData;
	float VccAuxData;
	float VccIntData;
	float MaxData;
	float MinData;
	XSysMon *SysMonInstPtr = &SysMonInst;

	//printf("\r\nEntering the SysMon Polled Example. \r\n");

	/*
	 * Initialize the SysMon driver.
	 */
	ConfigPtr = XSysMon_LookupConfig(SysMonDeviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XSysMon_CfgInitialize(SysMonInstPtr, ConfigPtr,
				ConfigPtr->BaseAddress);

	/*
	 * Self Test the System Monitor/ADC device
	 */
	Status = XSysMon_SelfTest(SysMonInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Disable the Channel Sequencer before configuring the Sequence
	 * registers.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_SAFE);


	/*
	 * Disable all the alarms in the Configuration Register 1.
	 */
	XSysMon_SetAlarmEnables(SysMonInstPtr, 0x0);


	/*
	 * Setup the Averaging to be done for the channels in the
	 * Configuration 0 register as 16 samples:
	 */
	XSysMon_SetAvg(SysMonInstPtr, XSM_AVG_16_SAMPLES);

	/*
	 * Setup the Sequence register for 1st Auxiliary channel
	 * Setting is:
	 *	- Add acquisition time by 6 ADCCLK cycles.
	 *	- Bipolar Mode
	 *
	 * Setup the Sequence register for 16th Auxiliary channel
	 * Setting is:
	 *	- Add acquisition time by 6 ADCCLK cycles.
	 *	- Unipolar Mode
	 */
	Status = XSysMon_SetSeqInputMode(SysMonInstPtr, XSM_SEQ_CH_AUX00);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XSysMon_SetSeqAcqTime(SysMonInstPtr, XSM_SEQ_CH_AUX15 |
						XSM_SEQ_CH_AUX00);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Enable the averaging on the following channels in the Sequencer
	 * registers:
	 * 	- On-chip Temperature, VCCINT/VCCAUX  supply sensors
	 * 	- 1st/16th Auxiliary Channels
	  *	- Calibration Channel
	 */
	Status =  XSysMon_SetSeqAvgEnables(SysMonInstPtr, XSM_SEQ_CH_TEMP |
						XSM_SEQ_CH_VCCINT |
						XSM_SEQ_CH_VCCAUX |
						XSM_SEQ_CH_AUX00 |
						XSM_SEQ_CH_AUX15 |
						XSM_SEQ_CH_CALIB);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the following channels in the Sequencer registers:
	 * 	- On-chip Temperature, VCCINT/VCCAUX supply sensors
	 * 	- 1st/16th Auxiliary Channel
	 *	- Calibration Channel
	 */
	Status =  XSysMon_SetSeqChEnables(SysMonInstPtr, XSM_SEQ_CH_TEMP |
						XSM_SEQ_CH_VCCINT |
						XSM_SEQ_CH_VCCAUX |
						XSM_SEQ_CH_AUX00 |
						XSM_SEQ_CH_AUX15 |
						XSM_SEQ_CH_CALIB);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Set the ADCCLK frequency equal to 1/32 of System clock for the System
	 * Monitor/ADC in the Configuration Register 2.
	 */
	XSysMon_SetAdcClkDivisor(SysMonInstPtr, 32);


	/*
	 * Set the Calibration enables.
	 */
	XSysMon_SetCalibEnables(SysMonInstPtr,
				XSM_CFR1_CAL_PS_GAIN_OFFSET_MASK |
				XSM_CFR1_CAL_ADC_GAIN_OFFSET_MASK);

	/*
	 * Enable the Channel Sequencer in continuous sequencer cycling mode.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_CONTINPASS);

	/*
	 * Wait till the End of Sequence occurs
	 */
	XSysMon_GetStatus(SysMonInstPtr); /* Clear the old status */
	while ((XSysMon_GetStatus(SysMonInstPtr) & XSM_SR_EOS_MASK) !=
			XSM_SR_EOS_MASK);

	/*
	 * Read the on-chip Temperature Data (Current/Maximum/Minimum)
	 * from the ADC data registers.
	 */
	TempRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_TEMP);
	TempData = XSysMon_RawToTemperature(TempRawData);
	//printf("\r\nThe Current Temperature is %0d.%03d Centigrades.\r\n",
	//			(int)(TempData), SysMonFractionToInt(TempData));
	*(output + 0) = (unsigned long)(TempData);				// Write the current temperature to the shared memory

	TempRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MAX_TEMP);
	MaxData = XSysMon_RawToTemperature(TempRawData);
	//printf("The Maximum Temperature is %0d.%03d Centigrades. \r\n",
	//			(int)(MaxData), SysMonFractionToInt(MaxData));
	*(output + 1) = (unsigned long)(MaxData);				// Write the maximum of temperature to the shared memory
	TempRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MIN_TEMP);
	MinData = XSysMon_RawToTemperature(TempRawData);
	//printf("The Minimum Temperature is %0d.%03d Centigrades. \r\n",
	//			(int)(MinData), SysMonFractionToInt(MinData));
	*(output + 2) = (unsigned long)(MinData);				// Write the minimum of temperature to the shared memory
	/*
	 * Read the VccInt Votage Data (Current/Maximum/Minimum) from the
	 * ADC data registers.
	 */
	VccIntRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCINT);
	VccIntData = XSysMon_RawToVoltage(VccIntRawData);
	//printf("\r\nThe Current VCCINT is %0d.%03d Volts. \r\n",
	//		(int)(VccIntData), SysMonFractionToInt(VccIntData));
	*(output + 3) = (unsigned long)(VccIntData);				// Write the current VCCINT to the shared memory
	VccIntRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MAX_VCCINT);
	MaxData = XSysMon_RawToVoltage(VccIntRawData);
	//printf("The Maximum VCCINT is %0d.%03d Volts. \r\n",
	//		(int)(MaxData), SysMonFractionToInt(MaxData));
	*(output + 4) = (unsigned long)(MaxData);				// Write the maximum of VCCINT to the shared memory
	VccIntRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MIN_VCCINT);
	MinData = XSysMon_RawToVoltage(VccIntRawData);
	//printf("The Minimum VCCINT is %0d.%03d Volts. \r\n",
	//		(int)(MinData), SysMonFractionToInt(MinData));
	*(output + 5) = (unsigned long)(MinData);				// Write the minimum VCCINT to the shared memory
	/*
	 * Read the VccAux Votage Data (Current/Maximum/Minimum) from the
	 * ADC data registers.
	 */
	VccAuxRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCAUX);
	VccAuxData = XSysMon_RawToVoltage(VccAuxRawData);
	//printf("\r\nThe Current VCCAUX is %0d.%03d Volts. \r\n",
	//		(int)(VccAuxData), SysMonFractionToInt(VccAuxData));
	*(output + 6) = (unsigned long)(VccAuxData);				// Write the current VCCAUX to the shared memory
	VccAuxRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MAX_VCCAUX);
	MaxData = XSysMon_RawToVoltage(VccAuxRawData);
	//printf("The Maximum VCCAUX is %0d.%03d Volts. \r\n",
	//			(int)(MaxData), SysMonFractionToInt(MaxData));
	*(output + 7) = (unsigned long)(MaxData);				// Write the maximum VCCAUX to the shared memory
	VccAuxRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr,
							XSM_MIN_VCCAUX);
	MinData = XSysMon_RawToVoltage(VccAuxRawData);
	//printf("The Minimum VCCAUX is %0d.%03d Volts. \r\n\r\n",
	//			(int)(MinData), SysMonFractionToInt(MinData));
	*(output + 8) = (unsigned long)(MinData);				// Write the minimum VCCAUX to the shared memory
	//printf("Exiting the SysMon Polled Example. \r\n");

	return XST_SUCCESS;
}
