#include <vector>

using namespace std;

#include "FunctionNew.h"

void initFunctions(vector<Function*> &functions, int &totalProbabilityWeight)
{
    Function *pFun;
    
    pFun = new Function();
    pFun->variations.push_back(variationSin);
    functions.push_back(pFun);

    pFun = new Function();    
    pFun->variations.push_back(variationSwirl);
    pFun->variations.push_back(variationPolar);
    pFun->variations.push_back(variationHandkerchief);                
    functions.push_back(pFun);

/*    Function *pFun;
    
    pFun = new FunctionSin();
    functions.push_back(pFun);

    pFun = new FunctionJulia();
    functions.push_back(pFun);

    pFun = new FunctionHyperbolic();
    functions.push_back(pFun);
    
    pFun = new FunctionDisk();
    functions.push_back(pFun);*/

    /*pFun = new FunctionHyperbolic();
    functions.push_back(pFun);

    pFun = new FunctionJulia();
    functions.push_back(pFun);
    
    pFun = new FunctionEx();
    functions.push_back(pFun);
    
    pFun = new FunctionBent();
    functions.push_back(pFun);*/

    /*pFun = new FunctionMirror();
    pFun->probabilityWeight=2;
    functions.push_back(pFun);*/
    
    totalProbabilityWeight=0;
    
    for(auto funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }
}
