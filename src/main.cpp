#include <iostream>
#include <QApplication>
#include <QtGui>

#include "Scene.h"
#include "Matrix.h"
#include "View.h"

const int HEIGHT = 1080;
const int WIDTH = 1080;

int main(int argc, char** argv)
{

        QApplication app (argc, argv);

        Scene3D scene;

        scene.AddPoint(0, 0, 0);
        scene.AddPoint(10, 0, 0);
        scene.AddPoint(10, 0, 10);

        scene.AddTriangle(0, 1, 2);

        scene.Transform(Matrix::CreateTranslationMatrix(10, 10, 10));
        scene.Transform(Matrix::CreateScaleMatrix(2, 2, 2));

        scene.SetObserverPosition(Point(0, 0, -7));
        Scene2D s2 = scene.GetPerspectiveProjection();
        std::cout << "P1: " << s2.triangles_[0].p1_.GetX() << " " << s2.triangles_[0].p1_.GetY() << std::endl;
        std::cout << "P2: " << s2.triangles_[0].p2_.GetX() << " " << s2.triangles_[0].p2_.GetY() << std::endl;
        std::cout << "P3: " << s2.triangles_[0].p3_.GetX() << " " << s2.triangles_[0].p3_.GetY() << std::endl;

        QWidget window;
        window.resize(500, 500);
        window.show();

        View* view = new View;       
        view->SetScene(s2);

        QLayout* layout = new QVBoxLayout;
        layout->addWidget(view);

        window.setLayout(layout);


        return app.exec();
}

