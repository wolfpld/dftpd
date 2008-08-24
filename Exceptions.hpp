#ifndef __DFTPD__EXCEPTIONS_HPP__
#define __DFTPD__EXCEPTIONS_HPP__

class ConnectionTerminated {};
class SyntaxError {};
class QuitRequested {};
class SessionError {};
class ServerCrash {};

extern ConnectionTerminated ConnectionTerminatedException;
extern SyntaxError SyntaxErrorException;
extern QuitRequested QuitRequestedException;
extern SessionError SessionErrorException;
extern ServerCrash ServerCrashException;

#endif
