#ifndef CONTROLER_H
#define CONTROLER_H

#include <memory>

#include "Scene.h"
#include "ObjDeserializer.h"

class View;
typedef std::weak_ptr<View> ViewWeakPtr;

class Controler
{
    public:
        Controler();
        void SetView(ViewWeakPtr view);

        OrthogonalProjection GetFrontView();
        OrthogonalProjection GetSideView();
        OrthogonalProjection GetTopView();
        QImage GetRenderedPerspectiveView();
        const PerspectiveCamera& GetPerspectiveCamera() { return perspectiveCamera_; }

        void SetScene(ScenePtr scene) { scene_ = scene; }
        bool HasScene() { return scene_ != nullptr; }

        void MoveCamera(const Vector& moveVector);
        void MoveTargetPoint(const Vector& moveVector);
        void SetCameraPosition(const Vector& position) { perspectiveCamera_.position = position; }
        void SetCameraXCoordinate(double x) { perspectiveCamera_.position.SetX(x); }
        void SetCameraYCoordinate(double y) { perspectiveCamera_.position.SetY(y); }
        void SetCameraZCoordinate(double z) { perspectiveCamera_.position.SetZ(z); }
        void SetCameraViewAngle(double angle) { perspectiveCamera_.SetViewAngle(angle); }

        void SetShader(Shader* shader) { delete activeShader_; activeShader_ = shader; }
        void SetFlatShader() { SetShader(new FlatShader); }
        void SetGouroudShader() { SetShader(new GouraudShader); }
        void SetPhongShader() { SetShader(new PhongShader); }

        void SaveSceneToObjFile(const std::string& path);
        void LoadObjFile(const std::string& path);

        void KeyPressed(int key);

    private:
        ViewWeakPtr view_;
        ScenePtr scene_;

        double rotationAngle;
        PerspectiveCamera perspectiveCamera_;
        Shader* activeShader_;
        NormalModifier* normalModifier_;
};

#endif

