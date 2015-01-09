#include "FlatShader.h"
#include <iostream>

QColor operator*(const QColor& left, double right)
{
    return QColor(std::max(left.red() * right, 0.0), std::max(0.0, left.green() * right), std::max(0.0, left.blue() * right));
}

QColor operator+(const QColor& left, const QColor& right)
{
    return QColor(std::min(left.red() + right.red(), 255), std::min(left.green() + right.green(), 255), std::min(left.blue() + right.blue(), 255));
}

Shader::Shader(const TriangleShadingInfo& shadingInfo): shadingInfo_(shadingInfo)
{
    shadingInfo_.lightColor.getRgb(lightRgb, lightRgb +1, lightRgb + 2, lightRgb + 3);
    shadingInfo_.ambientLightColor.getRgb(ambientLightRgb, ambientLightRgb +1, ambientLightRgb + 2, ambientLightRgb + 3);
}

QColor Shader::CalculatePhongModel(const Vector& point, const Vector& lightVector,
       const Vector& normal) const
{
    int result[4];

    // difuse
    double cos = normal.Dot(lightVector);
    if (cos <= 0.0)
        return QColor(0, 0, 0);

    // specular
    Vector toObserverVector = (shadingInfo_.observatorPosition - point).Normalize();
    Vector reflectionVector = (normal * 2 * normal.Dot(toObserverVector) - toObserverVector);
    double dot = lightVector.Dot(reflectionVector);
    double exponent = std::pow(dot, shadingInfo_.material.GetShiness());

    for (int i = 0; i < 3; i++)
    {
        double diffuse = lightRgb[i] * cos * shadingInfo_.material.GetDiffuse(i, 0, 0, 0);
        double specular = shadingInfo_.material.GetSpecular(i, 0, 0, 0) * lightRgb[i] * exponent;
        double own = shadingInfo_.material.GetOwnLigth(i, 0, 0, 0);
        result[i] = diffuse + specular + own + ambientLightRgb[i];
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
    p1Color_ = CalculatePhongModel(shadingInfo_.p1,
            (shadingInfo_.lightPosition - shadingInfo_.p1).Normalize(), shadingInfo_.p1Normal);            
    p2Color_ = CalculatePhongModel(shadingInfo_.p2,
            (shadingInfo_.lightPosition - shadingInfo_.p2).Normalize(), shadingInfo_.p2Normal);            
    p3Color_ = CalculatePhongModel(shadingInfo_.p3,
            (shadingInfo_.lightPosition - shadingInfo_.p3).Normalize(), shadingInfo_.p3Normal);            

    interpolator.SetVector1(Vector((int) shadingInfo_.projectedP1.GetX(), (int) shadingInfo_.projectedP1.GetY(), 1), p1Color_);
    interpolator.SetVector2(Vector((int) shadingInfo_.projectedP2.GetX(), (int) shadingInfo_.projectedP2.GetY(), 1), p2Color_);
    interpolator.SetVector3(Vector((int) shadingInfo_.projectedP3.GetX(), (int) shadingInfo_.projectedP3.GetY(), 1), p3Color_);
}

QColor GouraudShader::GetColorForPixel(const Vector& pixel) const
{
    return interpolator.Interpolate(Vector(pixel.GetX(), pixel.GetY(), 1));
}

