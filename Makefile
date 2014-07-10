DEBUG = FALSE
GCC = nspire-gcc
GCCFLAGS = -Wall -W -marm
ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif

AS = nspire-as
GXX = nspire-g++

LD = nspire-ld-bflt
LDFLAGS =
CPPOBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS = $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.S,%.o,$(wildcard *.S)) $(CPPOBJS)
ifneq ($(strip $(CPPOBJS)),)
	LDFLAGS += --cpp
endif

SOURCES = $(wildcard *.c) $(wildcard *.S) $(wildcard *.cpp)

EXE = pokespire.tns
DISTDIR = bin
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.c headers
	$(GCC) $(GCCFLAGS) -c $<

%.o: %.cpp headers
	$(GXX) $(GCCFLAGS) -c $<

%.o: %.S headers
	$(AS) -c $<

headers:
	makeheaders $(SOURCES)

$(EXE): $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $(DISTDIR)/$@ $(LDFLAGS)
ifeq ($(DEBUG),FALSE)
	@rm -f $(DISTDIR)/*.gdb
endif

clean:
	rm -f *.h *.o *.elf $(DISTDIR)/*.gdb $(DISTDIR)/$(EXE)

