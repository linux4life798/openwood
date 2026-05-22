EXAMPLES := \
	buttons \
	exception-recover \
	exceptions \
	hello-world \
	jump-fldm-loader \
	main-exit \
	mmu-test \
	serial

.PHONY: all clean $(EXAMPLES) $(addprefix clean-,$(EXAMPLES))

all: $(EXAMPLES)

$(EXAMPLES):
	$(MAKE) -C $@

clean: $(addprefix clean-,$(EXAMPLES))

$(addprefix clean-,$(EXAMPLES)): clean-%:
	$(MAKE) -C $* clean
