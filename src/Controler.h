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

        Scene2D GetPerspectiveProjection() const;
        QImage GetRenderedPerspectiveView();

    private:
        ViewWeakPtr view_;
        Scene3D scene_;
};

#endif

