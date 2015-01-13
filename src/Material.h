#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <QImage>
#include "BilinearInterpolator.h"
#include "Vector.h"

enum RGBComponent { RED, GREEN, BLUE, ALPHA };

int ClipRGB(int color);

class Material
{
    public:
        // 0 <= GetDiffuse() <= 1
        virtual double GetDiffuse(int component, const Vector& point) const { return component == RED ? 1 : 0.2; }
        // The shininess specifies how small the highlights are: the shinier, the smaller the highlights.
        virtual double GetShiness() const { return 20; };
        // 0 <= GetSpecular() <= 1
        virtual double GetSpecular(int component, const Vector& point) const { return 1; }
        virtual int GetOwnLigth(int component, const Vector& point) const { return 0; }
        virtual bool IsConstant() const { return true; }

    private:
};

class ImageSource : public Source<QColor>
{
    public:
        ImageSource(const QImage& source) : source_(source) {}
        
        QColor GetValue(int x, int y) const override;

        int GetWidth() const { return source_.width(); }
        int GetHeight() const { return source_.height(); }

    private:
        QImage source_;

};

class ImageTextureMaterial : public Material
{
    public:
        ImageTextureMaterial(const QImage& image);

        virtual double GetDiffuse(int component, const Vector& point) const override;
        bool IsConstant() const override { return false; }

    private:
        ImageSource source_;
        BilinearInterpolator<QColor> interpolator_;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif
