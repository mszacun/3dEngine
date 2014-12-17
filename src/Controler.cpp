#include "Controler.h"

Controler::Controler(const Scene3D& scene) : view_(nullptr), scene_(scene)
{
        scene_.SetObserverPosition(Point(0, 0, -7));
}

void Controler::SetView(ViewPtr view)
{
    view_ = view;
}

Scene2D Controler::GetPerspectiveProjection() const
{
        return scene_.GetPerspectiveProjection();
}

