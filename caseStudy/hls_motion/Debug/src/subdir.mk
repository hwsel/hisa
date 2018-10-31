################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/fmc_iic_axi.c \
../src/fmc_imageon.c \
../src/fmc_imageon_hdmi_framebuffer.c \
../src/helloworld.c \
../src/platform.c \
../src/video_detector.c \
../src/video_frame_buffer.c \
../src/video_generator.c \
../src/video_resolution.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/fmc_iic_axi.o \
./src/fmc_imageon.o \
./src/fmc_imageon_hdmi_framebuffer.o \
./src/helloworld.o \
./src/platform.o \
./src/video_detector.o \
./src/video_frame_buffer.o \
./src/video_generator.o \
./src/video_resolution.o 

C_DEPS += \
./src/fmc_iic_axi.d \
./src/fmc_imageon.d \
./src/fmc_imageon_hdmi_framebuffer.d \
./src/helloworld.d \
./src/platform.d \
./src/video_detector.d \
./src/video_frame_buffer.d \
./src/video_generator.d \
./src/video_resolution.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hls_motion_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


