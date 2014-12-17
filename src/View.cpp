#include "View.h"
#include "Controler.h"

void View::SetScene(const Scene2D& scene)
{
    scene_ = scene;
}

QSize View::minimumSizeHint() const
{
    return QSize(400, 400);
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
    QPainter painter(this);

    for (const Triangle2D& t : controler_->GetPerspectiveProjection().triangles_)
    {
        painter.drawLine((int) t.p1_.GetX(), (int) t.p1_.GetY(), (int) t.p2_.GetX(), (int) t.p2_.GetY());
        painter.drawLine((int) t.p2_.GetX(), (int) t.p2_.GetY(), (int) t.p3_.GetX(), (int) t.p3_.GetY());
        painter.drawLine((int) t.p3_.GetX(), (int) t.p3_.GetY(), (int) t.p1_.GetX(), (int) t.p1_.GetY());
    }
}

void View::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
}
