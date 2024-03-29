#ifndef __DFTPD__STRING_HPP__
#define __DFTPD__STRING_HPP__

#include <vector>
#include <string>

typedef std::vector<std::string> PathVector;
typedef std::vector<std::string> Command;
typedef std::vector<std::string> PortVector;

PathVector SplitPath( const std::string& str );
PortVector SplitPort( const std::string& str );
Command Split( const std::string& str );
Command ParseCommand( const std::string& cmd );
void ToUpper( std::string& str );

#endif
