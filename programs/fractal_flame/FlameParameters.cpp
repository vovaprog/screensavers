#include "FlameParameters.h"

#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
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

void FlameParameters::prepareLocale()
{
    setlocale(LC_NUMERIC,"C");
}

#ifndef NO_XML_FUNCTIONS

/*
<flames>
    <flame>
        <xform>
            <variation type="bubble" />
            <preTransformX x="1" y="1" c="1"/>
            <preTransformY x="1" y="1" c="1"/>
            <postTransformX x="1" y="1" c="1"/>
            <postTransformY x="1" y="1" c="1"/>
            <color r="255" g="255" b="255"/>
        </xform>
        <renderParameters xLowerBound="1" xUpperBound="1" yLowerBound="1" yUpperBound="1" setBoundsBy="x" setBoundsRatio="1" setBoundsCenter="1" />
    </flame>
</flames>
*/
void FlameParameters::save(const char *fileName)
{    
    prepareLocale();
    
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
						
		
		for(auto variation : xf->variations)
		{	
            TiXmlElement *varElement = new TiXmlElement( "variation" );		
            xformElement->LinkEndChild(varElement);
		    varElement->SetAttribute("type",variation.name);            
		}				

		
        TiXmlElement *transformElement = new TiXmlElement( "preTransformX" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->preTransformX_CoefX));
		transformElement->SetAttribute("y",to_string(xf->preTransformX_CoefY));
		transformElement->SetAttribute("c",to_string(xf->preTransformX_CoefC));

        transformElement = new TiXmlElement( "preTransformY" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->preTransformY_CoefX));
		transformElement->SetAttribute("y",to_string(xf->preTransformY_CoefY));
		transformElement->SetAttribute("c",to_string(xf->preTransformY_CoefC));
        
        transformElement = new TiXmlElement( "postTransformX" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->postTransformX_CoefX));
		transformElement->SetAttribute("y",to_string(xf->postTransformX_CoefY));
		transformElement->SetAttribute("c",to_string(xf->postTransformX_CoefC));
        
        transformElement = new TiXmlElement( "postTransformY" );		
        xformElement->LinkEndChild(transformElement);
		transformElement->SetAttribute("x",to_string(xf->postTransformY_CoefX));
		transformElement->SetAttribute("y",to_string(xf->postTransformY_CoefY));
		transformElement->SetAttribute("c",to_string(xf->postTransformY_CoefC));
		
		
		TiXmlElement *colorElement = new TiXmlElement( "color" );
		xformElement->LinkEndChild(colorElement);
		colorElement->SetAttribute("r",to_string(xf->r));
		colorElement->SetAttribute("g",to_string(xf->g));
		colorElement->SetAttribute("b",to_string(xf->b));				
	}
		
    TiXmlElement *renderElement = new TiXmlElement( "renderParameters" );		
    flameElement->LinkEndChild(renderElement);
    renderElement->SetAttribute("xLowerBound",to_string(xLowerBound));
    renderElement->SetAttribute("xUpperBound",to_string(xUpperBound));
    renderElement->SetAttribute("yLowerBound",to_string(yLowerBound));
    renderElement->SetAttribute("yUpperBound",to_string(yUpperBound));
    renderElement->SetAttribute("colorPower",to_string(colorPower));
            
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


#endif

bool FlameParameters::initRandomGenerator=true;

void FlameParameters::initRandom()
{
    resetVariables();
    
    if(initRandomGenerator)
    {
        srand(time(NULL));
        initRandomGenerator = false;
        cout <<"srand called"<<endl;
    }
    
    vector<Variation> &variations = getVariations();    
    
    int numberOfFunctions=MIN_NUMBER_OF_FUNCTIONS + rand() % (MAX_NUMBER_OF_FUNCTIONS + 1 - MIN_NUMBER_OF_FUNCTIONS);
    
    for(int i=0;i<numberOfFunctions;i++)
    {
        Function *pFun = new Function();
                
        int numberOfVariations = MIN_NUMBER_OF_VARIATIONS + rand() % (MAX_NUMBER_OF_VARIATIONS + 1 - MIN_NUMBER_OF_VARIATIONS);
        
        for(int j=0;j<numberOfVariations;j++)
        {        
            int variationIndex = rand() % variations.size();                    
            pFun->variations.push_back(variations[variationIndex]);
        }
        
                
        pFun->r = 150 + rand() % 106; 
        pFun->g = 150 + rand() % 106;
        pFun->b = 150 + rand() % 106;        
               
        
        const double KDEV = 0.5;        
        
        pFun->preTransformX_CoefX = 1.0 - getRandomValue(0.0,KDEV);
        pFun->preTransformX_CoefY = 0.0 + getRandomValue(-KDEV,KDEV);
        pFun->preTransformX_CoefC = 0.0 + getRandomValue(-KDEV,KDEV);
        
        pFun->preTransformY_CoefX = 0.0 + getRandomValue(-KDEV,KDEV);
        pFun->preTransformY_CoefY = 1.0 - getRandomValue(0.0,KDEV);
        pFun->preTransformY_CoefC = 0.0 + getRandomValue(-KDEV,KDEV);           

        
        pFun->postTransformX_CoefX = 1.0 - getRandomValue(0.0,KDEV);
        pFun->postTransformX_CoefY = 0.0 + getRandomValue(-KDEV,KDEV);
        pFun->postTransformX_CoefC = 0.0 + getRandomValue(-KDEV,KDEV);
        
        pFun->postTransformY_CoefX = 0.0 + getRandomValue(-KDEV,KDEV);
        pFun->postTransformY_CoefY = 1.0 - getRandomValue(0.0,KDEV);
        pFun->postTransformY_CoefC = 0.0 + getRandomValue(-KDEV,KDEV);           
                
        
        functions.push_back(unique_ptr<Function>(pFun));
    }
    
    initFunctionProbabilities();    
}


