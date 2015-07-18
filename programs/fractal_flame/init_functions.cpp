#include <vector>

using namespace std;

#include "Function.h"

void initFunctions(vector<Function*> &functions, int &totalProbabilityWeight)
{
    Function *pFun;
    
    /*
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
    functions.push_back(pFun);*/
    
    
    
    
    
    /* good picture
    pFun = new Function();
    //pFun->variations.push_back(variationSin);
    pFun->variations.push_back(variationHyperbolic);    
    pFun->r=200;
    functions.push_back(pFun);

    pFun = new Function();    
    pFun->variations.push_back(variationSwirl);
    pFun->variations.push_back(variationHorseshoe);    
    pFun->b=200;
    
    functions.push_back(pFun);    
    */
    

    pFun = new Function();
    //pFun->variations.push_back(variationSin);
    pFun->variations.push_back(variationHyperbolic);    
    pFun->r=200;
    functions.push_back(pFun);

    pFun = new Function();    
    pFun->variations.push_back(variationSwirl);
    pFun->variations.push_back(variationHorseshoe);    
    pFun->b=200;
    
    functions.push_back(pFun);    
    
    
    
    
    /*pFun->preTransformKoef[0][0]=1.5;
    pFun->preTransformKoef[0][1]=0.14;
    pFun->preTransformKoef[0][2]=0.87;
    
    pFun->preTransformKoef[1][0]=-1.9;
    pFun->preTransformKoef[1][1]=-2.85;
    pFun->preTransformKoef[1][2]=-1.85;*/           

    /*pFun->preTransformKoef[0][0]=-1.6;
    pFun->preTransformKoef[0][1]=-2.5;
    pFun->preTransformKoef[0][2]=-1.6;
    
    pFun->preTransformKoef[1][0]=-0.12;
    pFun->preTransformKoef[1][1]=-0.39;
    pFun->preTransformKoef[1][2]=-0.06;*/            
    
    
    

    
    totalProbabilityWeight=0;
    
    for(auto funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }
}
