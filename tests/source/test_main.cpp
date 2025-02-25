#include <gtest/gtest.h>

TEST(SampleTest, Test1) {
    EXPECT_EQ(1, 1);
}

TEST(SampleTest, Test2) {
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}