CFLAGS = -g -Wall
LDFLAGS = 

LIBS =
INCLUDES =

# --

OBJ = \
	AuthNone.o \
	Data.o \
	Exceptions.o \
	Filesystem.o \
	Listener.o \
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
