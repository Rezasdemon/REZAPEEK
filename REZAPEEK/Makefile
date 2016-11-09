TARGET  = REZAPEEK
OBJS    = memory.o blit.o font.o threads.o math.o  ui.o main.o

LIBS    = -ltaihen_stub -lSceAppUtil_stub -lSceAppMgr_stub -lSceCtrl_stub -lSceDisplay_stub -lSceFios2_stub -lScePower_stub -lSceLibc_stub
	
PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -g -Wl,-q -Wall -O3 -nostartfiles

ASFLAGS = $(CFLAGS)

all: $(TARGET).suprx

%.skprx: %.velf
	vita-make-fself $< $@

%.suprx: %.velf
	vita-make-fself $< $@

%.velf: %.elf
	vita-elf-create -e ./REZAPEEK.yml $< $@ $(VITASDK)/share/taihen.json

$(TARGET).elf:  $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).suprx $(TARGET).velf $(TARGET).elf $(OBJS)

send: $(TARGET).suprx
	curl -T $(TARGET).suprx ftp://192.168.43.204:1337/ux0:/plugins/$(TARGET).suprx
	curl -T $(TARGET).suprx ftp://192.168.43.204:1337/ux0:/tai/$(TARGET).suprx
	curl -T config.txt ftp://192.168.43.204:1337/ux0:/tai/config.txt
	@echo "Sent."