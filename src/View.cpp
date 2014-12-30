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
    QColor p("red");
    DrawTriangle(Point(50, 50, 0), Point(0, 150, 0), Point(100, 150, 0), c, painter);
    DrawTriangle(Point(150, 50, 0), Point(100, 150, 0), Point(200, 150, 0), c, painter);
    DrawTriangle(Point(100, 150, 0), Point(50, 50, 0), Point(150, 50, 0), c, painter);
    DrawTriangle(Point(350, 100, 0), Point(300, 160, 0), Point(410, 200, 0), p, painter);
    DrawTriangle(Point(200, 300, 0), Point(150, 500, 0), Point(300, 420, 0), p, painter);
    DrawTriangle(Point(100, 100, 0), Point(0, 100, 0), Point(100, 200, 0), p, painter);
}

void SortVertices(const Point& p1, const Point& p2, const Point& p3,
        Point& left, Point& center, Point& right)
{
    if (p1.GetX() < p2.GetX())
    {
        if (p1.GetX() < p3.GetX())
        {
            left = p1;
            if (p3.GetX() < p2.GetX())
            {
                center = p3;
                right = p2;
            }
            else
            {
                center = p2;
                right = p3;
            }
        }
        else
        {
            left = p3;
            center = p1;
            right = p2;
        }
    }
    else
    {
        if (p2.GetX() < p3.GetX())
        {
            left = p2;
            if (p3.GetX() < p1.GetX())
            {
                center = p3;
                right = p1;
            }
            else
            {
                center = p1;
                right = p3;
            }
        }
        else
        {
            left = p3;
            center = p2;
            right = p1;
        }
    }
    if (center.GetY() == left.GetY())
        std::swap(center, right);
    if (center.GetY() == right.GetY())
        std::swap(center, left);
}

void DrawTriangle2(const Point& p1, const Point& p2, const Point& p3, QColor& color, QPainter& painter)
{
    // inv: p2 and p3 are on the same line, p1 is peak of triangle
    assert(p2.GetY() == p3.GetY());

    painter.setPen(color);
    double ystart = p1.GetY();
    double yend = p2.GetY();
    int direction = ystart < yend ? 1 : -1;
    for (int y = (int) ystart; y != yend + direction; y += direction)
    {
        double betay = (y - yend) / (ystart - yend);
        double xl = betay * p1.GetX() + (1 - betay) * p2.GetX();
        double xr = betay * p1.GetX() + (1 - betay) * p3.GetX();

        for (int x = (int) xl; x < xr; x++)
            painter.drawPoint(x, y);

    }
}

void View::DrawTriangle(const Point& p1, const Point& p2, const Point& p3, QColor& color, QPainter& painter)
{
    // inv: p2 is on the left of p3
    QColor c1("black");
    QColor c2("yellow");
    Point left, right, center;
    SortVertices(p1, p2, p3, left, center, right);
    if (left.GetY() == right.GetY())
        DrawTriangle2(center, left, right, color, painter);
    else
    {
        if (left.GetY() < right.GetY())
        {
            double y = left.GetY();
            double betay = (y - right.GetY()) / (center.GetY() - right.GetY());
            double xr = betay * center.GetX() + (1 - betay) * right.GetX();

            DrawTriangle2(center, left, Point(xr, left.GetY(), 0), c1, painter);
            DrawTriangle2(right, left, Point(xr, left.GetY(), 0), c2, painter);
        }
        else
        {
            double y = right.GetY();
            double betay = (y - left.GetY()) / (center.GetY() - left.GetY());
            double xl = betay * center.GetX() + (1 - betay) * left.GetX();

            DrawTriangle2(center, Point(xl, right.GetY(), 0), right, c1, painter);
            DrawTriangle2(left, Point(xl, right.GetY(), 0), right, c2, painter);
        }
    }

}

void View::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
}
