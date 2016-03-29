sfml_LOCAL_PATH := $(call whereami)

SRCS_C += $(wildcard $(sfml_LOCAL_PATH)/platform/*.c)
SRCS_CPP += $(wildcard $(sfml_LOCAL_PATH)/*.cpp)
INCLUDE += $(sfml_LOCAL_PATH)/public

LDFLAGS += -L/usr/lib -lstdc++ -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32 -lm

CFLAGS_COMMON += -DTARGET_WINDOWS=1 -mwindows

INCLUDE_EXT += /usr/include

CC = mingw32-gcc
CPP = mingw32-g++

EXE = $(OUT)/$(NAME).exe

$(EXE): $(ELF)
	@cp $(ELF) $(EXE)

run: all
	./$(EXE)
