#
# Toybox
#

TYPE := custom
TARGET := toybox

toybox_CFLAGS := $(CFLAGS) $(CONFIG_USER_CFLAGS)

toybox/.config: $(CONFIG_SRCDIR)/toybox/config conf/config.mk | $(CURDIR)/toybox/
	cp $< $@
ifeq ($(origin CONFIG_MMU),undefined)
	echo "# CONFIG_TOYBOX_FORK is not set" >> $@
else
	echo "CONFIG_TOYBOX_FORK=y" >> $@
endif

toybox/.copied:
	cp -rs $(CONFIG_SRCDIR)/toybox/src/* toybox
	touch $@

toybox/toybox: toybox/.config toybox/.copied
	$(MAKE) -j1 -C toybox \
		CFLAGS="$(toybox_CFLAGS) -Wundef -Wno-char-subscripts" \
		CROSS_COMPILE="$(CROSS_COMPILE)" $(SHUTUP_IF_SILENT)
	chmod 755 $@
ifneq ($(CONFIG_SSTRIP),)
	sstrip $@
endif

toybox/toybox_CLEAN :=

.PHONY: toybox/toybox_clean2
clean: toybox/toybox_clean2
toybox/toybox_clean2:
	rm -rf toybox
