#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QKeyEvent>
#include <QGridLayout>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QPushButton>
#include <QSlider>
#include <memory>
#include <cassert>

#include "Scene.h"

class Controler;
class View;
typedef std::shared_ptr<Controler> ControlerPtr;

class OrthagonalViewport : public QWidget
{
    Q_OBJECT

    public:
        OrthagonalViewport(View* view) : view_(view), cameraMovingFlag_(false) {}
        void DrawScene(QImage& scene, const Vector& perspectiveCameraPosition);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

        virtual Vector GetCameraTranslation(int deltaX, int deltaY) = 0;

        View* view_;

        bool cameraMovingFlag_;
        QPoint lastMousePosition_;

        QRect cameraRect_;
        QImage buffer_;
};

class FrontView : public OrthagonalViewport
{
    public:
        FrontView(View* view) : OrthagonalViewport(view) {}

    protected:
        Vector GetCameraTranslation(int deltaX, int deltaY) { return Vector(deltaX, deltaY, 0); }
};

class TopView : public OrthagonalViewport
{
    public:
        TopView(View* view) : OrthagonalViewport(view) {}

    protected:
        Vector GetCameraTranslation(int deltaX, int deltaY) { return Vector(-deltaX, 0, -deltaY); }
};

class SideView : public OrthagonalViewport
{
    public:
        SideView(View* view) : OrthagonalViewport(view) {}

    protected:
        Vector GetCameraTranslation(int deltaX, int deltaY) { return Vector(0, deltaY, -deltaX); }
};

class PerspectiveViewport : public QWidget
{
    Q_OBJECT

    public:
        void DrawScene(QImage& scene);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        QPoint perspectiveCameraPosition_;
        QImage buffer_;
};

class ConfigurationPanel : public QWidget
{
    Q_OBJECT

    public:
        ConfigurationPanel();
        void SetControler(ControlerPtr controler) { controler_ = controler; }

        void UpdateCameraParameters(const PerspectiveCamera& camera);

    public slots:
        void OnXCameraPositionEntered();
        void OnYCameraPositionEntered();
        void OnZCameraPositionEntered();

    private:
        QGridLayout mainLayout_;

        QDoubleValidator coordinatesValidator_;
        QLabel xCameraPostitionLabel_;
        QLineEdit xCameraPostitionEdit_;
        QLabel yCameraPostitionLabel_;
        QLineEdit yCameraPostitionEdit_;
        QLabel zCameraPostitionLabel_;
        QLineEdit zCameraPostitionEdit_;

        QLabel cameraViewAngleLabel_;
        QSlider cameraViewAngleSlider_;

        QGroupBox shadowsTypeRadioButtons_;
        QVBoxLayout shadowsTypeRadioButtonsLayout_;
        QRadioButton flatShadingRadioButton_;
        QRadioButton gouroudShadingRadioButton_;
        QRadioButton phongShadingRadioButton_;

        QPushButton openFileButton_;
        QPushButton saveFileButton_;

        ControlerPtr controler_;
};

class View : public QWidget
{
    Q_OBJECT

    public:
        View();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        void SetControler(ControlerPtr controler);
        void MoveCamera(const Vector& moveVector);

    protected:
        void UpdateCameraViews();
        void keyPressEvent(QKeyEvent* event) override;

    private:
        PerspectiveViewport perspectiveView_;
        FrontView frontView_;
        SideView sideView_;
        TopView topView_;
        ConfigurationPanel configurationPanel_;
        QGridLayout layout;

        ControlerPtr controler_;
};

typedef std::shared_ptr<View> ViewPtr;

#endif
