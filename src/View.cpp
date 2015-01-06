#include "View.h"
#include "Controler.h"
#include <chrono>

View::View()
{
    setFocusPolicy(Qt::ClickFocus);
}

void View::SetScene(const Scene2D& scene)
{
    scene_ = scene;
}

QSize View::minimumSizeHint() const
{
    return QSize(800, 800);
}

QSize View::sizeHint() const
{
    return minimumSizeHint();
}

void View::SetControler(ControlerPtr controler)
{
    controler_ = controler;
}

void View::paintEvent(QPaintEvent* event)
{
    auto start = std::chrono::steady_clock::now();
    QPainter painter(this);

    QImage i = controler_->GetRenderedPerspectiveView();
    painter.drawImage(0, 0, i);

    i = controler_->GetFrontView();
    painter.drawImage(400, 400, i);

    i = controler_->GetSideView();
    painter.drawImage(0, 400, i);

    i = controler_->GetTopView();
    painter.drawImage(400, 0, i);

    auto end = std::chrono::steady_clock::now();


    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Drawing frame: " << dur << std::endl;
    std::cout << "FPS: " << 1000.0 / dur << std::endl;
}

void View::keyPressEvent(QKeyEvent* event)
{
    controler_->KeyPressed(event->key());
    update();
    QWidget::keyPressEvent(event);
}
