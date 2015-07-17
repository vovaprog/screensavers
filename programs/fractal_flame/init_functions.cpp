#include <vector>

using namespace std;

#include "Function.h"

void initFunctions(vector<Function*> &functions, int totalProbabilityWeight)
{
    functions.push_back(new FunctionSin());
    //functions.push_back(new Function2());
    /*functions.push_back(new Function3());
    functions.push_back(new Function4());*/
    
    //functions.push_back(new FunctionSpherical());
    //functions.push_back(new FunctionSwirl());
    //functions.push_back(new FunctionHorseshoe());
    //functions.push_back(new FunctionPolar());
    //functions.push_back(new FunctionHandkerchief());
    //functions.push_back(new FunctionHeart());
    functions.push_back(new FunctionDisk());
    //functions.push_back(new FunctionSpiral());
    functions.push_back(new FunctionHyperbolic());
    //functions.push_back(new FunctionDiamond());
    functions.push_back(new FunctionJulia());
    functions.push_back(new FunctionEx());
    functions.push_back(new FunctionBent());
    
    //Function *pFun;
    
    /*pFun = new FunctionFisheye();
    pFun->probabilityWeight=5;
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
