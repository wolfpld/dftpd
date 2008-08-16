#ifndef __DFTPD__EXCEPTIONS_HPP__
#define __DFTPD__EXCEPTIONS_HPP__

class ConnectionTerminated {};
class SyntaxError {};
class QuitRequested {};

extern ConnectionTerminated ConnectionTerminatedException;
extern SyntaxError SyntaxErrorException;
extern QuitRequested QuitRequestedException;

#endif
