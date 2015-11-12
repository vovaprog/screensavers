#include "FlameParameters.h"

#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <memory>
#include <locale.h>

#ifndef NO_XML_FUNCTIONS
#   include <tinyxml.h>
#endif

#include "Function.h"

using namespace std;

void FlameParameters::initFunctionProbabilities()
{
    totalProbabilityWeight=0;
    
    for(auto& funIter : functions)
    {
        totalProbabilityWeight += funIter->probabilityWeight;
        funIter->probabilityUpBorder=totalProbabilityWeight;
    }    
}

inline double FlameParameters::getRandom01()
{
    return ((double)(rand() % 1000))/1000.0;
}

inline double FlameParameters::getRandomValue(double start, double end)
{
    return start + getRandom01() * (end - start);
}

#ifndef NO_XML_FUNCTIONS

/*
<flames>
    <flame>
        <xform r="255" g="255" b="255">
            <variation type="bubble" />
            <preTransformX x="1" y="1" c="1"/>
            <preTransformY x="1" y="1" c="1"/>
            <postTransformX x="1" y="1" c="1"/>
            <postTransformY x="1" y="1" c="1"/>
        </xform>
        <renderParameters xLowerBound="1" xUpperBound="1" yLowerBound="1" yUpperBound="1" setBoundsBy="x" setBoundsRatio="1" setBoundsCenter="1" />
    </flame>
</flames>    
*/
void FlameParameters::save(const char *fileName)
{    
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	
	TiXmlElement *flamesElement = new TiXmlElement( "flames" );
	doc.LinkEndChild( flamesElement );
	
	TiXmlElement *flameElement = new TiXmlElement( "flame" );
	flamesElement->LinkEndChild(flameElement);

	for(auto& xf : functions)
	{
		TiXmlElement *xformElement = new TiXmlElement( "xform" );		
		flameElement->LinkEndChild(xformElement);
		
		
		xformElement->SetAttribute("r",to_string(xf->r));
		xformElement->SetAttribute("g",to_string(xf->g));
		xformElement->SetAttribute("b",to_string(xf->b));		
		
		
		for(auto variation : xf->variations)
		{	
            TiXmlElement *varElement = new TiXmlElement( "variation" );		
            xformElement->LinkEndChild(varElement);
		    varElement->SetAttribute("type",variation.name);            
		}				

		
        TiXmlElement *transformElement = new TiXmlElement( "preTransformX" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->preTransformKoef[0][0]));
		transformElement->SetAttribute("y",to_string(xf->preTransformKoef[0][1]));
		transformElement->SetAttribute("c",to_string(xf->preTransformKoef[0][2]));

        transformElement = new TiXmlElement( "preTransformY" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->preTransformKoef[1][0]));
		transformElement->SetAttribute("y",to_string(xf->preTransformKoef[1][1]));
		transformElement->SetAttribute("c",to_string(xf->preTransformKoef[1][2]));
        
        transformElement = new TiXmlElement( "postTransformX" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->postTransformKoef[0][0]));
		transformElement->SetAttribute("y",to_string(xf->postTransformKoef[0][1]));
		transformElement->SetAttribute("c",to_string(xf->postTransformKoef[0][2]));
        
        transformElement = new TiXmlElement( "postTransformX" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->postTransformKoef[1][0]));
		transformElement->SetAttribute("y",to_string(xf->postTransformKoef[1][1]));
		transformElement->SetAttribute("c",to_string(xf->postTransformKoef[1][2]));			
	}
		
    TiXmlElement *renderElement = new TiXmlElement( "renderParameters" );		
    flameElement->LinkEndChild(renderElement);
    renderElement->SetAttribute("xLowerBound",to_string(xLowerBound));
    renderElement->SetAttribute("xUpperBound",to_string(xUpperBound));
    renderElement->SetAttribute("yLowerBound",to_string(yLowerBound));
    renderElement->SetAttribute("yUpperBound",to_string(yUpperBound));
            
    if(setViewBoundsByX)
    {
        renderElement->SetAttribute("setBoundsBy","x");
    }
    else if(setViewBoundsByY)
    {
        renderElement->SetAttribute("setBoundsBy","y");
    }
        
    renderElement->SetAttribute("setBoundsRatio",to_string(viewBoundsRatio));
    renderElement->SetAttribute("setBoundsCenter",to_string(viewBoundsCenter));		
	
	
	doc.SaveFile( fileName );    
}

void FlameParameters::save_old(const char *fileName)
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
		
		/*for(auto variation : pFun->variations)
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
		}*/
			
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

void FlameParameters::initRandom()
{
    functions.clear();
    
    //vector<VariationPointer> variations;
    /*variations.push_back(variationSin);
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
    variations.push_back(variationTangent);*/
    //=======================================    
    //variations.push_back(variationNoise);
    //variations.push_back(variationBlur);
    //variations.push_back(variationGaussian);
    //=======================================
    //variations.push_back(variationExponential);//-    
    //variations.push_back(variationCosine);//-
    
    
    //if(firstTimeInitFunctionsRandom)
    //{        
        //firstTimeInitFunctionsRandom=false;
        //srand((unsigned int)time(NULL)); 
    //}

    vector<Variation> &variations = getVariations();
    
    
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
    
    initFunctionProbabilities();    
}


