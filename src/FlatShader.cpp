#include "FlatShader.h"
#include <iostream>

FlatShader::FlatShader(const TriangleShadingInfo& shadingInfo) : 
    shadingInfo_(shadingInfo)
{
    lightVector_  = shadingInfo_.lightPosition - shadingInfo_.p1;
    lightVector_ = lightVector_.Normalize();
}

QColor FlatShader::GetColorForPixel(const Vector& pixel) const
{
    int numberOfColors = 3;
    int lightRgb[4];
    char names[] = {'R', 'G', 'B', 'A'};

    shadingInfo_.lightColor.getRgb(lightRgb, lightRgb +1, lightRgb + 2, lightRgb + 3);

    for (int i = 0; i < 3; i++)
    {
        double cos = std::max(0.0, shadingInfo_.triangleNormal.Dot(lightVector_));
        double calculatedComponent = lightRgb[i] * cos * shadingInfo_.material.GetDiffuse(i, 0, 0, 0);
        std::cout << "Cos: " << cos << std::endl;
        std::cout << names[i] << ": " << lightRgb[i] << " -> " << calculatedComponent <<std::endl;
        lightRgb[i] *= cos * shadingInfo_.material.GetDiffuse(i, 0, 0, 0);
    }

    return QColor(lightRgb[0], lightRgb[1], lightRgb[2], lightRgb[3]);
}
