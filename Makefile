ALL=hello
F_CPU=16000000

CC=avr-gcc
CFLAGS= -Wall -O2 -DF_CPU=$(F_CPU) -gstabs -std=c++14
CXX="C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avr-g++"
INCFLAGS=-I.
LDFLAGS=-mmcu=atmega328p
OBJCOPY=avr-objcopy
AVRDUDE=avrdude -c arduino -p m328p -P COM8 -b 57600
CXXFLAGS=$(CFLAGS) $(INCFLAGS)

%.s: %.cc
	$(CXX) $(CXXFLAGS) -S $<
	
%.hex: %
	$(OBJCOPY) -O ihex $< $@

%.upload: %.hex
	$(AVRDUDE) -U flash:w:$<

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -f *.hex *.o *.s $(ALL)

.PHONY: 1MHz
1MHz:
	/bin/echo -e "write hfuse 0 0xd9\nwrite lfuse 0 0xe1" | $(AVRDUDE) -B 3 -t

.PHONY: 16MHz
16MHz:
	/bin/echo -e "write hfuse 0 0xd9\nwrite lfuse 0 0xff" | $(AVRDUDE) -B 3 -t
