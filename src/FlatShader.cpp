#include "FlatShader.h"
#include <iostream>

Shader::Shader(const TriangleShadingInfo& shadingInfo): shadingInfo_(shadingInfo)
{
}

FlatShader::FlatShader(const TriangleShadingInfo& shadingInfo) : 
    Shader(shadingInfo)
{
    double xCenter = (shadingInfo_.p1.GetX() + shadingInfo_.p2.GetX() + shadingInfo_.p3.GetX()) / 3;
    double yCenter = (shadingInfo_.p1.GetY() + shadingInfo_.p2.GetY() + shadingInfo_.p3.GetY()) / 3;
    double zCenter = (shadingInfo_.p1.GetZ() + shadingInfo_.p2.GetZ() + shadingInfo_.p3.GetZ()) / 3;
    Vector triangleCenter = Vector(xCenter, yCenter, zCenter);
    lightVector_  = shadingInfo_.lightPosition - triangleCenter;
    lightVector_ = lightVector_.Normalize();

    int numberOfColors = 3;
    int lightRgb[4];
    char names[] = {'R', 'G', 'B', 'A'};

    shadingInfo_.lightColor.getRgb(lightRgb, lightRgb +1, lightRgb + 2, lightRgb + 3);

    double cos = std::max(0.0, shadingInfo_.triangleNormal.Dot(lightVector_));
    for (int i = 0; i < 3; i++)
        lightRgb[i] *= cos * shadingInfo_.material.GetDiffuse(i, 0, 0, 0);

    calculatedColor_ =  QColor(lightRgb[0], lightRgb[1], lightRgb[2], lightRgb[3]);
}

QColor FlatShader::GetColorForPixel(const Vector& pixel) const
{
    return calculatedColor_;
}

GouraudShader::GouraudShader(const TriangleShadingInfo& shadingInfo) : 
    Shader(shadingInfo)
{
}

QColor GouraudShader::GetColorForPixel(const Vector& pixel) const
{
    return QColor("pink");
}
