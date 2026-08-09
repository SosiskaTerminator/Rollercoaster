#include <gtest/gtest.h>
#include "find_const.h"

TEST(CalcConstTest, OddNumberOfElements) {
    const std::vector<float> data{1.0f, 2.0f, 3.0f, 4.0f, 100.0f};

    const auto [median, mad] = calc_const(data);

    EXPECT_FLOAT_EQ(median, 3.0f);
    EXPECT_FLOAT_EQ(mad, 1.0f);
}

TEST(CalcConstTest, EvenNumberOfElements) {
    const std::vector<float> data{1.0f, 2.0f, 3.0f, 4.0f};

    const auto [median, mad] = calc_const(data);

    EXPECT_FLOAT_EQ(median, 2.5f);
    EXPECT_FLOAT_EQ(mad, 1.0f);
}

TEST(CalcConstTest, ConstantDataHasZeroMad) {
    const std::vector<float> data{7.0f, 7.0f, 7.0f, 7.0f, 7.0f};

    const auto [median, mad] = calc_const(data);

    EXPECT_FLOAT_EQ(median, 7.0f);
    EXPECT_FLOAT_EQ(mad, 0.0f);
}

TEST(CalcConstTest, MedianIsInsensitiveToLargeOutlier) {
    const std::vector<float> data{10.0f, 10.0f, 10.0f, 10.0f, 1000.0f};

    const auto [median, mad] = calc_const(data);

    EXPECT_FLOAT_EQ(median, 10.0f);
    EXPECT_FLOAT_EQ(mad, 0.0f);
}
