PWD = ${shell pwd}

SRCS = ${shell find ../ -maxdepth 1 -type f -name '*.cpp' -or -name '*.hpp'}
RESOURCES = \
	dftpd_loc.rss \
	dftpd_reg.rss \
	../resource/dftpd.rls \
	../resource/dftpd.rss \
	../resource/dftpd.svg \
	../resource/dftpd_01.rls \
	../resource/dftpd_27.rls

ICONSRC = ../resource/dftpd.svg

all: dftpd.sisx

dftpd.sisx: dftpd.sis
	signsis dftpd.sis dftpd.sisx

dftpd.sis: $(EPOCROOT)/epoc32/build/$(PWD)/dftpd/gcce/urel/dftpd.exe dftpd.pkg dftpd.mif $(RESOURCES)
	makesis -d$(EPOCROOT) dftpd.pkg

$(EPOCROOT)/epoc32/build/$(PWD)/dftpd/gcce/urel/dftpd.exe: dftpd.gcce $(SRCS) $(RESOURCES)
	+make -f dftpd.gcce UREL

dftpd.mif: $(ICONSRC)
	mifconv dftpd.mif $(ICONSRC)

dftpd.gcce: dftpd.mmp $(EPOCROOT)/epoc32/build/$(PWD)/gcce.make
	+$(EPOCROOT)/epoc32/tools/makmake dftpd.mmp gcce

clean: dftpd.gcce
	make -f dftpd.gcce CLEAN
	rm -f dftpd.gcce
	rm -f dftpd.sis
	rm -f dftpd.sisx
	rm -f dftpd.*.objects.via
	rm -f dftpd.mif

$(EPOCROOT)/epoc32/build/$(PWD)/gcce.make: bld.inf
	bldmake bldfiles
