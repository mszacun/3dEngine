
#include <gtest/gtest.h>
#include "../src/Interpolator.h"

TEST(InterpolatorTests, ShouldConvertCoordinatesToBarycentric){
    Vector p1(1, 0, 0);
    Vector p2(0, 1, 0);
    Vector p3(0, 0, 1);
    double cord = 1.0/3.0;

    BarycentricCoordinates barycentric = ConvertToBarycentric(p1, p2, p3, Vector(cord, cord,
        cord));

    EXPECT_NEAR(barycentric.l1, 0.3333, 0.0001);
    EXPECT_NEAR(barycentric.l2, 0.3333, 0.0001);
    EXPECT_NEAR(barycentric.l3, 0.3333, 0.0001);
}

TEST(InterpolatorTests, ShouldConvertCoordinatesToBarycentric2){
    Vector p1(1, 2, 3);
    Vector p2(0, 0, 3);
    Vector p3(4, -1, 3);

    BarycentricCoordinates barycentric = ConvertToBarycentric(p1, p2, p3,
        Vector(0.5, 1, 3));

    EXPECT_NEAR(barycentric.l1, 0.5, 0.0001);
    EXPECT_NEAR(barycentric.l2, 0.5, 0.0001);
    EXPECT_NEAR(barycentric.l3, 0.0, 0.0001);
}

TEST(InterpolatorTests, ShouldInterpolateNumbers){
    Vector p1(-7, 5, 3);
    int p1Value = 10;

    Vector p2(3, 2, 3);
    int p2Value = 2;

    Vector p3(-2, -2, 3);
    int p3Value = -20;

    Vector interpolatedPoint(-1, 0, 3);
    Triangle3DInterpolator<double> interpolator(p1, p1Value, p2, p2Value, p3, p3Value);

    double interpolatedValue = interpolator.Interpolate(interpolatedPoint);

    EXPECT_NEAR(interpolatedValue, -9.928, 0.001);
}
