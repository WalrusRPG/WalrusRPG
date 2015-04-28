NAME = Pokespire

DEBUG = FALSE

CC = nspire-gcc
CFLAGS = -Wall -W -marm -std=gnu11 -I include -I art

ifeq ($(DEBUG),FALSE)
	CFLAGS += -Ofast -flto
else
	CFLAGS += -O0 -g
endif

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)"

SOURCES = art/sprites.c $(wildcard src/*.c)
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
	@echo "CC: $@"
	@+$(CC) $^ -o $(ELF) $(CFLAGS)

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

