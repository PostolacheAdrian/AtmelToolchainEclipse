# Makefile created by Adrian Postolache - 2019
PROJECT_NAME := out
# Microcontroller specific
MCU := atmega32a
# Input Frequency 
F_CPU := 16000000
#Flashing board port
COMPORT:= COM18

#Output folder name
output_folder := out

#includes of project
INCLUDES:=$(addprefix -I,$(subst \,/, $(shell for /r %%a in (.) do @if exist "%%~fa\*.h" echo %%~fa)))

#sources of project
SOURCES := $(subst \,/,$(shell dir /B /S *.c))

#objects of project
OBJECTS := $(patsubst %.o,$(output_folder)/objects/%.o,$(notdir $(subst .c,.o, $(SOURCES))))

#Compiler 
CC := gcc.exe
#Linker
LD := ld.exe
#Assembler
AS := as.exe
#Object Dump
DMP :=objdump.exe
#Object copy
CPY := objcopy.exe
#Object size
SIZ := size.exe

#Compiller flags
CFLAGS := -Wall -O1 -mmcu=$(MCU) -DF_CPU=$(F_CPU) -D__AVR_ATmega32A__ -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections $(INCLUDES)
LDFLAGS := -mmcu=atmega32a -Wl,-Map,$(output_folder)/$(PROJECT_NAME).map  -gc-sections 
vpath %.c $(dir $(SOURCES))


# Compile all .c files 
$(output_folder)/objects/%.o: %.c 
	@echo ----------------------------------------------------
	@echo Compiling: $(notdir $<)
	@echo ####################################################
	$(CC) $(CFLAGS) -MD -MF $(output_folder)/dependencies/$(notdir $(subst .c,.d,$<)) -c $< -o $@
	
#Linking objects and generate ELF file
$(output_folder)/$(PROJECT_NAME).elf: $(OBJECTS)
	@echo ----------------------------------------------------
	@echo Linking objects: $(notdir $^)
	@echo ----------------------------------------------------
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo $@ file has been generated !
#Flashing command 
flash:
	@../../tools/and-load.exe -c $(COMPORT) -h $(output_folder)/$(PROJECT_NAME).hex

#Create output folder
create_output:
	@echo ----------------------------------------------------
	@echo Build started.
	@echo ----------------------------------------------------
ifneq ($(wildcard $(output_folder)),$(output_folder))
	@mkdir $(output_folder)
	@mkdir $(output_folder)\objects
	@mkdir $(output_folder)\dependencies
endif


#Trigger of entire build procces
.PHONY: all	$(output_folder)/$(PROJECT_NAME).elf
all: create_output generate
	@echo Build finished !

generate: $(output_folder)/$(PROJECT_NAME).elf 
#Display object sections size	
	@echo ----------------------------------------------------
	@$(SIZ) $<
	@echo ----------------------------------------------------
#Generate dump file
	@$(DMP) -h -S $< > $(output_folder)/$(PROJECT_NAME).lst
	@echo $(PROJECT_NAME).lst file has been generated !
	@echo ----------------------------------------------------
#Generate hex file
	@$(CPY) -O ihex $< $(output_folder)/$(PROJECT_NAME).hex
	@echo $(PROJECT_NAME).hex file has been generated !
	@echo ----------------------------------------------------
#Clean output folder
clean:
	@rm -rf $(output_folder)
	
-include $(wildcard $(output_folder)/dependencies/*.d)