#include <ColorMap.h>

#include <cstdlib>
#include <ctime>

using namespace std;

#define NumberOfColorVariants 8

#define RED_MASK   0x0000ff
#define GREEN_MASK 0x00ff00
#define BLUE_MASK  0xff0000

#define RED_SHIFT 0
#define GREEN_SHIFT 8
#define BLUE_SHIFT 16

struct RgbEntry
{
    float r, g, b;
};

static RgbEntry ColorVariants[NumberOfColorVariants];

bool ColorMapData::InitColorVariantsExecuted = false;

ColorMapData::~ColorMapData()
{
    if (Points != 0)
    {
        delete[] Points;
    }
    if (Map != 0)
    {
        delete[] Map;
    }
}


void ColorMapData::SetNextPointTarget(int PointIndex)
{
    ColorMapPoint& p = Points[PointIndex];

    int NextColorVariantIndex;

    while (true)
    {
        if (UseBlackColor)
        {
            NextColorVariantIndex = rand() % NumberOfColorVariants;
        }
        else
        {
            NextColorVariantIndex = 1 + rand() % (NumberOfColorVariants - 1);
        }

        if (p.CurrentColorVariantIndex != NextColorVariantIndex)
        {
            if (!CheckMixedColorVariants || PointIndex == 0 || NextColorVariantIndex != Points[PointIndex - 1].CurrentColorVariantIndex)
            {
                p.CurrentColorVariantIndex = NextColorVariantIndex;
                break;
            }
        }
    }

    p.dr = (ColorVariants[NextColorVariantIndex].r - p.r) / (float)ChangeMapSteps;
    p.dg = (ColorVariants[NextColorVariantIndex].g - p.g) / (float)ChangeMapSteps;
    p.db = (ColorVariants[NextColorVariantIndex].b - p.b) / (float)ChangeMapSteps;
}

void ColorMapData::SetNextPointTargets()
{
    for (int i = 0; i < NumberOfPoints; i++)
    {
        SetNextPointTarget(i);
    }
}

void ColorMapData::PointColorsStep()
{
    for (int i = 0; i < NumberOfPoints; i++)
    {
        ColorMapPoint& p = Points[i];

        p.r += p.dr;
        if (p.r > 255)
        {
            p.r = 255;
        }
        else if (p.r < 0)
        {
            p.r = 0;
        }

        p.g += p.dg;
        if (p.g > 255)
        {
            p.g = 255;
        }
        else if (p.g < 0)
        {
            p.g = 0;
        }

        p.b += p.db;
        if (p.b > 255)
        {
            p.b = 255;
        }
        else if (p.b < 0)
        {
            p.b = 0;
        }
    }
}


void ColorMapData::CreateColorMap()
{
    float r , g , b ;

    for (int i = 0; i < NumberOfPoints - 1; i++)
    {
        ColorMapPoint& pStart = Points[i];
        ColorMapPoint& pEnd = Points[i + 1];

        r = pStart.r;
        g = pStart.g;
        b = pStart.b;

        float dr = (pEnd.r - pStart.r) / (pEnd.Index - pStart.Index);
        float dg = (pEnd.g - pStart.g) / (pEnd.Index - pStart.Index);
        float db = (pEnd.b - pStart.b) / (pEnd.Index - pStart.Index);

        for (int j = pStart.Index; j < pEnd.Index; j++)
        {
            Map[j] = (
                         (((unsigned int)r) << RED_SHIFT) |
                         (((unsigned int)g) << GREEN_SHIFT) |
                         (((unsigned int)b) << BLUE_SHIFT));

            r += dr;
            g += dg;
            b += db;
        }
    }
}

void ColorMapData::InitColorVariants()
{
    InitColorVariantsExecuted = true;

    srand(time(NULL));

    for (int i = 0; i < 8; i++)
    {
        if ((i & 1) != 0)
        {
            ColorVariants[i].r = 0xff;
        }
        else
        {
            ColorVariants[i].r = 0x0;
        }

        if ((i & 2) != 0)
        {
            ColorVariants[i].g = 0xff;
        }
        else
        {
            ColorVariants[i].g = 0x0;
        }

        if ((i & 4) != 0)
        {
            ColorVariants[i].b = 0xff;
        }
        else
        {
            ColorVariants[i].b = 0x0;
        }
    }
}

void ColorMapData::InitColorPoints()
{
    int ColorMapIndex = 0;
    int ColorMapIndexD = ColorMapSize / (NumberOfPoints - 1);

    for (int i = 0; i < NumberOfPoints; i++)
    {
        int ColorIndex = rand() % NumberOfColorVariants;

        ColorMapPoint& p = Points[i];
        RgbEntry& Clr = ColorVariants[ColorIndex];

        p.r = Clr.r;
        p.g = Clr.g;
        p.b = Clr.b;
        p.CurrentColorVariantIndex = ColorIndex;

        if (i < NumberOfPoints - 1)
        {
            p.Index = ColorMapIndex;
        }
        else
        {
            p.Index = ColorMapSize;
        }

        ColorMapIndex += ColorMapIndexD;
    }
}

void ColorMapData::ColorMapStep()
{
    ++CurrentChangeMapStep;

    if (CurrentChangeMapStep < ChangeMapSteps)
    {
        PointColorsStep();
    }
    else
    {
        SetNextPointTargets();
        CurrentChangeMapStep = 0;
    }

    CreateColorMap();
}

unsigned int* ColorMapData::GetColorMap()
{
    return Map;
}

void ColorMapData::InitColorMap(int ColorMapSizeParameter, int NumberOfPointsParameter, int ChangeMapStepsParameter)
{
    if (!InitColorVariantsExecuted)
    {
        InitColorVariants();
    }

    ColorMapSize = ColorMapSizeParameter;
    Map = new unsigned int[ColorMapSize];

    NumberOfPoints = NumberOfPointsParameter;
    Points = new ColorMapPoint[NumberOfPointsParameter];

    ChangeMapSteps = ChangeMapStepsParameter;
    CurrentChangeMapStep = 0;

    InitColorPoints();

    SetNextPointTargets();
    ColorMapStep();
}

void ColorMapData::InitColorMap(int ColorMapSizeParameter, vector<ColorMapPoint> &PointParameters)
{
    InitColorMap(ColorMapSizeParameter, PointParameters, 0);
}

void ColorMapData::InitColorMap(int ColorMapSizeParameter, vector<ColorMapPoint> &PointParameters, int ChangeMapStepsParameter)
{
    if (!InitColorVariantsExecuted)
    {
        InitColorVariants();
    }

    ColorMapSize = ColorMapSizeParameter;
    Map = new unsigned int[ColorMapSize];

    NumberOfPoints = PointParameters.size();
    Points = new ColorMapPoint[NumberOfPoints];

    for (int i = 0; i < NumberOfPoints; i++)
    {
        Points[i] = PointParameters[i];
    }

    ChangeMapSteps = ChangeMapStepsParameter;
    CurrentChangeMapStep = 0;

    CreateColorMap();
    SetNextPointTargets();
}

void ColorMapPoint::SetColor(unsigned int Color)
{
    r = (Color & RED_MASK) >> RED_SHIFT;
    g = (Color & GREEN_MASK) >> GREEN_SHIFT;
    b = (Color & BLUE_MASK) >> BLUE_SHIFT;
}

