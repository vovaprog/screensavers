#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <memory>

#ifndef NO_XML_FUNCTIONS
#   include <tinyxml.h>
#endif

#include "Function.h"
#include "FractalFlame.h"

using namespace std;

void FractalFlame::initFunctionProbabilities(vector<unique_ptr<Function>> &functions, int &totalProbabilityWeight)
{
    totalProbabilityWeight=0;
    
    for(auto& funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }    
}


inline double FractalFlame::getRandom01()
{
    return ((double)(rand() % 1000))/1000.0;
}

inline double FractalFlame::getRandomValue(double start, double end)
{
    return start + getRandom01() * (end - start);
}

#ifndef NO_XML_FUNCTIONS

void FractalFlame::saveFunctions(const char *fileName,vector<unique_ptr<Function>> &functions)
{    
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	
	TiXmlElement *flamesElement = new TiXmlElement( "Flames" );
	doc.LinkEndChild( flamesElement );
	
	TiXmlElement *flameElement = new TiXmlElement( "flame" );
	flamesElement->LinkEndChild(flameElement);

	for(auto& pFun : functions)
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
			else if(variation == variationPower) xformElement->SetAttribute("power","1.0");
			else if(variation == variationBubble) xformElement->SetAttribute("bubble","1.0");
			else if(variation == variationCylinder) xformElement->SetAttribute("cylinder","1.0");
			else if(variation == variationTangent) xformElement->SetAttribute("tangent","1.0");
			else if(variation == variationNoise) xformElement->SetAttribute("noise","1.0");
			else if(variation == variationBlur) xformElement->SetAttribute("blur","1.0");
			else if(variation == variationGaussian) xformElement->SetAttribute("gaussian","1.0");
			else if(variation == variationExponential) xformElement->SetAttribute("exponential","1.0");
			else if(variation == variationCosine) xformElement->SetAttribute("cosine","1.0");
			else
			{
				throw string("unknown variation!");
			}
		}
			
		string coefs = to_string(pFun->preTransformKoef[0][0])+" "+to_string(pFun->preTransformKoef[1][0])+" "+
			to_string(pFun->preTransformKoef[0][1])+" "+to_string(pFun->preTransformKoef[1][1])+" "+
			to_string(pFun->preTransformKoef[0][2])+" "+to_string(pFun->preTransformKoef[1][2]);
			
		xformElement->SetAttribute("coefs",coefs.c_str());

		string post = to_string(pFun->postTransformKoef[0][0])+" "+to_string(pFun->postTransformKoef[1][0])+" "+
			to_string(pFun->postTransformKoef[0][1])+" "+to_string(pFun->postTransformKoef[1][1])+" "+
			to_string(pFun->postTransformKoef[0][2])+" "+to_string(pFun->postTransformKoef[1][2]);
		
		xformElement->SetAttribute("post",post.c_str());		
		
		xformElement->SetAttribute("r",to_string(pFun->r));
		xformElement->SetAttribute("g",to_string(pFun->g));
		xformElement->SetAttribute("b",to_string(pFun->b));
	}
	
	doc.SaveFile( fileName );    
}

#endif

