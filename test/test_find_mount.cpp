#include <gtest/gtest.h>
#include "find_mount.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>

std::string RunFindFunc(const std::vector<float>& data,
                        int min_width,
                        int max_width, bool type) {
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

    if (type) { find_mount_func(outfile, test_data, min_width, max_width); }
    else { reference(outfile, test_data, min_width, max_width); }
    

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
    EXPECT_TRUE(RunFindFunc({}, 1, 100,true).empty());
}

TEST(FindFuncTest, ConstantSignalContainsNoMountains) {
    const std::vector<float> data(30, 5.0f);

    EXPECT_TRUE(RunFindFunc(data, 1, 100,true).empty());
}

TEST(FindFuncTest, FindsSingleMountain) {
    const std::vector<float> data{
        0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0
    };

    EXPECT_EQ(
        RunFindFunc(data, 5, 10,true),
        "4 10\n"
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
        RunFindFunc(data, 5, 10,true),
        "9 15\n24 30\n"
    );
}

TEST(FindFuncTest, RejectsMountainThatIsTooNarrow) {
    const std::vector<float> data{
        0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0
    };

    EXPECT_TRUE(
        RunFindFunc(data, 10, 100,true).empty()
    );
}

TEST(FindFuncTest, RejectsMountainThatIsTooWide) {
    const std::vector<float> data{
        0, 0, 0, 0, 0,
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0
    };

    EXPECT_TRUE(
        RunFindFunc(data, 3, 4,true).empty()
    );
}

TEST(FindFuncTest, MountainAtBeginningIsHandledCorrectly) {
    const std::vector<float> data{
        10, 10, 10, 10, 10,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    EXPECT_EQ(RunFindFunc(data, 4, 10,true), "0 5\n");
}

TEST(FindFuncTest, MountainAtEndIsHandledCorrectly) {
    const std::vector<float> data{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        10, 10, 10, 10, 10
    };

    EXPECT_EQ(RunFindFunc(data, 5, 10,true), "9 14\n");
}

TEST(FindFuncTest, LowThresholdExpandsMountainBeyondHighThreshold) {
    const std::vector<float> data{
        0, 1, 1, 1, 2,
        5, 5, 5, 5,
        0, 1, 1, 1, 2
    };

    EXPECT_EQ(RunFindFunc(data, 5, 8,true), "4 9\n");
}

TEST(FindFuncTest, ValueExactlyAtHighIsNotAMountain) {
    const std::vector<float> data{
        0, 1, 1, 1, 1,
        1,
        0, 1, 1, 1, 1
    };

    EXPECT_TRUE(RunFindFunc(data, 1, 100,true).empty());
}

TEST(FindFuncTest, DifferentMount) {
    std::fstream infile("..\\resource\\detector_src_32f.bin", std::ios::in | std::ios::binary);

    infile.seekg(0, std::ios::end);
    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<float> data(size/sizeof(float));

    infile.read(reinterpret_cast<char*>(data.data()), size);
    infile.close();

    std::string reference = RunFindFunc(data, 3, 1000, false);
    std::string actual = RunFindFunc(data, 3, 1000, true);

    EXPECT_NE(actual, reference);
}

TEST(FindFuncTest, CountOfMountActual) {
    std::fstream infile("..\\resource\\detector_src_32f.bin", std::ios::in | std::ios::binary);

    infile.seekg(0, std::ios::end);
    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<float> data(size/sizeof(float));

    infile.read(reinterpret_cast<char*>(data.data()), size);
    infile.close();

    std::string temp = RunFindFunc(data, 3, 1000, true);
    int n = std::count(temp.begin(), temp.end(), '\n');

    EXPECT_EQ(n, 3527);
}

TEST(FindFuncTest, BorderOfEachMount) {
    std::fstream infile("..\\resource\\detector_src_32f.bin", std::ios::in | std::ios::binary);

    infile.seekg(0, std::ios::end);
    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<float> data(size/sizeof(float));

    infile.read(reinterpret_cast<char*>(data.data()), size);
    infile.close();

    std::fstream refFile("..\\resource\\ref.txt", std::ios::in | std::ios::binary);

    refFile.seekg(0, std::ios::end);
    std::streamsize n = refFile.tellg();
    refFile.seekg(0, std::ios::beg);

    std::string ref; ref.resize(n);
    refFile.read(&ref[0], n);
    ref.erase(std::remove(ref.begin(), ref.end(), '\r'), ref.end());

    std::string temp = RunFindFunc(data, 3, 1000, true);

    EXPECT_EQ(ref.size(), temp.size());
    EXPECT_EQ(ref.compare(temp), 0);
}