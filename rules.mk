.PHONY: format clean all run versionning include

all: $(EXE)

include $(wildcard */rules.mk)

%.o: %.c | $(BUILT_SRCS)
	@echo "CC: $@"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp | $(BUILT_SRCS)
	@echo "CPP: $@"
	@$(CPP) $(CPPFLAGS) -c $< -o $@

$(ELF): $(OBJS)
	@mkdir -p $(DISTDIR)
	@echo "CCLD: $@"
	@+$(CC) $(LDFLAGS) $^ -o $(ELF)

$(EXE): $(ELF)
	@mkdir -p $(DISTDIR)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

clean:
	@echo "Removing dist"
	@rm -rf $(DISTDIR)
	@echo "Removing object files"
	@rm -f $(OBJS)
	@echo "Removing other build artifacts"
	@rm -f $(CLEAN_SPEC)

format:
	@echo "Formatting source using clang-format"
	@clang-format -i -style=file $(SRCS_C) $(SRCS_CPP) $(wildcard $(addsuffix /*.h,$(INCLUDE)))

include:
	@echo $(addprefix -I ,$(INCLUDE))

run: all
	@echo "Sending $(EXE) to calculator"
	@tilp -ns $(EXE) > /dev/null

.FORCE:
