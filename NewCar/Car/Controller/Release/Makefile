################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../ctrl_devices \
../FTDI/ \
../FTDI/hardware/ \
../FTDI/hardware/FT800/ \
../FTDI/hardware/MCP7940M/ \
../FTDI/libraries/ \
../FTDI/libraries/FT_GC/ \
../FTDI/libraries/FT_GC/FT800/ \
../FTDI/libraries/FT_GC/FT_Transport_SPI/ \
../FTDI/libraries/FT_RTC/ \
../FTDI/libraries/FT_SD/ \
../__vm/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../ctrl_devices/Button.cpp \
../ctrl_devices/ControllerDisplay.cpp \
../ctrl_devices/Joystick.cpp \
../EEPROMExt.cpp \
../mrf24j.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
ctrl_devices/Button.o \
ctrl_devices/ControllerDisplay.o \
ctrl_devices/Joystick.o \
EEPROMExt.o \
mrf24j.o

OBJS_AS_ARGS +=  \
ctrl_devices/Button.o \
ctrl_devices/ControllerDisplay.o \
ctrl_devices/Joystick.o \
EEPROMExt.o \
mrf24j.o

C_DEPS +=  \
ctrl_devices/Button.d \
ctrl_devices/ControllerDisplay.d \
ctrl_devices/Joystick.d \
EEPROMExt.d \
mrf24j.d

C_DEPS_AS_ARGS +=  \
ctrl_devices/Button.d \
ctrl_devices/ControllerDisplay.d \
ctrl_devices/Joystick.d \
EEPROMExt.d \
mrf24j.d

OUTPUT_FILE_PATH +=Controller.elf

OUTPUT_FILE_PATH_AS_ARGS +=Controller.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler











ctrl_devices/%.o: ../ctrl_devices/%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -D__AVR_ATmega2560__ -D_VMDEBUG=1 -DARDUINO=105 -DARDUINO_MAIN -DF_CPU=16000000L -D__AVR__  -I"../../../Controller" -I"C:/Program Files (x86)/Arduino/libraries/EEPROM" -I"C:/Program Files (x86)/Arduino/libraries/SPI" -I"C:/Program Files (x86)/Arduino/libraries" -I"C:/Program Files (x86)/Arduino/hardware/arduino/libraries" -I"C:/Users/Matej/Documents/Arduino/libraries" -I"C:/Program Files (x86)/Arduino/hardware/arduino/cores/arduino" -I"C:/Program Files (x86)/Arduino/hardware/arduino/cores/arduino/avr-libc" -I"C:/Program Files (x86)/Arduino/hardware/arduino/variants/mega" -I"C:/Program Files (x86)/Arduino/hardware" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.132\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega2560 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.132\gcc\dev\atmega2560" -c -include=arduino.h -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -D__AVR_ATmega2560__ -D_VMDEBUG=1 -DARDUINO=105 -DARDUINO_MAIN -DF_CPU=16000000L -D__AVR__  -I"../../../Controller" -I"C:/Program Files (x86)/Arduino/libraries/EEPROM" -I"C:/Program Files (x86)/Arduino/libraries/SPI" -I"C:/Program Files (x86)/Arduino/libraries" -I"C:/Program Files (x86)/Arduino/hardware/arduino/libraries" -I"C:/Users/Matej/Documents/Arduino/libraries" -I"C:/Program Files (x86)/Arduino/hardware/arduino/cores/arduino" -I"C:/Program Files (x86)/Arduino/hardware/arduino/cores/arduino/avr-libc" -I"C:/Program Files (x86)/Arduino/hardware/arduino/variants/mega" -I"C:/Program Files (x86)/Arduino/hardware" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.132\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega2560 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.132\gcc\dev\atmega2560" -c -include=arduino.h -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Controller.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega2560 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.2.132\gcc\dev\atmega2560"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Controller.elf" "Controller.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Controller.elf" "Controller.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Controller.elf" > "Controller.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Controller.elf" "Controller.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "Controller.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Controller.elf" "Controller.a" "Controller.hex" "Controller.lss" "Controller.eep" "Controller.map" "Controller.srec" "Controller.usersignatures"
	