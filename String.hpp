#ifndef __DFTPD__STRING_HPP__
#define __DFTPD__STRING_HPP__

#include <vector>
#include <string>

std::vector<std::string> SplitPath( const std::string& str );
std::vector<std::string> Split( const std::string& str );
std::vector<std::string> ParseCommand( const std::string& cmd );
void ToUpper( std::string& str );

#endif
