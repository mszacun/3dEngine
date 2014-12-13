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
