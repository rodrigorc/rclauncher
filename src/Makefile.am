UTIL=$(top_srcdir)/util
AM_CPPFLAGS+=-DDATADIR=\"$(datadir)\" -I"$(UTIL)" -DGTK_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED $(GTK_CFLAGS)

LDADD=$(GTK_LIBS) -llirc_client

bin_PROGRAMS=rclauncher
rclauncher_SOURCES=rclauncher.cpp \
	$(UTIL)/miglib/miglib.h $(UTIL)/miglib/migtk.h $(UTIL)/miglib/migtkconn.h $(UTIL)/miglib/mipango.h \
	$(UTIL)/miauto.h $(UTIL)/micairo.h \
	$(UTIL)/xml/mixmlparse.h $(UTIL)/xml/simplexmlparse.h
