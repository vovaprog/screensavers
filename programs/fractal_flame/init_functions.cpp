#include <vector>
#include <cstdlib>

using namespace std;

#include "Function.h"

void initFunctionProbabilities(vector<Function*> &functions, int &totalProbabilityWeight)
{
    totalProbabilityWeight=0;
    
    for(auto funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }    
}


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
    
    
    initFunctionProbabilities(functions,totalProbabilityWeight);
}

const int MIN_NUMBER_OF_FUNCTIONS = 2;
const int MAX_NUMBER_OF_FUNCTIONS = 3;
const int MIN_NUMBER_OF_VARIATIONS = 1;
const int MAX_NUMBER_OF_VARIATIONS = 3;


double getRandom01()
{
    return ((double)(rand() % 1000))/1000.0;
}

double getRandomValue(double start, double end)
{
    return start + getRandom01() * (end - start);
}

void initFunctionsRandom(vector<Function*> &functions, int &totalProbabilityWeight)
{
    vector<VariationPointer> variations;
    variations.push_back(variationSin);
    variations.push_back(variationFisheye);            
    variations.push_back(variationSpherical);    
    variations.push_back(variationSwirl);    
    variations.push_back(variationHorseshoe);    
    variations.push_back(variationPolar);    
    variations.push_back(variationHandkerchief);    
    variations.push_back(variationHeart);    
    variations.push_back(variationDisk);    
    variations.push_back(variationSpiral);    
    variations.push_back(variationHyperbolic);    
    variations.push_back(variationDiamond);    
    variations.push_back(variationJulia);    
    variations.push_back(variationEx);    
    variations.push_back(variationBent);    
    //variations.push_back(variationMirror);
    
    
    int numberOfFunctions=MIN_NUMBER_OF_FUNCTIONS + rand() % (MAX_NUMBER_OF_FUNCTIONS + 1 - MIN_NUMBER_OF_FUNCTIONS);
    
    for(int i=0;i<numberOfFunctions;i++)
    {
        Function *pFun = new Function();
                
        int numberOfVariations=MIN_NUMBER_OF_VARIATIONS + rand() % (MAX_NUMBER_OF_VARIATIONS + 1 - MIN_NUMBER_OF_VARIATIONS);
        
        for(int j=0;j<numberOfVariations;j++)
        {        
            int variationIndex = rand() % variations.size();                    
            pFun->variations.push_back(variations[variationIndex]);
        }
        
        pFun->r = 200 + rand() % 56; 
        pFun->g = 200 + rand() % 56;
        pFun->b = 200 + rand() % 56;        
        
        /*for(int r=0;r<2;r++)
        {
            for(int c=0;c<3;c++)
            {
                //pFun->preTransformKoef[r][c]=getRandom01();
                //pFun->postTransformKoef[r][c]=getRandom01();
            }
        }*/
               
        
        const double KDEV = 0.2;        
        
        pFun->preTransformKoef[0][0]=1.0 - getRandomValue(0.0,KDEV);
        pFun->preTransformKoef[0][1]=0.0 + getRandomValue(-KDEV,KDEV);
        pFun->preTransformKoef[0][2]=0.0 + getRandomValue(-KDEV,KDEV);
        
        pFun->preTransformKoef[1][0]=0.0 + getRandomValue(-KDEV,KDEV);
        pFun->preTransformKoef[1][1]=1.0 - getRandomValue(0.0,KDEV);
        pFun->preTransformKoef[1][2]=0.0 + getRandomValue(-KDEV,KDEV);           


        
        pFun->postTransformKoef[0][0]=1.0 - getRandomValue(0.0,KDEV);
        pFun->postTransformKoef[0][1]=0.0 + getRandomValue(-KDEV,KDEV);
        pFun->postTransformKoef[0][2]=0.0 + getRandomValue(-KDEV,KDEV);
        
        pFun->postTransformKoef[1][0]=0.0 + getRandomValue(-KDEV,KDEV);
        pFun->postTransformKoef[1][1]=1.0 - getRandomValue(0.0,KDEV);
        pFun->postTransformKoef[1][2]=0.0 + getRandomValue(-KDEV,KDEV);           
                
        
        functions.push_back(pFun);
    }
    
    initFunctionProbabilities(functions,totalProbabilityWeight);
}
