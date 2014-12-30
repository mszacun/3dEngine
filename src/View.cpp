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

    QColor c("chartreuse");
    QColor p("red");

    QImage i = controler_->GetRenderedPerspectiveView();
    painter.drawImage(200, 200, i);
}



void View::keyPressEvent(QKeyEvent* event)
{
    QWidget::keyPressEvent(event);
}
