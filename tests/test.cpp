// Copyright 2019 Kirill <your_email>

#include <gtest/gtest.h>
#include <header.hpp>

TEST(Example, EmptyTest) {
    cache experiment;
    experiment.start();
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}