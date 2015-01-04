#include "FlatShader.h"
#include <iostream>

Shader::Shader(const TriangleShadingInfo& shadingInfo): shadingInfo_(shadingInfo)
{
}

double fattr(double r)
{
    return 1;
}

QColor Shader::CalculatePhongModel(const Vector& point, const Vector& lightVector,
       const Vector& normal) const
{
    int lightRgb[4];
    int ambientLightRgb[4];
    int result[4];

    shadingInfo_.lightColor.getRgb(lightRgb, lightRgb +1, lightRgb + 2, lightRgb + 3);
    shadingInfo_.ambientLightColor.getRgb(ambientLightRgb, ambientLightRgb +1, ambientLightRgb + 2, ambientLightRgb + 3);

    // difuse
    double cos = std::max(0.0, normal.Dot(lightVector_));
    double lightVectorLen = lightVector.Length();

    // specular
    Vector toObserverVector = shadingInfo_.observatorPosition - point;
    Vector reflectionVector = (normal * 2 * normal.Dot(toObserverVector) - toObserverVector).Normalize();
    double dot = lightVector_.Dot(reflectionVector);
    double exponent = std::pow(dot, shadingInfo_.material.GetShiness());

    for (int i = 0; i < 3; i++)
    {
        double diffuse = fattr(lightVectorLen) * lightRgb[i] * cos * shadingInfo_.material.GetDiffuse(i, 0, 0, 0);
        double specular = shadingInfo_.material.GetSpecular(i, 0, 0, 0) * fattr(lightVectorLen) * lightRgb[i] * exponent;
        double own = shadingInfo_.material.GetOwnLigth(i, 0, 0, 0);
        result[i] = diffuse + specular + own + ambientLightRgb[i];;
    }

    return QColor(std::min(result[0], 255), std::min(result[1], 255), std::min(result[2], 255));
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

    calculatedColor_ =  CalculatePhongModel(triangleCenter, lightVector_, shadingInfo_.triangleNormal);
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
