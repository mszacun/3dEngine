#ifndef BILINEAR_INTERPOLATOR_H
#define BILINEAR_INTERPOLATOR_H

#include <QColor>

template <typename T>
class Source
{
    public: 
        int GetWidth() const { return 30; }
        int GetHeight() const { return 30; }
        virtual QColor GetValue(int x, int y) const { return QColor("pink"); }
};

template <typename T>
class BilinearInterpolator
{
    public:
        T Interpolate(const Source<T>& source, double x, double y);

};

template <typename T>
T BilinearInterpolator<T>::Interpolate(const Source<T>& source, double x, double y)
{
    int Ai = (int)x;
    int Aj = (int)y;
    double alfa = y - Aj;
    double beta = x - Ai;
    double Xa, Xb, Xc, Xd;
    int X;
    int shifted;

    int aColor[4], bColor[4], cColor[4], dColor[4], result[4];

    source.GetValue(Ai, Aj).getRgb(aColor, aColor + 1, aColor + 2, aColor + 3);
    source.GetValue(Ai, Aj + 1).getRgb(bColor, bColor + 1, bColor + 2, bColor + 3);
    source.GetValue(Ai + 1, Aj).getRgb(cColor, cColor + 1, cColor + 2, cColor + 3);
    source.GetValue(Ai + 1, Aj + 1).getRgb(dColor, dColor + 1, dColor + 2, dColor + 3);

    int colorARGB = 0;

    for (int i = 0; i < 4; i++ )
    {
       Xa = (1 - alfa) * aColor[i];
       Xb = alfa * bColor[i];
       Xc = (1 - alfa) * cColor[i];
       Xd = (alfa) * dColor[i];
       X = (int)(beta * (Xc + Xd) + (1 - beta) * (Xa + Xb));
       result[i] = X;
    }

    return QColor(result[0], result[1], result[2], result[3]);
}

#endif
