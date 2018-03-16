INSTALL= /usr/bin/install -c
INSTALL_PROGRAM= ${INSTALL}
INSTALL_DATA= ${INSTALL} -m 644
INSTALLNLSDIR=$(PREFIX)/share/locale

MSGMERGE = msgmerge

NLSPACKAGE = system-config-date-timezones

CATALOGS = $(patsubst %.po,%.mo,$(wildcard *.po))

all: $(NLSPACKAGE).pot update-po

$(NLSPACKAGE).h::
	(cd /usr/share/zoneinfo; find . -type f -or -type l) | grep '^./[A-Z]' | egrep -v "(/right/)|(/posix/)|(/zone\.tab$$)" | sort -u | cut -d '/' -f 2- | \
	while read tz; do \
		echo "N_(\"$$tz\");"; \
		comment="`awk '-F\t' '{if ($$3 ~ "^'"$$tz"'$$") { print $$4 }}' < /usr/share/zoneinfo/zone.tab`"; \
		if test -n "$$comment" ; then \
			echo "/* comment for time zone $$tz */"; \
			echo "N_(\"$$comment\");"; \
		fi; \
	done > $@

update-pot: $(NLSPACKAGE).pot
$(NLSPACKAGE).pot: Makefile $(NLSPACKAGE).h
	xgettext --default-domain=$(NLSPACKAGE) \
		--add-comments --keyword=_ --keyword=N_ $(NLSPACKAGE).h
	if msgcmp -N --use-untranslated $(NLSPACKAGE).pot $(NLSPACKAGE).po >&/dev/null && \
		msgcmp -N --use-untranslated $(NLSPACKAGE).po $(NLSPACKAGE).pot >&/dev/null; then \
		rm -f $(NLSPACKAGE).po; \
	else \
		mv $(NLSPACKAGE).po $(NLSPACKAGE).pot; \
	fi

update-po: Makefile update-pot
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
		lang=`echo $$cat | sed 's/.mo//'`; \
		if $(MSGMERGE) $$lang.po $(NLSPACKAGE).pot > $$lang.pot ; then \
			mv -f $$lang.pot $$lang.po ; \
			echo "$(MSGMERGE) of $$lang succeeded" ; \
		else \
			echo "$(MSGMERGE) of $$lang failed" ; \
			rm -f $$lang.pot ; \
		fi \
	done

refresh-po: Makefile
	catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
		lang=`echo $$cat | sed 's/.mo//'`; \
		if $(MSGMERGE) $$lang.po $(NLSPACKAGE).pot > $$lang.pot ; then \
			mv -f $$lang.pot $$lang.po ; \
			echo "$(MSGMERGE) of $$lang succeeded" ; \
		else \
			echo "$(MSGMERGE) of $$lang failed" ; \
			rm -f $$lang.pot ; \
		fi \
	done

clean:
#	rm -f *mo $(NLSPACKAGE).pot *~
	rm -f *mo *~

distclean: clean
	rm -f .depend Makefile

depend:

install: refresh-po $(CATALOGS)
	mkdir -p $(DESTDIR)/usr/share/locale
	for n in $(CATALOGS); do \
	    l=`basename $$n .mo`; \
	    $(INSTALL) -m 755 -d $(DESTDIR)/usr/share/locale/$$l; \
	    $(INSTALL) -m 755 -d $(DESTDIR)/usr/share/locale/$$l/LC_MESSAGES; \
	    if [ -f $$n ]; then \
	        $(INSTALL) -m 644 $$n $(DESTDIR)/usr/share/locale/$$l/LC_MESSAGES/$(NLSPACKAGE).mo; \
	    fi; \
	done

%.mo: %.po
	msgfmt -o $@ $<

MSGCONV=msgconv -t utf-8

force-utf8:
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
		lang=`echo $$cat | sed 's/.mo//'`; \
		if $(MSGCONV) $$lang.po > $$lang.po.utf8 ; then \
			mv -f $$lang.po.utf8 $$lang.po ; \
			echo "$(MSGCONV) of $$lang succeeded" ; \
		else \
			echo "$(MSGCONV) of $$lang failed" ; \
			rm -f $$lang.po.utf8 ; \
		fi \
	done

report:
	@catalogs='$(CATALOGS)'; \
	for cat in $$catalogs; do \
		lang=`echo $$cat | sed 's/.mo//'`; \
		echo -n "$$lang.po: " ; \
		msgfmt --statistics -o /dev/null $$lang.po ; \
	done
