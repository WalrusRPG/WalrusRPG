NAME = WalrusRPG

DEBUG = FALSE

CFLAGS_COMMON = -Wall -W -marm -I include -I art -fdiagnostics-color=always

ifeq ($(DEBUG),FALSE)
	CFLAGS_COMMON += -Ofast -flto
else
	CFLAGS_COMMON += -O0 -g
endif

CC = nspire-gcc
CFLAGS = $(CFLAGS_COMMON) -std=gnu11

CPP = nspire-g++
CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++98

LDFLAGS = $(CFLAGS_COMMON)

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)"

INCDIR = include
SRCDIR = src

SOURCES_C = art/sprites.c $(wildcard $(SRCDIR)/*.c)
SOURCES_CPP = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst %.c,%.o,$(SOURCES_C)) $(patsubst %.cpp,%.o,$(SOURCES_CPP))

DISTDIR = bin
ELF = $(DISTDIR)/$(NAME).elf
EXE = $(DISTDIR)/$(NAME).tns


all: $(EXE)

%.o: %.c
	@echo "CC: $@"
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

%.o: %.cpp
	@echo "CPP: $@"
	@$(CPP) $(CPPFLAGS) -I$(INCDIR) -c $< -o $@

art/sprites.c:
	@$(MAKE) -C art/

$(ELF): $(OBJS)
	@mkdir -p $(DISTDIR)
	@echo "CC: $@"
	@+$(CC) $(LDFLAGS) $^ -o $(ELF)

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

