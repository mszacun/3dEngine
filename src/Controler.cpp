#include "Controler.h"

Controler::Controler(const Scene3D& scene) : scene_(scene), observerPosition_(0, 0, -10),
    rotationAngle(0)
{
    scene_.SetLightPosition(Vector(30, 50, -4));
    scene_.SetAmbientLightColor(QColor(0, 0, 0));
}

void Controler::SetView(ViewWeakPtr view)
{
    view_ = view;
}

QImage Controler::GetRenderedOrthogonalView()
{
    OrthogonalCamera cam(observerPosition_, Vector(0, 1, 0),
            Vector(0, 0, 0), 10, 1000, 400, 400);

    return scene_.RenderProjection(400, 400, cam);
}

QImage Controler::GetRenderedPerspectiveView()
{
    PerspectiveCamera cam(observerPosition_, Vector(0, 1, 0),
            Vector(0, 0, 0), 10, 1000, 0.78);

    QImage result = scene_.RenderProjection(400, 400, cam);

    return result;
}

void Controler::KeyPressed(int key)
{
    switch (key)
    {
        case Qt::Key_W: observerPosition_.SetZ(observerPosition_.GetZ() + 1); break;
        case Qt::Key_S: observerPosition_.SetZ(observerPosition_.GetZ() - 1); break;
        case Qt::Key_A: observerPosition_.SetX(observerPosition_.GetX() - 0.01); break;
        case Qt::Key_D: observerPosition_.SetX(observerPosition_.GetX() + 0.01); break;
    }
}

