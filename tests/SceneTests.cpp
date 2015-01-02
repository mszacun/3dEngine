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
    scene.Transform(Matrix::CreateScaleMatrix(20, 20, 1));

    Scene2D perspectiveProjection = scene.GetPerspectiveProjection();

    Triangle2D expectedProjectedTriangle1(
            Point(42.5, 77.5, 0),
            Point(77.5, 77.5, 0),
            Point(77.5, 42.5, 0));
    Triangle2D expectedProjectedTriangle2(
            Point(42.5, 77.5, 0),
            Point(77.5, 42.5, 0),
            Point(42.5, 42.5, 0));

    EXPECT_TRUE(expectedProjectedTriangle1 == perspectiveProjection.triangles_[0]);
    EXPECT_TRUE(expectedProjectedTriangle2 == perspectiveProjection.triangles_[1]);
}

TEST(SceneTests, ShouldTransformFartherPointsSmaller){
    Scene3D scene;

    scene.AddPoint(1, 1, 3);
    scene.AddPoint(-1, 1, 3);
    scene.AddPoint(1, -1, 3);
    scene.AddPoint(-1, -1, 3);

    scene.AddTriangle(1, 0, 2);
    scene.AddTriangle(1, 2, 3);

    scene.SetObserverPosition(Point(0, 0, -7));

    scene.Transform(Matrix::CreateTranslationMatrix(3, 3, 0));
    scene.Transform(Matrix::CreateScaleMatrix(20, 20, 1));

    Scene2D perspectiveProjection = scene.GetPerspectiveProjection();

    Triangle2D expectedProjectedTriangle1(
            Point(45.9999, 74, 0),
            Point(74, 74, 0),
            Point(74, 45.9999, 0));
    Triangle2D expectedProjectedTriangle2(
            Point(45.9999, 74, 0),
            Point(74, 45.9999, 0),
            Point(45.9999, 45.9999, 0));

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
