#include <iostream>
#include <QApplication>
#include <QtGui>

#include "Scene.h"
#include "Matrix.h"
#include "View.h"
#include "Controler.h"

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

        QWidget window;
        window.resize(500, 500);
        window.show();

        ViewPtr view = std::make_shared<View>();
        ControlerPtr controler = std::make_shared<Controler>(scene);

        view->SetControler(controler);
        controler->SetView(view);


        QLayout* layout = new QVBoxLayout;
        layout->addWidget(view.get());

        window.setLayout(layout);


        return app.exec();
}

