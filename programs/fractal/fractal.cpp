#include <cmath>
#include <ctime>
#include <atomic>

#include <omp.h>

#include <vectorf128.h>

#include <ColorMap.h>
#include <MovingValue.h>

using namespace std;

static int pictureWidth,pictureHeight;

static unsigned int *output=NULL;

static Vec4f *imList=NULL;
static Vec4f *reList=NULL; 
static Vec4f *reBubbleList=NULL;

static int iterLimit;

static Vec4f vCheckDistQuadrat;
static Vec4f v1(1);
static Vec4f v2(2);
static Vec4f vkoef_re;
static Vec4f vkoef_im;

static float lowBoundRe, highBoundRe;
static float lowBoundIm, highBoundIm;

static ColorMapData clrMapData;
static unsigned int *colorMap;

static MovingValue movingHighBound;
static MovingValue koefReD;
static MovingValue koefImD;
static MovingValue bubbleImPhaseMovingValue,bubbleRePhaseMovingValue;

static float bubbleImPhase=0.0, bubbleRePhase=0.0;

static int numberOfProcessors = 2;


static void *allocAligned16(size_t size)
{	
#ifdef __linux__
	
	void *p;	

	if(posix_memalign(&p,16,size)==0)
	{
		return p;
	}
	else
	{
		return 0;	
	}
	
#else

	uintptr_t p = (uintptr_t)malloc(size + 16);
	p = p + 16 - (p & 15);
	return (void*)p;
	
#endif	
}

static inline float calcBubbleIm(float x)
{
	return x+(sin(x*3.0+bubbleImPhase)/9.0) * x;
}

static inline float calcBubbleRe(float x)
{
	return sin(x*3.0+bubbleRePhase)/9.0;
}

static inline void iterFunJuliaSimd(
	Vec4f &vre,
	Vec4f &vim,	
	float *ResultIters
	)
{
	Vec4f viter(0);

	int IterCount = 0;

	while (++IterCount<iterLimit)
	{
		Vec4f vrevre=vre*vre;
		Vec4f vimvim=vim*vim;

		Vec4fb CompareResult = (vrevre + vimvim < vCheckDistQuadrat);

		if (!horizontal_or(CompareResult))
		{
			break;
		}

		viter = if_add(CompareResult, viter, v1);

		Vec4f temp_vre = vrevre - vimvim + vkoef_re;
		vim = v2*vre*vim + vkoef_im;

		vre = temp_vre;
	}

	viter.store(ResultIters);

	return;
}


static void calculateFractal()
{			
	//#pragma omp parallel sections
	{
		//#pragma omp section
		{
			float dIm = (highBoundIm - lowBoundIm) / pictureHeight;
			float im=highBoundIm;
				
			for(int i=0;i<pictureHeight / 2;i++)
			{	
				float imBubble=calcBubbleIm(im);	
				imList[i]=Vec4f(imBubble,imBubble,imBubble,imBubble);
				im-=dIm;
			}
		}
		
		//#pragma omp section
		{
			float dRe = (highBoundRe - lowBoundRe) / pictureWidth;
			float re=lowBoundRe;
			
			for(int i=0;i<pictureWidth/4;i++)
			{
				float re1, re2, re3;
				re1=re+dRe; re2=re1+dRe; re3=re2+dRe;
				reBubbleList[i]=Vec4f(calcBubbleRe(re),calcBubbleRe(re1),calcBubbleRe(re2),calcBubbleRe(re3));
				
				reList[i]=Vec4f(re,re1,re2,re3);
				re = re3 + dRe;
			}
		}
	}
	
	int NumberOfThreads = numberOfProcessors + 2;
	
#pragma omp parallel for num_threads(NumberOfThreads)	
	for(int offset=0;offset<NumberOfThreads;offset++)
	{	
		for(int i=offset;i<pictureHeight / 2;i+=NumberOfThreads)
		{
			for(int j=0;j<pictureWidth/4;j++)
			{
				float ResultIters[4];
				
				Vec4f vim=imList[i];
				Vec4f vre=reList[j] + reBubbleList[j] * vim;
				
				iterFunJuliaSimd(vre,vim,ResultIters);
				
				for (int q = 0; q < 4; q++)
				{
					int iter = (int)ResultIters[q];
					iter = colorMap[iter];
					output[i*pictureWidth+j*4+q] = iter;
					output[pictureWidth*pictureHeight - i*pictureWidth-j*4-q - 1] = iter;
				}				
			}
		}
	}
	
	/*atomic_int index(0);	
	int picWidthDiv4 = pictureWidth / 4;
	int picWidthMulPicHeight = pictureWidth*pictureHeight;
		
#pragma omp parallel for num_threads(NumberOfThreads)	
	for(int threadIndex=0;threadIndex<NumberOfThreads;++threadIndex)
	{	
	    while(true)
	    {
            int ind = index.fetch_add(1, memory_order_relaxed);
            
            if(ind >= (pictureHeight / 2) * picWidthDiv4)
            {
                break;
            }
            
            int i = ind / picWidthDiv4;
            int j = ind % picWidthDiv4;
            
            float ResultIters[4];
            
            Vec4f vim=imList[i];
            Vec4f vre=reList[j] + reBubbleList[j] * vim;
            
            iterFunJuliaSimd(vre,vim,ResultIters);
            
            for (int q = 0; q < 4; q++)
            {
                int iter = (int)ResultIters[q];
                iter = colorMap[iter];
                output[i*pictureWidth+j*4+q] = iter;
                output[picWidthMulPicHeight - i*pictureWidth-j*4-q - 1] = iter;
            }				
        }
	}	*/
}

