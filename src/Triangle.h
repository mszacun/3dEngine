#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle3D
{
    public:
        Triangle3D(const unsigned int& p1, const unsigned  int& p2,
            const unsigned int& p3);

    private:
        unsigned int p1_;
        unsigned int p2_;
        unsigned int p3_;
};

#endif
