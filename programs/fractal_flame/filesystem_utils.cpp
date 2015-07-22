#include <boost/filesystem.hpp>

using namespace boost::filesystem;

bool directoryExists(const char *dirName)
{
	if(is_directory(dirName))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void createDirectory(const char *dirName)
{
	create_directories(dirName);
}

