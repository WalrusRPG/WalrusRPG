.PHONY: format clean all run versionning cflags release bundle

all: $(EXE)

include $(wildcard */rules.mk)

RELEASE_DIRECTORY=release/$(PLATFORM)

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

release: $(ELF)
	@echo "Packing binary and data files into $(RELEASE_DIRECTORY)"
	@mkdir -p "$(RELEASE_DIRECTORY)"
	@cp $(ELF) "$(RELEASE_DIRECTORY)"
	@cp -ru data "$(RELEASE_DIRECTORY)" 2>/dev/null || :

bundle: release
	@echo "Tar-zipping"
	@tar cf "$(RELEASE_DIRECTORY).tar" "$(RELEASE_DIRECTORY)"
	@gzip "$(RELEASE_DIRECTORY).tar" -f
	@echo "Archive ready at $(RELEASE_DIRECTORY).tar.gzip"

format:
	@echo "Formatting source using clang-format"
	@clang-format -i -style=file $(shell git ls-files -c -o --exclude-standard *.c *.cpp *.h)

cflags:
	@echo -n $(CPPFLAGS) $(YCM_EXTRA_CFLAGS)

.FORCE:
