#pragma once

#include <vector>

using namespace std;

struct ColorMapPoint
{
    float r, g, b;
    int Index;
    float dr, dg, db;
    int CurrentColorVariantIndex;

    void SetColor(unsigned int Color);
};


class ColorMapData
{
private:
    ColorMapPoint *Points;
    unsigned int *Map;

    int ColorMapSize;
    int NumberOfPoints;

    int ChangeMapSteps;
    int CurrentChangeMapStep;

    bool CheckMixedColorVariants;
    bool UseBlackColor;

public:
    ColorMapData()
    {
        Points = 0;
        Map = 0;
        CurrentChangeMapStep = 0;
        CheckMixedColorVariants = false;
        UseBlackColor = true;
    }

    ~ColorMapData();

    void InitColorMap(int ColorMapSizeParameter, int NumberOfPointsParameter, int ChangeMapSteps);

    void InitColorMap(int ColorMapSizeParameter, vector<ColorMapPoint> &PointParameters);

    void InitColorMap(int ColorMapSizeParameter, vector<ColorMapPoint> &PointParameters, int ChangeMapStepsParameter);

    unsigned int* GetColorMap();

    void ColorMapStep();

    void SetCheckMixedColorVariants(bool check)
    {
        CheckMixedColorVariants = check;
    }

    void SetUseBlackColor(bool use)
    {
        UseBlackColor = use;
    }

private:
    static bool InitColorVariantsExecuted;
    static void InitColorVariants();

    void InitColorPoints();
    void CreateColorMap();
    void PointColorsStep();
    void SetNextPointTargets();
    void SetNextPointTarget(int PointIndex);

};