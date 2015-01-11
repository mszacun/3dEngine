#include "Controler.h"

int SCREEN_WIDTH = 500;
int SCREEN_HEIGHT = 500;
int ORTHOGONAL_CAMERA_WIDTH = 5;
int ORTHOGONAL_CAMERA_HEIGHT = 5;

Controler::Controler(const Scene3D& scene) : scene_(scene), observerPosition_(0, 0, -10),
    rotationAngle(0), perspectiveCamera_(observerPosition_, Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, 0.78)
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
            Vector(0, 0, 0), 3, 20, ORTHOGONAL_CAMERA_WIDTH, ORTHOGONAL_CAMERA_HEIGHT);

    return scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam, observerPosition_);
}

OrthogonalProjection Controler::GetSideView()
{
    OrthogonalCamera cam(Vector(-5, 0, 0), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, ORTHOGONAL_CAMERA_WIDTH, ORTHOGONAL_CAMERA_HEIGHT);

    return scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam, observerPosition_);
}

OrthogonalProjection Controler::GetTopView()
{
    OrthogonalCamera cam(Vector(0, 5, 0), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, ORTHOGONAL_CAMERA_WIDTH, ORTHOGONAL_CAMERA_HEIGHT);

    return scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam, observerPosition_);
}

QImage Controler::GetRenderedPerspectiveView()
{
    QImage result = scene_.RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH,
            perspectiveCamera_);

    return result;
}

void Controler::MoveCamera(const Vector& moveVector)
{
    observerPosition_ = observerPosition_ + moveVector;
    perspectiveCamera_.position = observerPosition_;
}

void Controler::KeyPressed(int key)
{
    switch (key)
    {
        case Qt::Key_W: MoveCamera(Vector(0, 0, 0.1)); break;
        case Qt::Key_S: MoveCamera(Vector(0, 0, -0.1)); break;
        case Qt::Key_A: MoveCamera(Vector(-0.1, 0, 0)); break;
        case Qt::Key_D: MoveCamera(Vector(0.1, 0, 0)); break;
        case Qt::Key_Down: MoveCamera(Vector(0, -0.1, 0)); break;
        case Qt::Key_Up: MoveCamera(Vector(0, 0.1, 0)); break;
    }
}

