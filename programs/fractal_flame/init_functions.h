#pragma once

void saveFunctions(const char *fileName,std::vector<std::unique_ptr<Function>> &functions);

void initFunctionsRandom(std::vector<std::unique_ptr<Function>> &functions, int &totalProbabilityWeight);

void loadFunctions(const char *fileName,std::vector<std::unique_ptr<Function>> &functions, int &totalProbabilityWeight);

