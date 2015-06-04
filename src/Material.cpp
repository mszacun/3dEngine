#include "Material.h"

int ClipRGB(int color) { return std::max(std::min(color, 255),0); }

QColor ImageSource::GetValue(int x, int y) const
{
    x = std::max(0, std::min(source_.width() - 1, x));
    y = std::max(0, std::min(source_.height() - 1, y));

    return source_.pixel(x, y);
}

ImageTextureMaterial::ImageTextureMaterial(const QImage& image) : source_(image)
{
}

double ImageTextureMaterial::GetDiffuse(int component, const Vector& point) const
{
    double x = point.GetX() * source_.GetWidth();
    double y = point.GetY() * source_.GetHeight();

    QColor color = interpolator_.Interpolate(source_, x, y);
    int rgb[4];
    color.getRgb(rgb, rgb + 1, rgb + 2, rgb + 3);

    return rgb[component] / 255.0;
}
