#ifndef CONTROLER_H
#define CONTROLER_H

#include "View.h"
#include "Scene.h"

class Controler
{
    public:
        Controler(const Scene3D& scene);
        void SetView(ViewPtr view);

        Scene2D GetPerspectiveProjection() const;

    private:
        ViewPtr view_;
        Scene3D scene_;
};

#endif

