.PHONY: format clean all run versionning cflags release bundle

all: $(EXE)

include $(wildcard */rules.mk)

DATA_FILES=$(addsuffix $(DATA_FILE_SUFFIX), $(wildcard data/*))
MISC_FILES=$(wildcard bundle_files/*)
RELEASE_DIRECTORY=release/$(PLATFORM)
RELEASE_FILES=$(addprefix $(RELEASE_DIRECTORY)/, $(addsuffix $(DATA_FILE_SUFFIX), $(wildcard data/*)))
RELEASE_MISC_FILES=$(addprefix $(RELEASE_DIRECTORY)/, $(notdir $(MISC_FILES)))

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
	@+$(CPP) $(LDFLAGS) $^ $(LIBS) -o $(ELF)

clean:
	@echo "RM: $(OUT)"
	@rm -rf $(OUT)
	@echo "RM : $(RELEASE_DIRECTORY)"
	@rm -rf $(RELEASE_DIRECTORY)

make_release_dirs:
	@mkdir -p "$(RELEASE_DIRECTORY)/data"


# Data files
$(RELEASE_DIRECTORY)/%$(DATA_FILE_SUFFIX) : % | make_release_dirs
	@echo "$^ => $@"
	@cp -u "$^" "$@"

# Misc bundle files
$(RELEASE_DIRECTORY)/% : bundle_files/% | make_release_dirs
	@echo "$^ => $@"
	@cp -u "$^" "$@"

# Executable
$(RELEASE_DIRECTORY)/$(notdir $(EXE)): $(EXE)
	@echo "$^ => $@"
	@cp -u "$^" "$@"

release: $(EXE) $(RELEASE_FILES) $(RELEASE_MISC_FILES) $(RELEASE_DIRECTORY)/$(notdir $(EXE))

#Archive
release/$(PLATFORM).tar.gz:
	@echo "Tar-zipping"
	@cd release; tar cfz "$(PLATFORM).tar.gz" $(PLATFORM);

bundle: release/$(PLATFORM).tar.gz release
	@echo "Archive ready at $(RELEASE_DIRECTORY).tar.gz"

format:
	@echo "Formatting source using clang-format"
	@clang-format -i -style=file $(shell git ls-files -c -o --exclude-standard *.c *.cpp *.h)

cflags:
	@echo -n $(CPPFLAGS) $(YCM_EXTRA_CFLAGS)

.FORCE:
