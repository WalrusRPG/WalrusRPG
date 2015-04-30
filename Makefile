NAME = WalrusRPG

DEBUG = FALSE

CC = nspire-g++
CFLAGS = -Wall -W -marm -std=gnu++98 -I include -I art

ifeq ($(DEBUG),FALSE)
	CFLAGS += -Ofast -flto
else
	CFLAGS += -O0 -g
endif

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)"

INCDIR = include
SRCDIR = src

SOURCES_C = art/sprites.c $(wildcard SRCDIR/*.c)
SOURCES_CPP = $(wildcard SRCDIR/*.cpp)
OBJS = $(patsubst %.c,%.o,$(SOURCES_C)) $(patsubst %.cpp,%.o,$(SOURCES_CPP)) 

DISTDIR = bin
ELF = $(DISTDIR)/$(NAME).elf
EXE = $(DISTDIR)/$(NAME).tns


all: $(EXE)

%.o: %.c %.cpp
	@echo "CC: $@"
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

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
	echo $(OBJS)
	rm -rf $(DISTDIR)
	rm -f $(OBJS)
	@$(MAKE) -C art/ clean

run: all
	tilp -ns $(EXE) > /dev/null

