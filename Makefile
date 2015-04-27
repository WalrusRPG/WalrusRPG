NAME = Pokespire

DEBUG = FALSE

CC = nspire-gcc
CFLAGS = -Wall -W -marm -flto

LD = nspire-ld
LDFLAGS = -flto

ifeq ($(DEBUG),FALSE)
	CFLAGS += -Ofast
else
	CFLAGS += -O0 -g
	LDFLAGS += --debug
endif

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)"

SOURCES = $(wildcard src/*.c) art/sprites.c
HEADERS = $(patsubst %.c,%.h,$(SOURCES))
OBJS = $(patsubst %.c,%.o,$(SOURCES))

DISTDIR = bin
ELF = $(DISTDIR)/$(NAME).elf
EXE = $(DISTDIR)/$(NAME).tns

all: $(EXE)

%.o: %.c headers
	@echo "CC: $@"
	@$(CC) $(CFLAGS) -c $< -o $@

headers: sprites
	makeheaders $(SOURCES)

sprites:
	@$(MAKE) -C art/

$(ELF): $(OBJS)
	@mkdir -p $(DISTDIR)
	@echo "LD: $@"
	@$(LD) $^ -o $(ELF) $(LDFLAGS)

$(EXE): $(ELF)
	@mkdir -p $(DISTDIR)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

clean:
	rm -rf $(DISTDIR)
	rm -f $(OBJS)
	rm -f $(HEADERS)
	@$(MAKE) -C art/ clean

run: all
	tilp -ns $(EXE) > /dev/null

