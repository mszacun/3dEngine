#include "Controler.h"

int SCREEN_WIDTH = 500;
int SCREEN_HEIGHT = 500;

Controler::Controler(const Scene3D& scene) : scene_(scene), observerPosition_(0, 0, -10),
    rotationAngle(0)
{
    scene_.SetLightPosition(Vector(30, -50, -4));
    scene_.SetAmbientLightColor(QColor(0, 0, 0));
}

void Controler::SetView(ViewWeakPtr view)
{
    view_ = view;
}

OrthogonalProjection Controler::GetFrontView()
{
    OrthogonalCamera cam(Vector(0, 0, -5), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, 5, 5);

    return scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam, observerPosition_);
}

OrthogonalProjection Controler::GetSideView()
{
    OrthogonalCamera cam(Vector(-5, 0, 0), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, 5, 5);

    return scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam, observerPosition_);
}

OrthogonalProjection Controler::GetTopView()
{
    OrthogonalCamera cam(Vector(0, 5, 0), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, 5, 5);

    return scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam, observerPosition_);
}

QImage Controler::GetRenderedPerspectiveView()
{
    PerspectiveCamera cam(observerPosition_, Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, 0.78);

    QImage result = scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam);

    return result;
}

void Controler::MoveCamera(const Vector& moveVector)
{
    observerPosition_ = observerPosition_ + moveVector;
}

void Controler::KeyPressed(int key)
{
    switch (key)
    {
        case Qt::Key_W: observerPosition_.SetZ(observerPosition_.GetZ() + 0.1); break;
        case Qt::Key_S: observerPosition_.SetZ(observerPosition_.GetZ() - 0.1); break;
        case Qt::Key_A: observerPosition_.SetX(observerPosition_.GetX() - 0.1); break;
        case Qt::Key_D: observerPosition_.SetX(observerPosition_.GetX() + 0.1); break;
        case Qt::Key_Down: observerPosition_.SetY(observerPosition_.GetY() - 0.1); break;
        case Qt::Key_Up: observerPosition_.SetY(observerPosition_.GetY() + 0.1); break;
    }
}

