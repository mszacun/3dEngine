#include "View.h"
#include "Controler.h"
#include <chrono>

void OrthagonalViewport::DrawScene(QImage& scene, const QPoint& perspectiveCameraPosition)
{
    buffer_ = scene;
    perspectiveCameraPosition_ = perspectiveCameraPosition;
    update();
}

void OrthagonalViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawImage(0, 0, buffer_);
}

void PerspectiveViewport::DrawScene(QImage& scene)
{
    buffer_ = scene;
    update();
}

void PerspectiveViewport::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.drawImage(0, 0, buffer_);
}

View::View()
{
    setFocusPolicy(Qt::ClickFocus);
    
    layout.addWidget(&perspectiveView_, 0, 0);
    layout.addWidget(&frontView_, 0, 1);
    layout.addWidget(&sideView_, 1, 1);
    layout.addWidget(&topView_, 1, 0);

    setLayout(&layout);
}

void View::SetScene(const Scene2D& scene)
{
    scene_ = scene;
}

QSize View::minimumSizeHint() const
{
    return QSize(1000, 1000);
}

QSize View::sizeHint() const
{
    return minimumSizeHint();
}

void View::SetControler(ControlerPtr controler)
{
    controler_ = controler;
    UpdateCameraViews();
}

void View::UpdateCameraViews()
{
    auto start = std::chrono::steady_clock::now();

/*    QImage i = controler_->GetRenderedPerspectiveView();
    perspectiveView_.DrawScene(i);

    i = controler_->GetFrontView();
    frontView_.DrawScene(i, QPoint(0, 0));

    i = controler_->GetSideView();
    sideView_.DrawScene(i, QPoint(0, 0));

    i = controler_->GetTopView();
    topView_.DrawScene(i, QPoint(0, 0));

    auto end = std::chrono::steady_clock::now();


    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Drawing frame: " << dur << std::endl;
    std::cout << "FPS: " << 1000.0 / dur << std::endl;*/
}

void View::paintEvent(QPaintEvent* event)
{
    TriangleShadingInfo shadingInfo;
    QPainter painter(this);

    shadingInfo.projectedP1 = Vector(100, 100, 1);
    shadingInfo.projectedP2 = Vector(95, 105, 1);
    shadingInfo.projectedP3 = Vector(105, 105, 1);

    InterpolateShader shader(shadingInfo, QColor(123, 124, 32), QColor(254, 34, 65), QColor(12, 53, 255));
    QImage i(500, 500, QImage::Format_ARGB32);
    QPainter p(&i);


    Scene3D scene;
    scene.DrawTriangle(shadingInfo.projectedP1, shadingInfo.projectedP2, shadingInfo.projectedP3,
        p, shader, InterpolateZ);

    painter.drawImage(100, 100, i);
}


void View::keyPressEvent(QKeyEvent* event)
{
    controler_->KeyPressed(event->key());
    UpdateCameraViews();
    QWidget::keyPressEvent(event);
}
