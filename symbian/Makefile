PWD = ${shell pwd}

SRCS = ${shell find ../ -maxdepth 1 -type f}

all: dftpd.sisx

dftpd.sisx: dftpd.sis
	signsis dftpd.sis dftpd.sisx

dftpd.sis: $(EPOCROOT)/epoc32/build/$(PWD)/openttd/gcce/urel/dftpd.exe dftpd.pkg
	makesis -d$(EPOCROOT) dftpd.pkg

$(EPOCROOT)/epoc32/build/$(PWD)/openttd/gcce/urel/dftpd.exe: dftpd.gcce $(SRCS)
	+make -f dftpd.gcce UREL

dftpd.gcce: dftpd.mmp $(EPOCROOT)/epoc32/build/$(PWD)/gcce.make
	+$(EPOCROOT)/epoc32/tools/makmake dftpd.mmp gcce

clean: dftpd.gcce
	make -f dftpd.gcce CLEAN
	rm -f dftpd.gcce
	rm -f dftpd.sis
	rm -f dftpd.sisx
	rm -f dftpd.*.objects.via

$(EPOCROOT)/epoc32/build/$(PWD)/gcce.make: bld.inf
	bldmake bldfiles
