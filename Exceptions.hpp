#ifndef __DFTPD__EXCEPTIONS_HPP__
#define __DFTPD__EXCEPTIONS_HPP__

class ConnectionTerminated {};
class SyntaxError {};
class QuitRequested {};
class SessionError {};

extern ConnectionTerminated ConnectionTerminatedException;
extern SyntaxError SyntaxErrorException;
extern QuitRequested QuitRequestedException;
extern SessionError SessionErrorException;

#endif
