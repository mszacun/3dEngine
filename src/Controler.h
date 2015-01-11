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
        const PerspectiveCamera& GetPerspectiveCamera() { return perspectiveCamera_; }

        void MoveCamera(const Vector& moveVector);
        void SetCameraXCoordinate(double x) { perspectiveCamera_.position.SetX(x); }
        void SetCameraYCoordinate(double y) { perspectiveCamera_.position.SetY(y); }
        void SetCameraZCoordinate(double z) { perspectiveCamera_.position.SetZ(z); }
        void SetCameraViewAngle(double angle) { perspectiveCamera_.SetViewAngle(angle); }

        void KeyPressed(int key);

    private:
        ViewWeakPtr view_;
        Scene3D scene_;

        Vector observerPosition_;
        double rotationAngle;
        PerspectiveCamera perspectiveCamera_;
};

#endif

