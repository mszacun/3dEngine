#include <gtest/gtest.h>
#include "../src/Point.h"

TEST(PointTests, ShouldCreatePoint){
    Point p(1,666,23);

    EXPECT_EQ(p.x_, 1);
    EXPECT_EQ(p.y_, 666);
    EXPECT_EQ(p.z_, 23);
}