void FractalFlame::initFunctionsRandom(vector<unique_ptr<Function>> &functions, int &totalProbabilityWeight)
{
    functions.clear();
    
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
    variations.push_back(variationMirror);
    //=======================================
    variations.push_back(variationPower);
    variations.push_back(variationBubble);
    variations.push_back(variationCylinder);
    variations.push_back(variationTangent);
    //=======================================    
    //variations.push_back(variationNoise);
    //variations.push_back(variationBlur);
    //variations.push_back(variationGaussian);
    //=======================================
    //variations.push_back(variationExponential);//-    
    //variations.push_back(variationCosine);//-
    
    
    if(firstTimeInitFunctionsRandom)
    {        
        firstTimeInitFunctionsRandom=false;
        srand((unsigned int)time(NULL)); 
    }

    
    int numberOfFunctions=MIN_NUMBER_OF_FUNCTIONS + rand() % (MAX_NUMBER_OF_FUNCTIONS + 1 - MIN_NUMBER_OF_FUNCTIONS);
    cout <<"------------"<<endl;
    for(int i=0;i<numberOfFunctions;i++)
    {
        Function *pFun = new Function();
                
        int numberOfVariations=MIN_NUMBER_OF_VARIATIONS + rand() % (MAX_NUMBER_OF_VARIATIONS + 1 - MIN_NUMBER_OF_VARIATIONS);
        
        for(int j=0;j<numberOfVariations;j++)
        {        
            int variationIndex = rand() % variations.size();                    
            pFun->variations.push_back(variations[variationIndex]);
            cout <<variationIndex<<"   ";
        }
        cout<<endl;
        
                
        pFun->r = 150 + rand() % 106; 
        pFun->g = 150 + rand() % 106;
        pFun->b = 150 + rand() % 106;        
               
        
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
                
        
        functions.push_back(unique_ptr<Function>(pFun));
    }
    cout <<"------------"<<endl;
    
    initFunctionProbabilities(functions,totalProbabilityWeight);    
}


#ifndef NO_XML_FUNCTIONS

void FractalFlame::loadFunctions(const char *fileName,vector<unique_ptr<Function>> &functions, int &totalProbabilityWeight)
{
    functions.clear();
    
	TiXmlDocument doc(fileName);
	if (!doc.LoadFile()) 
	{
		throw string("can't open file!");
	}
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* xformElem;
	
	xformElem=hDoc.FirstChild("Flames").FirstChild("flame").FirstChild("xform").Element();

	while(xformElem && strcmp(xformElem->Value(),"xform")==0)
	{
		Function *pFun=new Function();		
		
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
		if(xformElem->Attribute("power")) pFun->variations.push_back(variationPower);
		if(xformElem->Attribute("bubble")) pFun->variations.push_back(variationBubble);
		if(xformElem->Attribute("cylinder")) pFun->variations.push_back(variationCylinder);
		if(xformElem->Attribute("tangent")) pFun->variations.push_back(variationTangent);
		if(xformElem->Attribute("noise")) pFun->variations.push_back(variationNoise);
		if(xformElem->Attribute("blur")) pFun->variations.push_back(variationBlur);
		if(xformElem->Attribute("gaussian")) pFun->variations.push_back(variationGaussian);
		if(xformElem->Attribute("exponential")) pFun->variations.push_back(variationExponential);
		if(xformElem->Attribute("cosine")) pFun->variations.push_back(variationCosine);

		if(xformElem->Attribute("r"))
		{
		    sscanf(xformElem->Attribute("r"),"%u",&(pFun->r));
		}
		else
		{
		    pFun->r = 150 + rand() % 106;
		}
		
		if(xformElem->Attribute("g"))
		{
		    sscanf(xformElem->Attribute("g"),"%u",&(pFun->g));
		}
		else
		{         
		    pFun->g = 150 + rand() % 106;
        }
        
		if(xformElem->Attribute("b"))
		{
		    sscanf(xformElem->Attribute("b"),"%u",&(pFun->b));
		}
		else
		{        
		    pFun->b = 150 + rand() % 106;
		}						
		
		xformElem=xformElem->NextSiblingElement();
				
		functions.push_back(unique_ptr<Function>(pFun));
	}
	
	initFunctionProbabilities(functions,totalProbabilityWeight);
	
	TiXmlElement* paramsElem=hDoc.FirstChild("Flames").FirstChild("flame").FirstChild("renderParameters").Element();
		
	if(paramsElem)
	{
	    const char *attributeString;
	    
	    attributeString = xformElem->Attribute("xLowerBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&xLowerBound);
		}

	    attributeString = xformElem->Attribute("xUpperBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&xUpperBound);
		}
		
	    attributeString = xformElem->Attribute("yLowerBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&yLowerBound);
		}

	    attributeString = xformElem->Attribute("yUpperBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&yUpperBound);
		}				
	}
}

#endif

