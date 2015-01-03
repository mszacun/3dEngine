#include "Controler.h"

Controler::Controler(const Scene3D& scene) : scene_(scene), observerPosition_(0, 0, -10),
    rotationAngle(0)
{
    scene_.SetObserverPosition(observerPosition_);
    scene_.SetLightPosition(Vector(30, 50, 0));
}

void Controler::SetView(ViewWeakPtr view)
{
    view_ = view;
}

Scene2D Controler::GetPerspectiveProjection() const
{
    return scene_.GetPerspectiveProjection();
}

QImage Controler::GetRenderedPerspectiveView()
{
    QImage result = scene_.RenederPerspectiveProjection(400, 400);
    /*Vector cubeCenter(220, 205, 2);

    rotationAngle += M_PI / 180;
    Matrix transformationMatrix = Matrix::CreateYAxisRotationMatrix(rotationAngle);
    Vector newObP = observerPosition_.Transform(transformationMatrix);
    scene_.SetObserverPosition(newObP);

    std::cout << "Observer position: " << newObP << std::endl;
    std::cout << "Angle: " << rotationAngle << std::endl;*/

    return result;
}