#ifndef NO_XML_FUNCTIONS

void FlameParameters::load(const char *fileName)
{
    resetVariables();
    
    prepareLocale();
    
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
                        else
                        {
                            throw string("unknown variation!");
                        }
                    }
                }
                else if(strcmp(xformChild->Value(),"preTransformX")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->preTransformX_CoefX));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->preTransformX_CoefY));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->preTransformX_CoefC));                
                }
                else if(strcmp(xformChild->Value(),"preTransformY")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->preTransformY_CoefX));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->preTransformY_CoefY));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->preTransformY_CoefC));                
                }
                else if(strcmp(xformChild->Value(),"postTransformX")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->postTransformX_CoefX));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->postTransformX_CoefY));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->postTransformX_CoefC));                
                }
                else if(strcmp(xformChild->Value(),"postTransformY")==0)
                {
                    sscanf(xformChild->Attribute("x"),"%lf",&(pFun->postTransformY_CoefX));
                    sscanf(xformChild->Attribute("y"),"%lf",&(pFun->postTransformY_CoefY));
                    sscanf(xformChild->Attribute("c"),"%lf",&(pFun->postTransformY_CoefC));                
                }
                else if(strcmp(xformChild->Value(),"color")==0)
                {
                    sscanf(xformChild->Attribute("r"),"%u",&(pFun->r));
                    sscanf(xformChild->Attribute("g"),"%u",&(pFun->g));
                    sscanf(xformChild->Attribute("b"),"%u",&(pFun->b));
                }
                
                xformChild=xformChild->NextSiblingElement();
            }
            
            functions.push_back(unique_ptr<Function>(pFun));
	    }
	    else if(strcmp(flameChild->Value(),"renderParameters")==0)
	    {
	        const char *attributeString;
	        
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
	        
            attributeString = flameChild->Attribute("setBoundsRatio");	    
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
                else if(strcmp(attributeString,"y")==0)
                {
                    setViewBoundsByY=true;
                }
            }
	    }
	    
	    flameChild=flameChild->NextSiblingElement();	    
	}
	
	initFunctionProbabilities();
}

void FlameParameters::load_old(const char *fileName)
{
    resetVariables();
    
    prepareLocale();
    
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
			&(pFun->preTransformX_CoefX),&(pFun->preTransformY_CoefX),
			&(pFun->preTransformX_CoefY),&(pFun->preTransformY_CoefY),
			&(pFun->preTransformX_CoefC),&(pFun->preTransformY_CoefC));
		
		sscanf(xformElem->Attribute("post"),"%lf %lf %lf %lf %lf %lf",
			&(pFun->postTransformX_CoefX),&(pFun->postTransformY_CoefX),
			&(pFun->postTransformX_CoefY),&(pFun->postTransformY_CoefY),
			&(pFun->postTransformX_CoefC),&(pFun->postTransformY_CoefC));
		
		for(auto v : getVariations())
		{
		    if(xformElem->Attribute(v.name))
		    {
		        pFun->variations.push_back(v);
		    }
		}

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

void FlameParameters::resetVariables()
{
    xLowerBound=-1.0;
    xUpperBound=1.0;
    yLowerBound=-1.0;
    yUpperBound=1.0;
    
    viewBoundsRatio=1.0;
    viewBoundsCenter=0.0;
    colorPower = 0.5;
    
    functions.clear();
    
    setViewBoundsByX=false;
    setViewBoundsByY=false;    
    totalProbabilityWeight=0;    
}

