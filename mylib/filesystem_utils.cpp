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

bool fileExists(const char *fileName)
{
	if(exists(fileName))
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

void deleteDirectory(const char *dirName)
{
    remove_all(dirName);
}

void deleteFile(const char *fileName)
{
    remove(fileName);
}