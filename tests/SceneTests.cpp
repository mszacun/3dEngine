#include <gtest/gtest.h>
#include "../src/Scene.h"

TEST(SceneTests, ShouldCalculateTriangleNormalVector)
{
    Vector p1(-1, 0, 5);
    Vector p2(2, -4, 8);
    Vector p3(5, 0, 0);
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
    Vector p1(-1, 0, 1);
    Vector p2(3, -3, 2);
    Vector p3(1, 0 ,-6);
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
    Vector p1(-1, 0, 1);
    Vector p2(1, 0 ,-6);
    Vector p3(-2, 1, -1);
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
    Vector p1(-1, 0, 1);
    Vector p2(1, 0 ,-6);
    Vector p3(-2, 1, -1);
    Vector p4(3, -3, 2);
    Triangle3D triangle(1, 2, 0);
    Triangle3D triangle2(0, 3, 1);

    scene.AddPoint(p1);
    scene.AddPoint(p2);
    scene.AddPoint(p3);
    scene.AddPoint(p4);
    scene.AddTriangle(triangle);
    scene.AddTriangle(triangle2);

    scene.RecalculateNormals();
    Vector vertextNormal = scene.CalculatePointNormal(0);
    Vector expectedNormal = Vector(0.548431, 0.821383, 0.156694);

    EXPECT_TRUE(vertextNormal == expectedNormal);
}
