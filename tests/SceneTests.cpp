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

TEST(SceneTests, ShouldCalculateTriangleNormalVector2)
{
    Point p1(-1, 0, 1);
    Point p2(3, -3, 2);
    Point p3(1, 0 ,-6);
    Scene3D scene;
    Triangle3D triangle(0, 1, 2);
    Vector expectedNormal(7/(3 * sqrt(17)), 10 / (3*sqrt(17)), 2 / (3*sqrt(17)));

    scene.AddPoint(p1);
    scene.AddPoint(p2);
    scene.AddPoint(p3);

    Vector calculatedNormal = scene.CalculateNormal(triangle);

    EXPECT_TRUE(calculatedNormal == expectedNormal);
}

TEST(SceneTests, ShouldCalculateTriangleNormalVector3)
{
    Point p1(-1, 0, 1);
    Point p2(1, 0 ,-6);
    Point p3(-2, 1, -1);
    Scene3D scene;
    Triangle3D triangle(1, 2, 0);
    Vector expectedNormal(7 / sqrt(174), 11/sqrt(174), sqrt(2.0/87.0));

    scene.AddPoint(p1);
    scene.AddPoint(p2);
    scene.AddPoint(p3);

    Vector calculatedNormal = scene.CalculateNormal(triangle);

    EXPECT_TRUE(calculatedNormal == expectedNormal);
}

TEST(SceneTests, ShouldCalculateNormalVectorForVertex)
{
    Scene3D scene;
    Point p1(-1, 0, 1);
    Point p2(1, 0 ,-6);
    Point p3(-2, 1, -1);
    Point p4(3, -3, 2);
    Triangle3D triangle(1, 2, 0);
    Triangle3D triangle2(0, 3, 1);

    scene.AddPoint(p1);
    scene.AddPoint(p2);
    scene.AddPoint(p3);
    scene.AddPoint(p4);
    scene.AddTriangle(triangle);
    scene.AddTriangle(triangle2);

    Vector vertextNormal = scene.CalculatePointNormal(0);
    Vector expectedNormal = Vector(0.548431, 0.821383, 0.156694);

    EXPECT_TRUE(vertextNormal == expectedNormal);
}
