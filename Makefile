CFLAGS = -g -Wall
LDFLAGS = 

LIBS = -lpthread
INCLUDES =

# --

OBJ = \
	AuthNone.o \
	AuthToken.o \
	Data.o \
	DataBufferFile.o \
	DataBufferListing.o \
	Exceptions.o \
	Filesystem.o \
	Listener.o \
	LogSTDOUT.o \
	Main.o \
	Server.o \
	SessionController.o \
	Session.o \
	String.o \
	Telnet.o \
	TelnetCommand.o

all: dftpd

clean:
	rm -f *.o *.d dftpd

.cpp.o:
	$(CXX) -c $(INCLUDES) $(CFLAGS) $<

dftpd: $(OBJ)
	$(CXX) $(OBJ) -o dftpd $(LIBS)

%.d: %.cpp
	@$(CXX) -MM $(CFLAGS) $(INCLUDES) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(OBJ:.o=.d)

.PHONY: clean
