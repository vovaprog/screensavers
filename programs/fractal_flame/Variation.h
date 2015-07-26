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
//=================================================================
void variationPower(double x, double y, double &xOut, double &yOut);
void variationBubble(double x, double y, double &xOut, double &yOut);
void variationCylinder(double x, double y, double &xOut, double &yOut);
void variationTangent(double x, double y, double &xOut, double &yOut);
//=================================================================
void variationExponential(double x, double y, double &xOut, double &yOut);//-
void variationCosine(double x, double y, double &xOut, double &yOut);//-
void variationNoise(double x, double y, double &xOut, double &yOut);//-
void variationBlur(double x, double y, double &xOut, double &yOut);//-
void variationGaussian(double x, double y, double &xOut, double &yOut);//-


