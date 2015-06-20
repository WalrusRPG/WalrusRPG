NAME = WalrusRPG

DEBUG = FALSE

CFLAGS_COMMON = -Wall -W -marm -fdiagnostics-color=always -DGIT_VERSION='"$(BRANCH_NAME)-$(COMMIT_NUMBER)"'

ifeq ($(DEBUG),FALSE)
	CFLAGS_COMMON += -Ofast -flto
else
	CFLAGS_COMMON += -O0 -g
endif

CC = nspire-gcc
CFLAGS = $(CFLAGS_COMMON) -std=gnu11

CPP = nspire-g++
CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++11

LDFLAGS = $(CFLAGS_COMMON) -Wl,--gc-sections

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)" --compress

SRCDIR = src

SOURCES_C = art/sprites.c $(wildcard $(SRCDIR)/*.c)
SOURCES_CPP = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst %.c,%.o,$(SOURCES_C)) $(patsubst %.cpp,%.o,$(SOURCES_CPP))
INCLUDE = -I include -I art -I external/tinystl/include

DISTDIR = bin
ELF = $(DISTDIR)/$(NAME).elf
EXE = $(DISTDIR)/$(NAME).tns

all: versionning sprites $(EXE)

.PHONY: format clean sprites all run versionning

versionning:
	@bash versionning.bash

sprites:
	@$(MAKE) -C art/

art/sprites.c: sprites

%.o: %.c| sprites versionning
	@echo "CC: $@"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.cpp| sprites versionning
	@echo "CPP: $@"
	@$(CPP) $(CPPFLAGS) $(INCLUDE) -c $< -o $@


$(ELF): $(OBJS) |sprites
	@mkdir -p $(DISTDIR)
	@echo "CCLD: $@"
	@+$(CC) $(LDFLAGS) $^ -o $(ELF)

$(EXE): $(ELF)
	@mkdir -p $(DISTDIR)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

clean:
	rm -rf $(DISTDIR)
	rm -f $(OBJS)
	@$(MAKE) -C art/ clean

format:
	clang-format -i -style=file src/*.c src/*.cpp include/*.h

run: all
	tilp -ns $(EXE) > /dev/null
