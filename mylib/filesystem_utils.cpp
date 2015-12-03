#include <boost/filesystem.hpp>
#include <cstdio>

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

bool readTextFile(const char *fileName, char **output)
{
    FILE *fp;
    
    if(!(fp = fopen(fileName, "r"))) 
	{		
		return false;
	}
	
	fseek(fp, 0, SEEK_END);
	long int length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*output = new char[length+1];

	length = fread(*output, 1, length, fp); 
    
	(*output)[length] = 0;
	
	fclose(fp);
	
	return true;
}

