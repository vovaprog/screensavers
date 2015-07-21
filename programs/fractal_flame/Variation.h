#pragma once

typedef void (*VariationPointer)(double x, double y, double &xOut, double &yOut);

void variationSin(double x, double y, double &xOut, double &yOut);
void variationFisheye(double x, double y, double &xOut, double &yOut);
void variationMirror(double x, double y, double &xOut, double &yOut);
void variationSpherical(double x, double y, double &xOut, double &yOut);
void variationSwirl(double x, double y, double &xOut, double &yOut);
void variationHorseshoe(double x, double y, double &xOut, double &yOut);
void variationPolar(double x, double y, double &xOut, double &yOut);
void variationHandkerchief(double x, double y, double &xOut, double &yOut);
void variationHeart(double x, double y, double &xOut, double &yOut);
void variationDisk(double x, double y, double &xOut, double &yOut);
void variationSpiral(double x, double y, double &xOut, double &yOut);
void variationHyperbolic(double x, double y, double &xOut, double &yOut);
void variationDiamond(double x, double y, double &xOut, double &yOut);
void variationJulia(double x, double y, double &xOut, double &yOut);
void variationEx(double x, double y, double &xOut, double &yOut);
void variationBent(double x, double y, double &xOut, double &yOut);

