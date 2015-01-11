#include <iostream>
#include <QApplication>
#include <QtGui>

#include "Scene.h"
#include "Matrix.h"
#include "View.h"
#include "Controler.h"
#include "ObjDeserializer.h"

const int HEIGHT = 1080;
const int WIDTH = 1080;

int main(int argc, char** argv)
{

        QApplication app (argc, argv);

        ObjDeserializer deserializer;
        Scene3D scene = deserializer.ParseFile("scene.obj");

        scene.RecalculateNormals();
        scene.SetLightColor(QColor("white"));

        QWidget window;
        window.resize(1200, 700);
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

