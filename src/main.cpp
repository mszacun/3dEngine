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
        ObjFile parsedFile = deserializer.ParseFile("scene.obj");
        Scene3D scene = parsedFile.scene;

        scene.RecalculateNormals();
        scene.SetLightColor(QColor("white"));

        QImage image;
        image.load("texture.jpg");
        MaterialPtr material = std::make_shared<ImageTextureMaterial>(image);
        scene.SetMaterial(material);
        scene.SetAmbientLightColor(QColor(50, 50, 50));

        QWidget window;
        window.resize(1200, 700);
        window.show();

        ViewPtr view = std::make_shared<View>();
        ControlerPtr controler = std::make_shared<Controler>(scene);

        controler->SetCameraPosition(parsedFile.cameraPosition);

        view->SetControler(controler);
        controler->SetView(view);

        QLayout* layout = new QVBoxLayout;
        layout->addWidget(view.get());

        window.setLayout(layout);


        return app.exec();
}

