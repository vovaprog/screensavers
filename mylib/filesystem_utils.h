#pragma once

bool directoryExists(const char *dirName);

bool fileExists(const char *fileName);

void createDirectory(const char *dirName);

void deleteDirectory(const char *dirName);

void deleteFile(const char *fileName);

bool readTextFile(const char *fileName, char **output);
