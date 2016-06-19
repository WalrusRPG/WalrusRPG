external_LOCAL_PATH := $(call whereami)

ifneq (, $(findstring tinystl, $(DEPS)))
    INCLUDE_EXT += $(external_LOCAL_PATH)/tinystl/include
endif

ifneq (, $(findstring lodepng, $(DEPS)))
    CPPFLAGS += -DLODEPNG_NO_COMPILE_CPP
    INCLUDE_EXT += $(external_LOCAL_PATH)/lodepng
    SRCS_CPP += $(external_LOCAL_PATH)/lodepng/lodepng.cpp
endif

ifneq (, $(findstring imgui, $(DEPS)))
    INCLUDE_EXT += $(external_LOCAL_PATH)/imgui $(external_LOCAL_PATH)/imgui-backends/SFML
	SRCS_CPP += $(external_LOCAL_PATH)/imgui/imgui.cpp $(external_LOCAL_PATH)/imgui/imgui_draw.cpp
endif
