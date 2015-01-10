#ifndef CONTROLER_H
#define CONTROLER_H

#include <memory>

#include "Scene.h"

class View;
typedef std::weak_ptr<View> ViewWeakPtr;

class Controler
{
    public:
        Controler(const Scene3D& scene);
        void SetView(ViewWeakPtr view);

        OrthogonalProjection GetFrontView();
        OrthogonalProjection GetSideView();
        OrthogonalProjection GetTopView();
        QImage GetRenderedPerspectiveView();

        void MoveCamera(const Vector& moveVector);

        void KeyPressed(int key);

    private:
        ViewWeakPtr view_;
        Scene3D scene_;

        Vector observerPosition_;
        double rotationAngle;
};

#endif

