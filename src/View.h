#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QGridLayout>
#include <memory>
#include <cassert>

#include "Scene.h"

class Controler;
typedef std::shared_ptr<Controler> ControlerPtr;

class OrthagonalViewport : public QWidget
{
    Q_OBJECT

    public:
        void DrawScene(QImage& scene, const QPoint& perspectiveCameraPosition);

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        QPoint perspectiveCameraPosition_;
        QImage buffer_;
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

class View : public QWidget
{
    Q_OBJECT

    public:
        View();
        void SetScene(const Scene2D& scene);

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        void SetControler(ControlerPtr controler);

    protected:
        void UpdateCameraViews();
        void keyPressEvent(QKeyEvent* event) override;
        void paintEvent(QPaintEvent* event) override;

    private:
        PerspectiveViewport perspectiveView_;
        OrthagonalViewport sideView_;
        OrthagonalViewport topView_;
        OrthagonalViewport frontView_;
        QGridLayout layout;

        ControlerPtr controler_;
        Scene2D scene_;

};

typedef std::shared_ptr<View> ViewPtr;

#endif