unsigned int* fractalStep()
{	
	highBoundRe=movingHighBound.step();
	lowBoundRe=-highBoundRe;
	highBoundIm=highBoundRe;
	lowBoundIm=-highBoundIm;
	
	
	bubbleImPhase=bubbleImPhaseMovingValue.step();
	bubbleRePhase=bubbleRePhaseMovingValue.step();
	
	
	clrMapData.ColorMapStep();
	
	
	vkoef_re = Vec4f(koefReD.step());
	vkoef_im = Vec4f(koefImD.step());

	calculateFractal();
	
	return output;
}

static void allocateBuffers()
{
	output=new unsigned int[pictureWidth*pictureHeight];
	
	imList=(Vec4f*)allocAligned16(sizeof(Vec4f) * pictureHeight / 2);
	reList=(Vec4f*)allocAligned16(sizeof(Vec4f) * pictureWidth / 4); 
	reBubbleList=(Vec4f*)allocAligned16(sizeof(Vec4f) * pictureWidth / 4);	
}



void fractalInit(int argPictureWidth, int argPictureHeight)
{
	numberOfProcessors = omp_get_num_procs();
	
	
	pictureWidth=argPictureWidth;
	pictureHeight=argPictureHeight;
	
	allocateBuffers();
	
	
	lowBoundRe = -1.5;
	highBoundRe = 1.5;

	lowBoundIm = -1.5;
	highBoundIm = 1.5;

	vCheckDistQuadrat = Vec4f(350.0*350.0);

	vector<ColorMapPoint> Points;
	ColorMapPoint p;


	iterLimit = 160;

	
	p.Index = 0;
	p.SetColor(0x00000000);	
	Points.push_back(p);	

	p.Index = 40;
	p.SetColor(0x00ff00);
	Points.push_back(p);

	p.Index = 80;
	p.SetColor(0x00ffff);
	Points.push_back(p);

	p.Index = iterLimit;
	p.SetColor(0xffffff);
	Points.push_back(p);
	
	clrMapData.InitColorMap(iterLimit, Points, 150);
	colorMap = clrMapData.GetColorMap();


	/*
	parameters for MovingValue constructor:
		normalDValue,
		dIncValue,
		downBreakBound,
		upBreakBound,
		initialDValue
		initialControlledValue
	*/
	
	srand(time(NULL));

	bubbleImPhaseMovingValue=MovingValue(0.02,0.001,0.0,12.0,0.02,0.0);
	bubbleRePhaseMovingValue=MovingValue(0.015,0.001,0.0,12.0,0.015,0.0);

	
	movingHighBound=MovingValue(0.005,0.00008,0.4,1.4,-0.003,1.5);

	koefReD=MovingValue(0.000113,0.00000323,-0.80,-0.79,0.000113,-0.8);
	
	koefImD=MovingValue(0.000099,0.00000245,0.1535,0.158,0.000099,0.156);
}

