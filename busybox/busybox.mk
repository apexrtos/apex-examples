#
# Busybox
#

TYPE := custom
TARGET := busybox

# This hackery is required to build a static pie busybox binary.
# LDFLAGS is supposed to be extra flags for the compiler when invoking the
# linker but Busybox passes LDFLAGS to $(LD) rather than to $(CC) so we
# need to shuffle some flags around.
busybox_LDFLAGS := $(LDFLAGS) $(LDFLAGS_$(COMPILER)) $(CONFIG_USER_LDFLAGS)
busybox_CFLAGS := $(CFLAGS) $(CFLAGS_$(COMPILER)) $(CONFIG_USER_CFLAGS) \
		  $(filter -static,$(busybox_LDFLAGS)) \
		  $(filter -pie,$(busybox_LDFLAGS)) \
		  $(filter -static-pie,$(busybox_LDFLAGS)) \
		  -Wno-ignored-optimization-argument \
		  -Wno-unused-command-line-argument \
		  -Wno-string-plus-int \
		  -Wno-format-security \
		  -Wno-unused-function
busybox_LDFLAGS := $(filter-out -static,$(busybox_LDFLAGS))
busybox_LDFLAGS := $(filter-out -pie,$(busybox_LDFLAGS))
busybox_LDFLAGS := $(filter-out -static-pie,$(busybox_LDFLAGS))

busybox/.config: $(CONFIG_SRCDIR)/busybox/config conf/config.mk | $(CURDIR)/busybox/
	cp $< $@
ifeq ($(origin CONFIG_MMU),undefined)
	echo "CONFIG_NOMMU=y" >> $@
else
	echo "# CONFIG_NOMMU is not set" >> $@
endif
	echo "CONFIG_CROSS_COMPILER_PREFIX=\"$(CONFIG_CROSS_COMPILE)\"" >> $@
	echo "CONFIG_EXTRA_CFLAGS=\"$(busybox_CFLAGS)\"" >> $@
	echo "CONFIG_EXTRA_LDFLAGS=\"$(busybox_LDFLAGS)\"" >> $@

busybox/.copied: | $(CURDIR)/busybox/
	cp -rs $(CONFIG_SRCDIR)/busybox/src/* busybox
	touch $@

busybox/busybox: busybox/.config busybox/.copied
	MAKEFLAGS="$(_MFLAGS)" $(MAKE) CC=$(CONFIG_CROSS_COMPILE)cc -C busybox $(SHUTUP_IF_SILENT)
ifneq ($(CONFIG_SSTRIP),)
	sstrip $@
endif

busybox/busybox_CLEAN :=

.PHONY: busybox/busybox_clean2
clean: busybox/busybox_clean2
busybox/busybox_clean2:
	rm -rf busybox
