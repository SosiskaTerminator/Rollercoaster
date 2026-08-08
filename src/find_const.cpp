#include "find_const.h"
#include <cmath>
#include <algorithm>

std::pair<float, float> calc_const(std::vector<float> data) {
    sort(data.begin(), data.end());

    std::pair<float, float> median_sigma;

    median_sigma.first = (data.size() % 2 == 1) ? data[data.size() / 2] : (data[data.size() / 2] + data[data.size() / 2 - 1]) / 2;

    std::vector<float> temp;

    for (float i : data) {
        temp.push_back(std::abs(i - median_sigma.first));
    }

    sort(temp.begin(), temp.end());

    median_sigma.second = (temp.size() % 2 == 1) ? temp[temp.size() / 2] : (temp[temp.size() / 2] + temp[temp.size() / 2 - 1]) / 2;

    return median_sigma;
}