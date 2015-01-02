#include "Controler.h"

Controler::Controler(const Scene3D& scene) : scene_(scene)
{
    scene_.SetObserverPosition(Point(0, 0, -10));
    scene_.SetLightPosition(Point(30, 50, 0));
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
    scene_.Transform(Matrix::CreateXAxisRotationMatrix(1));
    scene_.Transform(Matrix::CreateYAxisRotationMatrix(1));
    return scene_.RenederPerspectiveProjection(160, 100);
}
