DEBUG = FALSE

GCC = nspire-gcc
GCCFLAGS = -Wall -W -marm
ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Ofast
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif

LD = nspire-ld-bflt
LDFLAGS =

SOURCES = $(wildcard src/*.c) art/sprites.c
HEADERS = $(patsubst %.c,%.h,$(SOURCES))
OBJS = $(patsubst %.c,%.o,$(SOURCES))

EXE = pokespire.tns
DISTDIR = bin

all: $(EXE)

%.o: %.c headers
	@$(GCC) $(GCCFLAGS) -c $< -o $@

headers: sprites
	makeheaders $(SOURCES)

sprites:
	@$(MAKE) -C art/

$(EXE): $(OBJS)
	@mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)
ifeq ($(DEBUG),FALSE)
	@rm -f $(DISTDIR)/*.gdb
endif

clean:
	rm -f $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE) $(OBJS) $(HEADERS)
	@$(MAKE) -C art/ clean

run: all
	tilp -ns $(DISTDIR)/$(EXE) > /dev/null

