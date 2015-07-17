.PHONY: format clean all run versionning include

all: $(EXE)

include $(wildcard */rules.mk)

$(OUT)/%.o: %.c | $(BUILT_SRCS)
	@echo "CC: $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c | $(BUILT_SRCS)
	@echo "CC: $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/%.o: %.cpp | $(BUILT_SRCS)
	@echo "CPP: $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp | $(BUILT_SRCS)
	@echo "CPP: $@"
	@$(CPP) $(CPPFLAGS) -c $< -o $@

$(ELF): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "CCLD: $@"
	@+$(CC) $(LDFLAGS) $^ -o $(ELF)

$(EXE): $(ELF)
	@mkdir -p $(dir $@)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

clean:
	@echo "RM: $(OUT)"
	@rm -rf $(OUT)

format:
	@echo "Formatting source using clang-format"
	@clang-format -i -style=file $(SRCS_C) $(SRCS_CPP) $(wildcard $(addsuffix /*.h,$(INCLUDE)))

include:
	@echo -n $(addprefix -I ,$(INCLUDE))

run: all
	@echo "TILP: $(EXE)"
	@tilp -ns $(EXE) > /dev/null

.FORCE:
