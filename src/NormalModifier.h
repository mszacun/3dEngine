#ifndef NORMAL_MODIFIER
#define NORMAL_MODIFIER

#include "Vector.h"
#include <QImage>
#include <QColor>
#include <QPainter>

class NormalModifier
{
    public:
        virtual Vector ModifyNormal(const Vector& normal, const Vector& textureCoordinates) const = 0;
        virtual bool IsConstant() const = 0;
};

class NoNormalModifier : public NormalModifier
{
    public:
        virtual Vector ModifyNormal(const Vector& normal, const Vector& textureCoordinates) const override { return normal; }
        virtual bool IsConstant() const override { return true; }
};

class BumpMapNormalModifier : public NormalModifier
{
    public:
        BumpMapNormalModifier(const QImage& source);
        virtual Vector ModifyNormal(const Vector& normal, const Vector& textureCoordinates) const override;
        virtual bool IsConstant() const override { return false; }

    private:
        QImage Bu;
        QImage Bv;
};

#endif
