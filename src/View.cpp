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
//    QPainter painter(this);

    QImage i = controler_->GetRenderedPerspectiveView();
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
    std::cout << "FPS: " << 1000.0 / dur << std::endl;
}

void View::keyPressEvent(QKeyEvent* event)
{
    controler_->KeyPressed(event->key());
    UpdateCameraViews();
    QWidget::keyPressEvent(event);
}
