#include "NormalModifier.h"

// thanks to
// http://www.ics.uci.edu/~majumder/VC/classes/BEmap.pdf

QColor SubColors(const QColor& left, const QColor& right)
{
    return QColor(
            std::max(0, left.red() - right.red()), 
            std::max(0, left.blue() - right.blue()), 
            std::max(0, left.green() - right.green()));
}

BumpMapNormalModifier::BumpMapNormalModifier(const QImage& source) :
    Bu(source.width(), source.height(), QImage::Format_ARGB32),
    Bv(source.width(), source.height(), QImage::Format_ARGB32)
{
    QPainter buPainter(&Bu);
    QPainter bvPainter(&Bv);

    for (int y = 0; y < source.height(); y++)
        for (int x = 0; x < source.width(); x++)
        {
            int x1 = std::min(x + 1, source.width() - 1);
            int y1 = std::min(y + 1, source.height() - 1);

            buPainter.setPen(SubColors(source.pixel(x1, y), source.pixel(x, y)));
            buPainter.drawPoint(x, y);

            bvPainter.setPen(SubColors(source.pixel(x, y1), source.pixel(x, y)));
            bvPainter.drawPoint(x, y);
        }
}

Vector BumpMapNormalModifier::ModifyNormal(const Vector& normal,
        const Vector& textureCoordinates) const
{
    Vector t1 = Vector(2, 1, (-normal.GetX() * 2 - normal.GetY()) / normal.GetZ()).Normalize();
    Vector t2 = normal.Cross(t1).Normalize();

    int x = std::max(0, std::min(Bu.width() - 1, (int) (textureCoordinates.GetX() * Bu.width())));
    int y = std::max(0, std::min(Bu.height() - 1, (int) (textureCoordinates.GetY() * Bu.height())));
    QColor pixel = Bu.pixel(x, y);

    double t1Factor = (double) (pixel.red()) * 20;
    double t2Factor = (double) (QColor(Bv.pixel(x, y)).red()) * 20;

    Vector result = (normal + t1 * t1Factor + t2 * t2Factor).Normalize();
    return result;
}
