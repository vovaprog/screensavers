#include <vector>
#include <cstdlib>

#include <tinyxml.h>

#include "Function.h"
#include "init_functions.h"

using namespace std;

static void initFunctionProbabilities(vector<Function*> &functions, int &totalProbabilityWeight)
{
    totalProbabilityWeight=0;
    
    for(auto funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }    
}


static const int MIN_NUMBER_OF_FUNCTIONS = 2;
static const int MAX_NUMBER_OF_FUNCTIONS = 3;
static const int MIN_NUMBER_OF_VARIATIONS = 1;
static const int MAX_NUMBER_OF_VARIATIONS = 3;


static inline double getRandom01()
{
    return ((double)(rand() % 1000))/1000.0;
}

static inline double getRandomValue(double start, double end)
{
    return start + getRandom01() * (end - start);
}


void saveFunctions(const char *fileName,vector<Function*> &functions)
{
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	
	TiXmlElement *flamesElement = new TiXmlElement( "Flames" );
	doc.LinkEndChild( flamesElement );
	
	TiXmlElement *flameElement = new TiXmlElement( "flame" );
	flamesElement->LinkEndChild(flameElement);

	for(auto pFun : functions)
	{
		TiXmlElement *xformElement = new TiXmlElement( "xform" );		
		flameElement->LinkEndChild(xformElement);
		
		for(auto variation : pFun->variations)
		{		
			if(variation == variationSin) xformElement->SetAttribute("sinusoidal","1.0");
			else if(variation == variationFisheye) xformElement->SetAttribute("eyefish","1.0");
			else if(variation == variationSpherical) xformElement->SetAttribute("spherical","1.0");
			else if(variation == variationSwirl) xformElement->SetAttribute("swirl","1.0");						
			else if(variation == variationHorseshoe) xformElement->SetAttribute("horseshoe","1.0");
			else if(variation == variationPolar) xformElement->SetAttribute("polar","1.0");
			else if(variation == variationHandkerchief) xformElement->SetAttribute("handkerchief","1.0");
			else if(variation == variationHeart) xformElement->SetAttribute("heart","1.0");
			else if(variation == variationDisk) xformElement->SetAttribute("disk","1.0");
			else if(variation == variationSpiral) xformElement->SetAttribute("spiral","1.0");
			else if(variation == variationHyperbolic) xformElement->SetAttribute("hyperbolic","1.0");
			else if(variation == variationDiamond) xformElement->SetAttribute("diamond","1.0");
			else if(variation == variationJulia) xformElement->SetAttribute("julia","1.0");
			else if(variation == variationEx) xformElement->SetAttribute("ex","1.0");
			else if(variation == variationBent) xformElement->SetAttribute("bent","1.0");
			else if(variation == variationMirror) xformElement->SetAttribute("mirror","1.0");
			else
			{
				throw string("unknown variation!");
			}
		}
			
		string coefs = to_string(pFun->preTransformKoef[0][0])+" "+to_string(pFun->preTransformKoef[1][0])+" "+
			to_string(pFun->preTransformKoef[0][1])+" "+to_string(pFun->preTransformKoef[1][1])+" "+
			to_string(pFun->preTransformKoef[0][2])+" "+to_string(pFun->preTransformKoef[1][2]);
			
		xformElement->SetAttribute("coefs",coefs);

		string post = to_string(pFun->postTransformKoef[0][0])+" "+to_string(pFun->postTransformKoef[1][0])+" "+
			to_string(pFun->postTransformKoef[0][1])+" "+to_string(pFun->postTransformKoef[1][1])+" "+
			to_string(pFun->postTransformKoef[0][2])+" "+to_string(pFun->postTransformKoef[1][2]);
		
		xformElement->SetAttribute("post",post);				
	}
	
	doc.SaveFile( fileName );    	
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

void loadFunctions(const char *fileName,vector<Function*> &functions, int &totalProbabilityWeight)
{
	TiXmlDocument doc(fileName);
	if (!doc.LoadFile()) 
	{
		throw string("can't open file!");
	}
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* xformElem;
	
	xformElem=hDoc.FirstChild("Flames").FirstChild("flame"	).FirstChild("xform").Element();

	while(xformElem)
	{
		Function *pFun=new Function();
		functions.push_back(pFun);
		
		sscanf(xformElem->Attribute("coefs"),"%lf %lf %lf %lf %lf %lf",
			&(pFun->preTransformKoef[0][0]),&(pFun->preTransformKoef[1][0]),
			&(pFun->preTransformKoef[0][1]),&(pFun->preTransformKoef[1][1]),
			&(pFun->preTransformKoef[0][2]),&(pFun->preTransformKoef[1][2]));
		
		sscanf(xformElem->Attribute("post"),"%lf %lf %lf %lf %lf %lf",
			&(pFun->postTransformKoef[0][0]),&(pFun->postTransformKoef[1][0]),
			&(pFun->postTransformKoef[0][1]),&(pFun->postTransformKoef[1][1]),
			&(pFun->postTransformKoef[0][2]),&(pFun->postTransformKoef[1][2]));
		    
		if(xformElem->Attribute("sinusoidal")) pFun->variations.push_back(variationSin);
		if(xformElem->Attribute("eyefish")) pFun->variations.push_back(variationFisheye);
		if(xformElem->Attribute("spherical")) pFun->variations.push_back(variationSpherical);
		if(xformElem->Attribute("swirl")) pFun->variations.push_back(variationSwirl);
		if(xformElem->Attribute("horseshoe")) pFun->variations.push_back(variationHorseshoe);
		if(xformElem->Attribute("polar")) pFun->variations.push_back(variationPolar);
		if(xformElem->Attribute("handkerchief")) pFun->variations.push_back(variationHandkerchief);
		if(xformElem->Attribute("heart")) pFun->variations.push_back(variationHeart);
		if(xformElem->Attribute("disk")) pFun->variations.push_back(variationDisk);
		if(xformElem->Attribute("spiral")) pFun->variations.push_back(variationSpiral);
		if(xformElem->Attribute("hyperbolic")) pFun->variations.push_back(variationHyperbolic);
		if(xformElem->Attribute("diamond")) pFun->variations.push_back(variationDiamond);
		if(xformElem->Attribute("julia")) pFun->variations.push_back(variationJulia);
		if(xformElem->Attribute("ex")) pFun->variations.push_back(variationEx);
		if(xformElem->Attribute("bent")) pFun->variations.push_back(variationBent);
		if(xformElem->Attribute("mirror")) pFun->variations.push_back(variationMirror);

        pFun->r = 200 + rand() % 56; 
        pFun->g = 200 + rand() % 56;
        pFun->b = 200 + rand() % 56;        
		
		
		xformElem=xformElem->NextSiblingElement();
	}
	
	initFunctionProbabilities(functions,totalProbabilityWeight);
}