#ifndef NO_XML_FUNCTIONS

/*
<flames>
    <flame>
        <xform r="255" g="255" b="255">
            <variation type="bubble" />
            <preTransformX x="1" y="1" c="1"/>
            <preTransformY x="1" y="1" c="1"/>
            <postTransformX x="1" y="1" c="1"/>
            <postTransformY x="1" y="1" c="1"/>
        </xform>
        <renderParameters xLowerBound="1" xUpperBound="1" yLowerBound="1" yUpperBound="1"             
            colorPower="1" setBoundsBy="x" setBoundsRatio="1" setBoundsCenter="1" />
    </flame>
</flames>    
*/

void FlameParameters::load2(const char *fileName)
{
    setlocale(LC_NUMERIC,"C");
    
    functions.clear();
    
	TiXmlDocument doc(fileName);
	if (!doc.LoadFile()) 
	{
		throw string("can't open file!");
	}
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* flameChild;
	
	flameChild=hDoc.FirstChild("flames").FirstChild("flame").FirstChild().Element();

	while(flameChild!=nullptr)
	{	
	    if(strcmp(flameChild->Value(),"xform")==0)
	    {
	        Function *pFun=new Function();
	        
	        if(flameChild->Attribute("r"))
            {
                sscanf(flameChild->Attribute("r"),"%u",&(pFun->r));
            }
            else
            {
                pFun->r = 150 + rand() % 106;
            }
            
            if(flameChild->Attribute("g"))
            {
                sscanf(flameChild->Attribute("g"),"%u",&(pFun->g));
            }
            else
            {         
                pFun->g = 150 + rand() % 106;
            }
            
            if(flameChild->Attribute("b"))
            {
                sscanf(flameChild->Attribute("b"),"%u",&(pFun->b));
            }
            else
            {        
                pFun->b = 150 + rand() % 106;
            }		        
	        
            TiXmlElement* xformChild=flameChild->FirstChildElement();
            
            while(xformChild!=nullptr)
            {
                if(strcmp(xformChild->Value(),"variation")==0)
                {
                    const char* variationName = xformChild->Attribute("type");
                    if(variationName)
                    {
                        Variation v = getVariationByName(variationName);
                        if(!isNullVariation(v))
                        {
                            pFun->variations.push_back(v);
                        }
                    }
                }
                else if(strcmp(xformChild->Value(),"preTransformX")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->preTransformKoef[0][0]));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->preTransformKoef[0][1]));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->preTransformKoef[0][2]));                
                }
                else if(strcmp(xformChild->Value(),"preTransformY")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->preTransformKoef[1][0]));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->preTransformKoef[1][1]));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->preTransformKoef[1][2]));                
                }
                else if(strcmp(xformChild->Value(),"postTransformX")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->postTransformKoef[0][0]));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->postTransformKoef[0][1]));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->postTransformKoef[0][2]));                
                }
                else if(strcmp(xformChild->Value(),"postTransformY")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->postTransformKoef[1][0]));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->postTransformKoef[1][1]));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->postTransformKoef[1][2]));                
                }
                
                xformChild=xformChild->NextSiblingElement();
            }            
	    }
	    else if(strcmp(flameChild->Value(),"renderParameters")==0)
	    {
	        attributeString = flameChild->Attribute("xLowerBound");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&xLowerBound);
            }
    
            attributeString = flameChild->Attribute("xUpperBound");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&xUpperBound);
            }
            
            attributeString = flameChild->Attribute("yLowerBound");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&yLowerBound);
            }
    
            attributeString = flameChild->Attribute("yUpperBound");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&yUpperBound);
            }
            
            attributeString = flameChild->Attribute("colorPower");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&colorPower);
            }
	        
            attributeString = flameChild->Attribute("setBoundsRation");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&viewBoundsRatio);
            }	        

            attributeString = flameChild->Attribute("setBoundsCenter");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&viewBoundsCenter);
            }	        

            setViewBoundsByX=false;
            setViewBoundsByY=false;
            
            attributeString = flameChild->Attribute("setBoundsBy");	    
            if(attributeString)
            {
                if(strcmp(attributeString,"x")==0)
                {
                    setViewBoundsByX=true;
                }
                else if(strcmp(attributeString,"x")==0)
                {
                    setViewBoundsByY=true;
                }
            }
	    }
	    
	    flameChild=flameChild->NextSiblingElement();	    
	}
}

