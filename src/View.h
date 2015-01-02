#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QPainter>
#include <memory>
#include <cassert>

#include "Scene.h"

class Controler;
typedef std::shared_ptr<Controler> ControlerPtr;

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
        void paintEvent(QPaintEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;

    private:
        Scene2D scene_;
        ControlerPtr controler_;

};

typedef std::shared_ptr<View> ViewPtr;

#endif
