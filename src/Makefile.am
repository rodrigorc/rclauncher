UTIL=$(top_srcdir)/util
AM_CPPFLAGS+=-DDATADIR=\"$(datadir)\" -I"$(UTIL)" -DGTK_DISABLE_DEPRECATED -DG_DISABLE_DEPRECATED $(GTK_CFLAGS) -DTIXML_USE_STL

LDADD=$(GTK_LIBS) -llirc_client

bin_PROGRAMS=remote_browser
remote_browser_SOURCES=remote_browser.cpp \
	$(UTIL)/xml/tinyxml.cpp $(UTIL)/xml/tinyxml.h $(UTIL)/xml/tinyxmlparser.cpp $(UTIL)/xml/tinyxmlerror.cpp
