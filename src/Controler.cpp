#include "Controler.h"

int SCREEN_WIDTH = 500;
int SCREEN_HEIGHT = 500;
int ORTHOGONAL_CAMERA_WIDTH = 5;
int ORTHOGONAL_CAMERA_HEIGHT = 5;

Controler::Controler() : scene_(nullptr),
    rotationAngle(0), perspectiveCamera_(Vector(0, 0, -10), Vector(0, 1, 0),
        Vector(0, 0, 0), 3, 20, 0.78), activeShader_(new PhongShader),
    normalModifier_(new NoNormalModifier)
{
}

void Controler::SetView(ViewWeakPtr view)
{
    view_ = view;
}

OrthogonalProjection Controler::GetFrontView()
{
    OrthogonalCamera cam(Vector(0, 0, -5), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, ORTHOGONAL_CAMERA_WIDTH, ORTHOGONAL_CAMERA_HEIGHT);

    return scene_->RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam,
            *activeShader_, perspectiveCamera_.CalculateFrustrum(), *normalModifier_);
}

OrthogonalProjection Controler::GetSideView()
{
    OrthogonalCamera cam(Vector(-5, 0, 0), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, ORTHOGONAL_CAMERA_WIDTH, ORTHOGONAL_CAMERA_HEIGHT);

    return scene_->RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam,
            *activeShader_, perspectiveCamera_.CalculateFrustrum(), *normalModifier_);
}

OrthogonalProjection Controler::GetTopView()
{
    OrthogonalCamera cam(Vector(0, 5, 0), Vector(0, 1, 0),
            Vector(0, 0, 0), 3, 20, ORTHOGONAL_CAMERA_WIDTH, ORTHOGONAL_CAMERA_HEIGHT);

    return scene_->RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH, cam,
            *activeShader_, perspectiveCamera_.CalculateFrustrum(), *normalModifier_);
}

QImage Controler::GetRenderedPerspectiveView()
{
    QImage result = scene_->RenderProjection(SCREEN_WIDTH, SCREEN_WIDTH,
            perspectiveCamera_, *activeShader_, *normalModifier_);

    return result;
}

void Controler::MoveCamera(const Vector& moveVector)
{
    perspectiveCamera_.position = perspectiveCamera_.position + moveVector;
}


void Controler::MoveTargetPoint(const Vector& moveVector)
{
    perspectiveCamera_.target = perspectiveCamera_.target + moveVector;
}

void Controler::SaveSceneToObjFile(const std::string& path)
{
    ObjSerializer serializer;
    ObjFile objFile;

    objFile.scene = scene_;
    objFile.cameraPosition = perspectiveCamera_.position;

    serializer.SaveToFile(path, objFile);
}

void Controler::LoadObjFile(const std::string& path)
{
    ObjDeserializer deserializer;
    ObjFile parsedFile = deserializer.ParseFile(path);

    SetScene(parsedFile.scene);
    SetCameraPosition(parsedFile.cameraPosition);
}

void Controler::KeyPressed(int key)
{
    switch (key)
    {
        case Qt::Key_W: MoveCamera(Vector(0, 0, 0.1)); break;
        case Qt::Key_S: MoveCamera(Vector(0, 0, -0.1)); break;
        case Qt::Key_A: MoveCamera(Vector(-0.1, 0, 0)); break;
        case Qt::Key_D: MoveCamera(Vector(0.1, 0, 0)); break;
        case Qt::Key_Down: MoveCamera(Vector(0, -0.1, 0)); break;
        case Qt::Key_Up: MoveCamera(Vector(0, 0.1, 0)); break;
    }
}

