#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QPainter>

#include "Scene.h"

class View : public QWidget
{
    Q_OBJECT

    public:
        void SetScene(const Scene2D& scene);

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        Scene2D scene_;
};

#endif
