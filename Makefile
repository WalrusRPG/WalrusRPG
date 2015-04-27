NAME = Pokespire

DEBUG = FALSE

CC = nspire-gcc
CFLAGS = -Wall -W -marm -I include -I art

LD = nspire-ld
LDFLAGS =

ifeq ($(DEBUG),FALSE)
	CFLAGS += -Ofast -flto
	LDFLAGS += -flto
else
	CFLAGS += -O0 -g
	LDFLAGS += --debug
endif

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)"

SOURCES = $(wildcard src/*.c) art/sprites.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))

DISTDIR = bin
ELF = $(DISTDIR)/$(NAME).elf
EXE = $(DISTDIR)/$(NAME).tns

all: $(EXE)

%.o: %.c
	@echo "CC: $@"
	@$(CC) $(CFLAGS) -c $< -o $@

art/sprites.c:
	@$(MAKE) -C art/

$(ELF): $(OBJS)
	@mkdir -p $(DISTDIR)
	@echo "LD: $@"
	@+$(LD) $^ -o $(ELF) $(LDFLAGS)

$(EXE): $(ELF)
	@mkdir -p $(DISTDIR)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

clean:
	rm -rf $(DISTDIR)
	rm -f $(OBJS)
	@$(MAKE) -C art/ clean

run: all
	tilp -ns $(EXE) > /dev/null

