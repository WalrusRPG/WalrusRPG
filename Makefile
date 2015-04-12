NAME = Pokespire

DEBUG = FALSE

GCC = nspire-gcc
GCCFLAGS = -Wall -W -marm
ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Ofast -flto
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif

LD = nspire-ld
LDFLAGS =

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)"

SOURCES = $(wildcard src/*.c) art/sprites.c
HEADERS = $(patsubst %.c,%.h,$(SOURCES))
OBJS = $(patsubst %.c,%.o,$(SOURCES))

DISTDIR = bin

all: exe

%.o: %.c headers
	@$(GCC) $(GCCFLAGS) -c $< -o $@

headers: sprites
	makeheaders $(SOURCES)

sprites:
	@$(MAKE) -C art/

exe: $(OBJS)
	@mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$(NAME).elf $(LDFLAGS)
	$(ZEHN) --input $(DISTDIR)/$(NAME).elf --output $(DISTDIR)/$(NAME).tns $(ZEHNFLAGS)
ifeq ($(DEBUG),FALSE)
	@rm -f $(DISTDIR)/*.gdb
endif

clean:
	rm -rf $(DISTDIR) $(OBJS) $(HEADERS)
	@$(MAKE) -C art/ clean

run: all
	tilp -ns $(DISTDIR)/$(NAME).tns > /dev/null

