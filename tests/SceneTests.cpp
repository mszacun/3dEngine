#include <gtest/gtest.h>
#include "../src/Scene.h"

TEST(SceneTests, ShouldAddPoint){
    Scene3D s;

    s.AddPoint(3, 5, 10);
}

TEST(SceneTests, ShouldAddTriangle){
    Scene3D s;

    s.AddTriangle(3, 5, 10);
}

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
