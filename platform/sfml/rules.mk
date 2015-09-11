sfml_LOCAL_PATH := $(call whereami)

SRCS_C += $(wildcard $(sfml_LOCAL_PATH)/platform/*.c)
SRCS_CPP += $(wildcard $(sfml_LOCAL_PATH)/*.cpp)
INCLUDE += $(sfml_LOCAL_PATH)/public

LDFLAGS += -lstdc++ -lsfml-window -lsfml-graphics -lsfml-system -lGL

CC = gcc
CPP = g++

PLATFORM = sfml
EXE = $(OUT)/$(NAME)

$(EXE): $(ELF)
	@cp $(ELF) $(EXE)

run: all
	./$(EXE)
