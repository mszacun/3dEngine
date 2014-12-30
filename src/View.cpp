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

    QColor c("chartreuse");
    DrawTriangle(Point(50, 50, 0), Point(0, 150, 0), Point(100, 150, 0), c, painter);
    DrawTriangle(Point(150, 50, 0), Point(100, 150, 0), Point(200, 150, 0), c, painter);
    DrawTriangle(Point(100, 150, 0), Point(50, 50, 0), Point(150, 50, 0), c, painter);
}

void View::DrawTriangle(const Point& p1, const Point& p2, const Point& p3, QColor& color, QPainter& painter)
{
    // inv: p2 and p3 are on the same line, p1 is peak of triangle
    assert(p2.GetY() == p3.GetY());

    painter.setPen(color);
    double ystart = p1.GetY();
    double yend = p2.GetY();
    int direction = ystart < yend ? 1 : -1;
    for (int y = (int) ystart; y != yend; y += direction)
    {
        double betay = (y - yend) / (ystart - yend);
        double xl = betay * p1.GetX() + (1 - betay) * p2.GetX();
        double xr = betay * p1.GetX() + (1 - betay) * p3.GetX();

        for (int x = (int) xl; x < xr; x++)
            painter.drawPoint(x, y);

    }
}

void View::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
}
