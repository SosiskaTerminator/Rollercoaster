#include <gtest/gtest.h>
#include "find_mount.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

std::string RunFindFunc(const std::vector<float>& data,
                        int min_width,
                        int max_width) {
    static std::size_t test_number = 0;

    const auto file_path =
        std::filesystem::temp_directory_path() /
        ("rollercoaster_find_mount_test_" +
         std::to_string(
             std::chrono::high_resolution_clock::now()
                 .time_since_epoch()
                 .count()) +
         "_" + std::to_string(test_number++) + ".txt");

    std::fstream outfile(
        file_path,
        std::ios::in | std::ios::out | std::ios::trunc);

    if (!outfile.is_open()) {
        return "";
    }

    std::vector<float> test_data = data;

    find_func(outfile, test_data, min_width, max_width);

    outfile.flush();
    outfile.seekg(0);

    const std::string result(
        (std::istreambuf_iterator<char>(outfile)),
        std::istreambuf_iterator<char>());

    outfile.close();
    std::filesystem::remove(file_path);

    return result;
}


TEST(FindFuncTest, EmptyInputProducesNoOutput) {
    EXPECT_TRUE(RunFindFunc({}, 1, 100).empty());
}

TEST(FindFuncTest, ConstantSignalContainsNoMountains) {
    const std::vector<float> data(30, 5.0f);

    EXPECT_TRUE(RunFindFunc(data, 1, 100).empty());
}

TEST(FindFuncTest, FindsSingleMountain) {
    const std::vector<float> data{
        0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0
    };

    EXPECT_EQ(
        RunFindFunc(data, 5, 5),
        "5 9\n"
    );
}

TEST(FindFuncTest, FindsSeveralSeparatedMountains) {
    const std::vector<float> data{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    EXPECT_EQ(
        RunFindFunc(data, 5, 5),
        "10 14\n25 29\n"
    );
}

TEST(FindFuncTest, RejectsMountainThatIsTooNarrow) {
    const std::vector<float> data{
        0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0
    };

    EXPECT_TRUE(
        RunFindFunc(data, 6, 100).empty()
    );
}

TEST(FindFuncTest, RejectsMountainThatIsTooWide) {
    const std::vector<float> data{
        0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0
    };

    EXPECT_TRUE(
        RunFindFunc(data, 3, 4).empty()
    );
}

TEST(FindFuncTest, MountainAtBeginningIsHandledCorrectly) {
    const std::vector<float> data{
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    EXPECT_EQ(RunFindFunc(data, 4, 5), "0 4\n");
}

TEST(FindFuncTest, MountainAtEndIsHandledCorrectly) {
    const std::vector<float> data{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10
    };

    EXPECT_EQ(RunFindFunc(data, 5, 5), "10 14\n");
}

TEST(FindFuncTest, LowThresholdExpandsMountainBeyondHighThreshold) {
    const std::vector<float> data{
        0, 1, 1, 1, 2,
        5, 5, 5, 5,
        0, 1, 1, 1, 2
    };

    EXPECT_EQ(RunFindFunc(data, 7, 8), "1 8\n");
}

TEST(FindFuncTest, ValueExactlyAtHighIsNotAMountain) {
    const std::vector<float> data{
        0, 1, 1, 1, 1,
        1,
        0, 1, 1, 1, 1
    };

    EXPECT_TRUE(RunFindFunc(data, 1, 100).empty());
}