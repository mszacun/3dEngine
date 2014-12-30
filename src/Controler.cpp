#include "Controler.h"

Controler::Controler(const Scene3D& scene) : scene_(scene)
{
    scene_.SetObserverPosition(Point(0, 0, -7));
}

void Controler::SetView(ViewWeakPtr view)
{
    view_ = view;
}

Scene2D Controler::GetPerspectiveProjection() const
{
    return scene_.GetPerspectiveProjection();
}

QImage Controler::GetRenderedPerspectiveView() const
{
    return scene_.RenederPerspectiveProjection();
}
