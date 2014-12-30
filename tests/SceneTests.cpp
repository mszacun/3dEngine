#include <gtest/gtest.h>
#include "../src/Scene.h"

// test based on output, transforms plane into plane, doesnt deform it
TEST(SceneTests, ShouldTransformPerspectively){
    Scene3D scene;

    scene.AddPoint(1, 1, 1);
    scene.AddPoint(-1, 1, 1);
    scene.AddPoint(1, -1, 1);
    scene.AddPoint(-1, -1, 1);

    scene.AddTriangle(1, 0, 2);
    scene.AddTriangle(1, 2, 3);

    scene.SetObserverPosition(Point(0, 0, -7));

    scene.Transform(Matrix::CreateTranslationMatrix(3, 3, 0));
    scene.Transform(Matrix::CreateScaleMatrix(20, 20, 10));

    Scene2D perspectiveProjection = scene.GetPerspectiveProjection();

    Triangle2D expectedProjectedTriangle1(
            Point(16.4706, 32.9412, 0),
            Point(32.9412, 32.9412, 0),
            Point(32.9412, 16.4706, 0));
    Triangle2D expectedProjectedTriangle2(
            Point(16.4706, 32.9412, 0),
            Point(32.9412, 16.4706, 0),
            Point(16.4706, 16.4706, 0));

    EXPECT_TRUE(expectedProjectedTriangle1 == perspectiveProjection.triangles_[0]);
    EXPECT_TRUE(expectedProjectedTriangle2 == perspectiveProjection.triangles_[1]);
}

TEST(SceneTests, ShouldTransformFartherPointsSmaller){
    Scene3D scene;

    scene.AddPoint(1, 1, 1);
    scene.AddPoint(-1, 1, 1);
    scene.AddPoint(1, -1, 1);
    scene.AddPoint(-1, -1, 1);

    scene.AddTriangle(1, 0, 2);
    scene.AddTriangle(1, 2, 3);

    scene.SetObserverPosition(Point(0, 0, -7));

    scene.Transform(Matrix::CreateTranslationMatrix(3, 3, 0));
    scene.Transform(Matrix::CreateScaleMatrix(20, 20, 70));

    Scene2D perspectiveProjection = scene.GetPerspectiveProjection();

    Triangle2D expectedProjectedTriangle1(
            Point(3.63636, 7.27273, 0),
            Point(7.27273, 7.27273, 0),
            Point(7.27273, 3.63636, 0));
    Triangle2D expectedProjectedTriangle2(
            Point(3.63636, 7.27273, 0),
            Point(7.27273, 3.63636, 0),
            Point(3.63636, 3.63636, 0));

    EXPECT_TRUE(expectedProjectedTriangle1 == perspectiveProjection.triangles_[0]);
    EXPECT_TRUE(expectedProjectedTriangle2 == perspectiveProjection.triangles_[1]);
}

TEST(SceneTests, ShouldCalculateTriangleNormalVector)
{
    Point p1(-1, 0, 5);
    Point p2(2, -4, 8);
    Point p3(5, 0, 0);
    Scene3D scene;
    Triangle3D triangle(0, 1, 2);
    Vector expectedNormal(-4*sqrt(5.0/413.0), -33/sqrt(2065), -24/sqrt(2065));

    scene.AddPoint(p1);
    scene.AddPoint(p2);
    scene.AddPoint(p3);

    Vector calculatedNormal = scene.CalculateNormal(triangle);

    EXPECT_TRUE(calculatedNormal == expectedNormal || -calculatedNormal == expectedNormal);
}

