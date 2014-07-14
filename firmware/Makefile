ARDUINO_DIR=~/nonapt/arduino-1.5.6-r2
ARDUINO=$(ARDUINO_DIR)/arduino
AVRDUDE=/usr/bin/avrdude
AVR_GCC=/usr/bin/avr-gcc
AVR_OBJCOPY=/usr/bin/avr-objcopy
PROGRAMMER=buspirate
PROGRAMMER_PORT=/dev/buspirate
ARDUINO_PORT=/dev/ttyACM0

all: check_submodule compile_app


check_submodule:
	@echo ""
compile_app:
ifeq ("$(wildcard ./bin/application.hex )", "")
	@rm -f ./build/app.md5
endif
	@md5sum application/application.ino | cmp -s build/app.md5 -; \
	 if test $$? -ne 0; then \
	  xvfb-run -a $(ARDUINO) \
	   --verbose-build \
	   --board arduino:RFduino:RFduino \
	   --pref board=RFduino \
	   --pref target_platform=RFduino \
	   --pref build.path=`pwd`/build \
	   --pref sketchbook.path=`pwd` \
	   --verify `pwd`/application/application.ino; \
	  md5sum application/application.ino > build/app.md5; \
	  $(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy -O ihex -R .eeprom \
	              build/application.cpp.elf bin/application.hex; \
	  touch build/.git_empty; \
	fi

compile_test:
ifeq ("$(wildcard ./bin/test.hex )", "")
	@rm -f ./build/test.md5
endif
	@md5sum test/test.ino | cmp -s build/test.md5 -; \
	 if test $$? -ne 0; then \
	  xvfb-run -a $(ARDUINO) \
	   --verbose-build \
	   --board arduino:avr:leonardo \
	   --pref board=leonardo \
	   --pref target_platform=avr \
	   --pref build.path=`pwd`/build \
	   --pref sketchbook.path=`pwd` \
	   --verify `pwd`/test/test.ino; \
	  md5sum test/test.ino > build/test.md5; \
	  $(ARDUINO_DIR)/hardware/tools/avr/bin/avr-objcopy -O srec -R .eeprom \
	              build/test.cpp.elf bin/test.hex; \
	  touch build/.git_empty; \
	fi

app: compile_app
	wine hardware/arduino/RFduino/RFDLoader.exe com1 bin/application.hex

test: compile_test
	$(AVRDUDE) -patmega32u4 -cavr109 -P$(ARDUINO_PORT) -b57600 -D -Uflash:w:./bin/test.hex:s 

clean:
	rm -rf build/*
	rm -rf bin/*