void FlameParameters::load(const char *fileName)
{
    setlocale(LC_NUMERIC,"C");
    
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
		
		for(auto v : getVariations())
		{
		    if(xformElem->Attribute(v.name))
		    {
		        pFun->variations.push_back(v);
		    }
		}
		
		/*if(xformElem->Attribute("sinusoidal")) pFun->variations.push_back(getVariationByName("sinusoidal"));
		if(xformElem->Attribute("eyefish")) pFun->variations.push_back(getVariationByName("eyefish"));
		if(xformElem->Attribute("spherical")) pFun->variations.push_back(getVariationByName("spherical"));
		if(xformElem->Attribute("swirl")) pFun->variations.push_back(getVariationByName("swirl"));
		if(xformElem->Attribute("horseshoe")) pFun->variations.push_back(getVariationByName("horseshoe"));
		if(xformElem->Attribute("polar")) pFun->variations.push_back(getVariationByName("polar"));
		if(xformElem->Attribute("handkerchief")) pFun->variations.push_back(getVariationByName("handkerchief"));
		if(xformElem->Attribute("heart")) pFun->variations.push_back(getVariationByName("heart"));
		if(xformElem->Attribute("disk")) pFun->variations.push_back(getVariationByName("disk"));
		if(xformElem->Attribute("spiral")) pFun->variations.push_back(getVariationByName("spiral"));
		if(xformElem->Attribute("hyperbolic")) pFun->variations.push_back(getVariationByName("hyperbolic"));
		if(xformElem->Attribute("diamond")) pFun->variations.push_back(getVariationByName("diamond"));
		if(xformElem->Attribute("julia")) pFun->variations.push_back(getVariationByName("julia"));
		if(xformElem->Attribute("ex")) pFun->variations.push_back(getVariationByName("ex"));
		if(xformElem->Attribute("bent")) pFun->variations.push_back(getVariationByName("bent"));
		if(xformElem->Attribute("mirror")) pFun->variations.push_back(getVariationByName(variationMirror);
		if(xformElem->Attribute("power")) pFun->variations.push_back(getVariationByName(variationPower);
		if(xformElem->Attribute("bubble")) pFun->variations.push_back(getVariationByName(variationBubble);
		if(xformElem->Attribute("cylinder")) pFun->variations.push_back(getVariationByName(variationCylinder);
		if(xformElem->Attribute("tangent")) pFun->variations.push_back(getVariationByName(variationTangent);
		if(xformElem->Attribute("noise")) pFun->variations.push_back(getVariationByName(variationNoise);
		if(xformElem->Attribute("blur")) pFun->variations.push_back(getVariationByName(variationBlur);
		if(xformElem->Attribute("gaussian")) pFun->variations.push_back(getVariationByName(variationGaussian);
		if(xformElem->Attribute("exponential")) pFun->variations.push_back(getVariationByName(variationExponential);
		if(xformElem->Attribute("cosine")) pFun->variations.push_back(getVariationByName(variationCosine);*/

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
	
	initFunctionProbabilities();
	
	TiXmlElement* paramsElem=hDoc.FirstChild("Flames").FirstChild("flame").FirstChild("renderParameters").Element();
		
	if(paramsElem)
	{
	    const char *attributeString;
	    
	    attributeString = paramsElem->Attribute("xLowerBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&xLowerBound);
		}

	    attributeString = paramsElem->Attribute("xUpperBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&xUpperBound);
		}
		
	    attributeString = paramsElem->Attribute("yLowerBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&yLowerBound);
		}

	    attributeString = paramsElem->Attribute("yUpperBound");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&yUpperBound);
		}
		
	    attributeString = paramsElem->Attribute("colorPower");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&colorPower);
		}
		
		
		setViewBoundsByX = false;
		setViewBoundsByY = false;

	    attributeString = paramsElem->Attribute("xViewBoundsRatio");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&viewBoundsRatio);
		    
            attributeString = paramsElem->Attribute("xViewBoundsCenter");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&viewBoundsCenter);
            }		    
		    
            setViewBoundsByX = true;
		}

	    attributeString = paramsElem->Attribute("yViewBoundsRatio");	    
	    if(attributeString)
		{
		    sscanf(attributeString,"%lf",&viewBoundsRatio);
		    
            attributeString = paramsElem->Attribute("yViewBoundsCenter");	    
            if(attributeString)
            {
                sscanf(attributeString,"%lf",&viewBoundsCenter);
            }		    
		    
            setViewBoundsByY = true;
		}
	}
}

#endif

void FlameParameters::setViewBoundsForPictureSize(int pictureWidth, int pictureHeight)
{
    if(setViewBoundsByX)
    {
        double xSize = viewBoundsRatio * (yUpperBound-yLowerBound)*((double)pictureWidth / (double)pictureHeight);
        xLowerBound=viewBoundsCenter - (xSize/2.0);
        xUpperBound=viewBoundsCenter + (xSize/2.0);            
    }
    else if(setViewBoundsByY)
    {
        double ySize = viewBoundsRatio * (xUpperBound-xLowerBound)*((double)pictureHeight / (double)pictureWidth);
        yLowerBound=viewBoundsCenter - (ySize/2.0);
        yUpperBound=viewBoundsCenter + (ySize/2.0);            
    }
}

void FlameParameters::prepare()
{
    initFunctionProbabilities();    
}

