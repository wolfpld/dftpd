#include "Filesystem.hpp"

Filesystem::Filesystem( const std::string& root )
	: m_root( root )
	, m_path( "/" )
{
}

Filesystem::~Filesystem()
{
}
