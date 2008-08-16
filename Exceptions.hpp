#ifndef __DFTPD__EXCEPTIONS_HPP__
#define __DFTPD__EXCEPTIONS_HPP__

class ConnectionTerminated {};
class SyntaxError {};

extern ConnectionTerminated ConnectionTerminatedException;
extern SyntaxError SyntaxErrorException;

#endif
