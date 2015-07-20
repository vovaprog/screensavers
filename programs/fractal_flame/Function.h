#pragma once

#include <vector>

#include <cstdlib>

#include <cmath>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#include <tinyxml.h>

using namespace std;

typedef void (*VariationPointer)(double x, double y, double &xOut, double &yOut);



//=================================================================

inline double radius(double x, double y)
{
    return sqrt(x*x + y*y);
}

inline double theta(double x, double y)
{
    return atan(x/y);    
}

inline double fi(double x, double y)
{
    return atan(y/x);    
}

inline double omega()
{
    if(rand() % 2==0)
    {
        return M_PI;    
    }
    else
    {
        return 0.0;    
    }
}

//=================================================================

inline void variationSin(double x, double y, double &xOut, double &yOut) //+
{
    xOut=sin(x);
    yOut=sin(y);
}    

inline void variationFisheye(double x, double y, double &xOut, double &yOut)
{        
    xOut = (2.0 / (radius(x,y) +1)) * y; 
    yOut = (2.0 / (radius(x,y) +1)) * x;
}

inline void variationMirror(double x, double y, double &xOut, double &yOut)
{        
    xOut = -x;
    yOut = -y;
}

inline void variationSpherical(double x, double y, double &xOut, double &yOut) //+
{       
    double r = radius(x,y);
    
    xOut=(1.0/(r*r)) * x;
    yOut=(1.0/(r*r)) * y;        
}

inline void variationSwirl(double x, double y, double &xOut, double &yOut) //+
{       
    double r = radius(x,y);
    double r2 = r*r;
    
    xOut=x*sin(r2)-y*cos(r2);
    yOut=x*cos(r2)+y*sin(r2);        
}

inline void variationHorseshoe(double x, double y, double &xOut, double &yOut)
{       
    double r = radius(x,y);
    
    xOut=(1.0/r)*(x-y)*(x+y);
    yOut=(1.0/r)*2.0*x*y;        
}

inline void variationPolar(double x, double y, double &xOut, double &yOut)
{       
    xOut = theta(x,y) / M_PI;
    yOut = radius(x,y)-1.0;
}

inline void variationHandkerchief(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = r*sin(th+r);
    yOut = r*cos(th-r);
}

inline void variationHeart(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = r*sin(th*r);
    yOut = r*(-cos(th*r));
}

inline void variationDisk(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = (th/M_PI) * sin(M_PI*r);
    yOut = (th/M_PI) * cos(M_PI*r);
}

inline void variationSpiral(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = (1.0/r) * (cos(th)+sin(r));
    yOut = (1.0/r) * (sin(th)-cos(r));
}

inline void variationHyperbolic(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = sin(th)/r;
    yOut = r*cos(th);
}

inline void variationDiamond(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    xOut = sin(th)*cos(r);
    yOut = cos(th)*sin(r);
}

inline void variationJulia(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    double om=omega();
    
    xOut = sqrt(r)*cos(th/2+om);
    yOut = sqrt(r)*sin(th/2+om);
}

inline void variationEx(double x, double y, double &xOut, double &yOut)
{       
    double r=radius(x,y);
    double th=theta(x,y);
    
    double p0=sin(th+r);
    double p1=cos(th-r);
    
    double p0_3=p0*p0*p0;
    double p1_3=p1*p1*p1;
    
    xOut=r*(p0_3+p1_3);
    yOut=r*(p0_3-p1_3);
}

inline void variationBent(double x, double y, double &xOut, double &yOut)
{       
    if(x>=0 && y>=0)
    {
        xOut=x;
        yOut=y;            
    }
    else if(x<0 && y>=0)
    {
        xOut=2*x;
        yOut=y;
    }
    else if(x>=0 && y<0)
    {
        xOut=x;
        yOut=y/2;
    }
    else
    {
        xOut=2*x;
        yOut=y/2;
    }
}

//====================================================================================

class Function{
public:        
    int probabilityWeight;
    int probabilityUpBorder;
    
    unsigned int r,g,b;
    
    double preTransformKoef[2][3];
    double postTransformKoef[2][3];

    vector<VariationPointer> variations;
    
    Function():probabilityWeight(1.0),r(255),g(255),b(255)
    {
        preTransformKoef[0][0]=1.0;
        preTransformKoef[0][1]=0.0;
        preTransformKoef[0][2]=0.0;
        
        preTransformKoef[1][0]=0.0;
        preTransformKoef[1][1]=1.0;
        preTransformKoef[1][2]=0.0;        
        
        
        postTransformKoef[0][0]=1.0;
        postTransformKoef[0][1]=0.0;
        postTransformKoef[0][2]=0.0;
        
        postTransformKoef[1][0]=0.0;
        postTransformKoef[1][1]=1.0;
        postTransformKoef[1][2]=0.0;        
    }
    
    void save(const char *fileName)
    {
    	TiXmlDocument doc;
		TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
		doc.LinkEndChild( decl );
		
		TiXmlElement *flamesElement = new TiXmlElement( "Flames" );
		doc.LinkEndChild( flamesElement );
		
		TiXmlElement *flameElement = new TiXmlElement( "flame" );
		flamesElement->LinkEndChild(flameElement);

		for(auto variation : variations)
		{
			TiXmlElement *xformElement = new TiXmlElement( "xform" );		
			flameElement->LinkEndChild(xformElement);
		
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
	
			string coefs = to_string(preTransformKoef[0][0])+" "+to_string(preTransformKoef[1][0])+" "+
				to_string(preTransformKoef[0][1])+" "+to_string(preTransformKoef[1][1])+" "+
				to_string(preTransformKoef[0][2])+" "+to_string(preTransformKoef[1][2]);
				
			xformElement->SetAttribute("coefs",coefs);

			string post = to_string(postTransformKoef[0][0])+" "+to_string(postTransformKoef[1][0])+" "+
				to_string(postTransformKoef[0][1])+" "+to_string(postTransformKoef[1][1])+" "+
				to_string(postTransformKoef[0][2])+" "+to_string(postTransformKoef[1][2]);
			
			xformElement->SetAttribute("post",post);				
		}
		
		doc.SaveFile( fileName );    	
    }
    
    void load(const char *fileName)
    {
    	TiXmlDocument doc(fileName);
    	if (!doc.LoadFile()) 
    	{
    		throw string("can't open file!");
    	}
    	
    	TiXmlHandle hDoc(&doc);
    	TiXmlElement* xformElem;
    	
    	xformElem=hDoc.FirstChild("Flames").FirstChild("flame"	).FirstChild("xform").Element();

    	cout << "+++"<<xformElem->Attribute("coefs")<<endl;    	
    	
    	sscanf(xformElem->Attribute("coefs"),"%lf %lf %lf %lf %lf %lf",
    		&(preTransformKoef[0][0]),&(preTransformKoef[1][0]),
    		&(preTransformKoef[0][1]),&(preTransformKoef[1][1]),
    		&(preTransformKoef[0][2]),&(preTransformKoef[1][2]));

    	cout << "---"<<preTransformKoef[0][0]<<preTransformKoef[1][0]<<preTransformKoef[0][1]<<preTransformKoef[1][1]<<endl;
    	
    	sscanf(xformElem->Attribute("post"),"%lf %lf %lf %lf %lf %lf",
    		&(postTransformKoef[0][0]),&(postTransformKoef[1][0]),
    		&(postTransformKoef[0][1]),&(postTransformKoef[1][1]),
    		&(postTransformKoef[0][2]),&(postTransformKoef[1][2]));

    	

    	
    	
    	
    	
    }
};