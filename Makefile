CFLAGS = -g -Wall
LDFLAGS = 

LIBS =
INCLUDES =

# --

OBJ = \
	Listener.o \
	Main.o \
	Session.o \
	SessionController.o \
	Server.o \
	Telnet.o

all: dftpd

clean:
	rm -f *.o dftpd

.cpp.o:
	$(CXX) -c $(INCLUDES) $(CFLAGS) $<

dftpd: $(OBJ)
	$(CXX) $(OBJ) -o dftpd $(LIBS)

.PHONY: clean
