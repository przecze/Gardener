ALL=main main_err
F_CPU=16000000

CC=avr-gcc
CFLAGS= -Wall -O3 -DF_CPU=$(F_CPU) -gstabs -std=c++14
CXX="C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avr-g++"
INCFLAGS=-I.
LDFLAGS=-mmcu=atmega328p
OBJCOPY="C:\Program Files (x86)\Arduino\hardware\tools\avr/bin/avr-objcopy"
AVRDUDE=avrdude -c arduino -p m328p -P COM8 -b 57600
CXXFLAGS=$(CFLAGS) $(INCFLAGS)
HEADERS=Sound.h defines.h hardware.h CantusFirmusGenerator.h

test: main.cc Sound.h hardware_mock.h defines.h
	g++ -o a.exe -std=c++14 -I. -D TEST main.cc
	./a.exe

test_with_error_modes: main.cc $(HEADERS) hardware_mock.h 
	g++ -o a.exe -std=c++14 -I. -D TEST -D ERROR_MODE main.cc
	./a.exe

main: main.cc $(HEADERS) hardware_atmega328p.h
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS) $< -o $@

main_err: main.cc $(HEADERS) hardware_atmega328p.h
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS) -D ERROR_MODE $< -o $@

%.hex: %
	$(OBJCOPY) -O ihex $< $@

upload: main.hex
	$(AVRDUDE) -U flash:w:$<

upload_with_error_modes: main_err.hex
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
