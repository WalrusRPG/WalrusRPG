external_nspire_LOCAL_PATH := $(call whereami)

#CPPFLAGS += 

INCLUDE_EXT += $(external_nspire_LOCAL_PATH)/imgui $(external_nspire_LOCAL_PATH)/imgui-backends/SFML

SRCS_CPP += $(wildcard $(external_nspire_LOCAL_PATH)/imgui/*.cpp)

