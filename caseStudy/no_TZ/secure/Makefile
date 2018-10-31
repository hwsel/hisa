.phony: all

all: secure.bin
ARMGNU ?= arm-xilinx-linux-gnueabi

CC  = $(ARMGNU)-gcc
LD  = $(ARMGNU)-ld
COPY= $(ARMGNU)-objcopy
DUMP= $(ARMGNU)-objdump
PLATFORM_LIBGCC = /opt/CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/lib/gcc/arm-xilinx-linux-gnueabi/4.6.1

secure.bin: secure.elf
	$(COPY) -O binary $< secure-origin.bin
	$(DUMP) -D $< > dump_secure.log
	$(COPY) -I binary -O binary --pad-to 0x4000 --gap-fill=0x00 secure-origin.bin $@
	
secure.elf: secure_start.S s_boot.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -O0 -nostdlib secure_start.S
#	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib secure_startup.S
	$(CC) $(USR_DEFINE) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -O0 -nostdlib s_boot.c printf.c timer.c uart.c fmc_iic_axi.c fmc_imageon_hdmi_framebuffer.c fmc_imageon.c video_detector.c video_frame_buffer.c video_generator.c video_resolution.c xvtc.c xil_assert.c xil_io.c sleep.c usleep.c xtime_l.c xaxivdma.c xaxivdma_channel.c xvtc_sinit.c xvtc_g.c xaxivdma_sinit.c xaxivdma_g.c xscutimer.c xscutimer_selftest.c xscutimer_sinit.c xaxicdma_bdring.c xaxicdma.c xhls_macc.c  xaxicdma_intr.c xaxicdma_bd.c xscutimer_g.c xaxicdma_sinit.c xscugic_intr.c xaxicdma_g.c xil_cache.c xscugic.c xil_exception.c xscugic_sinit.c xscugic_hw.c xscugic_g.c xiic_l.c xhls_macc.c xhls_macc_sinit.c
	#$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -O0 -nostdlib monitor.S
	$(LD)  -nostartfiles -static -nostdlib -T x-load.lds secure_start.o s_boot.o printf.o timer.o uart.o fmc_iic_axi.o fmc_imageon_hdmi_framebuffer.o fmc_imageon.o video_detector.o video_frame_buffer.o video_generator.o video_resolution.o xvtc.o xil_assert.o xil_io.o sleep.o usleep.o xaxivdma.o xtime_l.o xaxivdma_channel.o xvtc_sinit.o xvtc_g.o xaxivdma_sinit.o xaxivdma_g.o xscutimer.o xscutimer_selftest.o xscutimer_sinit.o xaxicdma_bdring.o xaxicdma.o xaxicdma_intr.o xaxicdma_bd.o xscutimer_g.o xaxicdma_sinit.o xscugic_intr.o xaxicdma_g.o xil_cache.o xscugic.o xil_exception.o xscugic_sinit.o xhls_macc.o xhls_macc_sinit.o xscugic_hw.o xscugic_g.o xiic_l.o -o secure.elf  -lgcc -L${PLATFORM_LIBGCC}
#	$(LD)  -nostartfiles -static -nostdlib -T x-load.lds secure_start.o s_boot.o monitor.o printf.o timer.o -o secure.elf  -lgcc -L${PLATFORM_LIBGCC}
#	$(LD) -T x-load.lds secure_start.o s_boot.o monitor.o printf.o timer.o -o secure.elf # -lgcc -L${PLATFORM_LIBGCC}

clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
