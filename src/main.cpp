#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include <sys/time.h>
#include <functional>

const int HEIGHT = 1080;
const int WIDTH = 1080;
void BenchmarkQPixmap();

long long Benchmark(const std::function<void()>& f)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    f();

    gettimeofday(&end, NULL);
    return (end.tv_sec * (unsigned int)1e6 +   end.tv_usec) -
         (start.tv_sec * (unsigned int)1e6 + start.tv_usec);
}


int main(int argc, char** argv)
{

        QApplication app (argc, argv);

        for (int i = 0; i < 10; i++)
            std::cout << Benchmark(BenchmarkQPixmap) << std::endl;
        QPushButton button ("Hello world !");
        button.show();


        return app.exec();
}

void BenchmarkQPixmap()
{
    QPixmap pixmap(WIDTH, HEIGHT);
    QPainter painter(&pixmap);

    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            painter.drawPoint(x,y);
}
