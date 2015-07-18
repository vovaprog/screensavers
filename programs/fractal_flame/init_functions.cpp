#include <vector>

using namespace std;

#include "Function.h"

void initFunctions(vector<Function*> &functions, int &totalProbabilityWeight)
{
    Function *pFun;
    
    pFun = new Function();
    pFun->variations.push_back(variationSin);
    pFun->variations.push_back(variationHyperbolic);
    pFun->r=200;
    functions.push_back(pFun);

    pFun = new Function();    
    //pFun->variations.push_back(variationSwirl);
    //pFun->variations.push_back(variationPolar);
    pFun->variations.push_back(variationHandkerchief);
    pFun->variations.push_back(variationDiamond);
    pFun->b=200;
    functions.push_back(pFun);

    
    totalProbabilityWeight=0;
    
    for(auto funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }
}
