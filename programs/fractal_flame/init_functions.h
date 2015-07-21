#pragma once

void saveFunctions(const char *fileName,vector<Function*> &functions);

void initFunctionsRandom(vector<Function*> &functions, int &totalProbabilityWeight);

void loadFunctions(const char *fileName,vector<Function*> &functions, int &totalProbabilityWeight);

