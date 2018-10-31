This is the source code for HISA based on our ICCAD 2018 publication: "HISA: Hardware Isolation-based Secure Architecture for CPU-FPGA Embedded Systems". Please direct your questions and comments to hwselunl@gmail.com.

# HISA Execution Instructions

## Environment Setup
The provided source code has been tested in Vivado2015.4, 64-bit Ubuntu 14.04.
 - Install Vivado
 - Connect UART and PROG ports of ZC702 board with IMAGEON daughter card (the daughter card is for the motion detection demo) to the PC and turn the board on
 - To check the uart output in the board, using gtkterm is recommended
   - After executing gtkterm, go to Configuration in the menu of gtkterm
   - Select the correct serial port for the ZedBoard/ZC702 UART (Most likely the port is /dev/ttyUSB# for ZC702)
   - Set Baud Rate to 115200
 - Download [Sourcery CodeBench](https://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/) and store CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux into /opt/

## Reference IP Execution
 - Execute reference IPs by running.sh
 - To execute different IPs:
   - Replace the content in trustzone_isolation/secure/s_boot.c to the content of s_boot_$IP.c. (E.g., to run multiplier, replace the content of s_boot.c to the content of s_boot_Multiplier.c.)
   - Copy and paste the design_1_wrapper_hw_platform_0 folder from the referenceIPs/$IP/$IP.sdk to the trustzone_isolation directory. (E.g., to run multiplier, copy and paste ../referenceIPs/multiplier/multiplier.sdk/design_1_wrapper_hw_platform_0 to the trustzone_isolation directory.)
 - To execute motion detection demo in the caseStudy folder 
   - Execute the motion detection demo without TrustZone protection by running.sh
   - Execute the motion detection demo with TrustZone protection by running_TZ.sh
   - Execute the motion detection demo with hardware Trojan by running_Trojan.sh