#include <gtest/gtest.h>

TEST(Test, TEST){
    int x = 2;
    EXPECT_EQ(x, x);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
