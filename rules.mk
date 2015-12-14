.PHONY: format clean all run versionning include release bundle

all: $(EXE)

include $(wildcard */rules.mk)

RELEASE_DIRECTORY=release/$(PLATFORM)
RELEASE_FILES=$(addprefix $(RELEASE_DIRECTORY)/, $(addsuffix $(DATA_FILE_SUFFIX), $(wildcard data/*)))

# Object dependency files
-include $(OBJS:%.o=%.d)

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

clean:
	@echo "RM: $(OUT)"
	@rm -rf $(OUT)
	@echo "RM : $(RELEASE_DIRECTORY)"
	@rm -rf $(RELEASE_DIRECTORY)

make_release_dirs:
	@mkdir -p "$(RELEASE_DIRECTORY)/data"

release/$(PLATFORM)/%$(DATA_FILE_SUFFIX) : % | make_release_dirs
	@echo "$^=> $@"
	@cp -u "$^" "$@"


release: $(ELF) $(RELEASE_FILES)
	@cp $(ELF) "$(RELEASE_DIRECTORY)"


bundle: release
	@echo "Tar-zipping"
	@tar cf "$(RELEASE_DIRECTORY).tar" "$(RELEASE_DIRECTORY)"
	@gzip "$(RELEASE_DIRECTORY).tar" -f
	@echo "Archive ready at $(RELEASE_DIRECTORY).tar.gzip"

format:
	@echo "Formatting source using clang-format"
	@clang-format -i -style=file $(shell git ls-files -c -o --exclude-standard *.c *.cpp *.h)

include:
	@echo -n $(addprefix -I ,$(INCLUDE) $(INCLUDE_EXT))

.FORCE:
