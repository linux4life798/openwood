EXAMPLES := \
	buttons \
	exception-recover \
	exceptions \
	hello-world \
	jump-fldm-loader \
	main-exit \
	reboot \
	serial \
	shutdown

# mmu-test is not included because it is not tracked/CI-enabled yet.

.PHONY: all clean $(EXAMPLES) $(addprefix clean-,$(EXAMPLES))

all: $(EXAMPLES)

$(EXAMPLES):
	$(MAKE) -C $@

clean: $(addprefix clean-,$(EXAMPLES))

$(addprefix clean-,$(EXAMPLES)): clean-%:
	$(MAKE) -C $* clean
