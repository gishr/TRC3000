################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"E:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmspx --data_model=restricted -Ooff --use_hw_mpy=F5 --include_path="E:/ti/ccs910/ccs/ccs_base/msp430/include" --include_path="D:/Monash/Year 3/TRC3000/workspace/IMU BackChannel V3" --include_path="D:/Monash/Year 3/TRC3000/workspace/IMU BackChannel V3/driverlib/MSP430F5xx_6xx" --include_path="D:/Monash/Year 3/TRC3000/workspace/IMU BackChannel V3/" --include_path="D:/Monash/Year 3/TRC3000/workspace/IMU BackChannel V3/USB_config" --include_path="E:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


