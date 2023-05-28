PKG_CONFIG = pkg-config

INSTALL := install
MKDIR := mkdir
RMDIR := rmdir
LN := ln
RM := rm

CFLAGS += -std=c++11 -Wall -Wextra `$(PKG_CONFIG) --cflags mpv gio-2.0`
LDFLAGS += `$(PKG_CONFIG) --libs gio-2.0`

SCRIPTS_DIR := $(HOME)/.config/mpv/scripts

PREFIX := /usr/local
PLUGINDIR := $(PREFIX)/lib/wscrobble
SYS_SCRIPTS_DIR := /etc/mpv/scripts

.PHONY: \
  install install-user install-system \
  uninstall uninstall-user uninstall-system \
  clean

wscrobble.so: wscrobble.c
	echo `$(PKG_CONFIG) --cflags gio-2.0`
	$(CXX) wscrobble.c -o wscrobble.so $(CFLAGS) $(LDFLAGS) -shared -fPIC

ifneq ($(shell id -u),0)
install: install-user
uninstall: uninstall-user
else
install: install-system
uninstall: uninstall-system
endif

install-user: wscrobble.so
	$(MKDIR) -p $(SCRIPTS_DIR)
	$(INSTALL) -t $(SCRIPTS_DIR) wscrobble.so

uninstall-user:
	$(RM) -f $(SCRIPTS_DIR)/wscrobble.so
	$(RMDIR) -p $(SCRIPTS_DIR)

install-system: wscrobble.so
	$(MKDIR) -p $(DESTDIR)$(PLUGINDIR)
	$(INSTALL) -t $(DESTDIR)$(PLUGINDIR) wscrobble.so
	$(MKDIR) -p $(DESTDIR)$(SYS_SCRIPTS_DIR)
	$(LN) -s $(PLUGINDIR)/wscrobble.so $(DESTDIR)$(SYS_SCRIPTS_DIR)

uninstall-system:
	$(RM) -f $(DESTDIR)$(SYS_SCRIPTS_DIR)/wscrobble.so
	$(RMDIR) -p $(DESTDIR)$(SYS_SCRIPTS_DIR)
	$(RM) -f $(DESTDIR)$(PLUGINDIR)/wscrobble.so
	$(RMDIR) -p $(DESTDIR)$(PLUGINDIR)

clean:
	rm -f wscrobble.so
