#include "find_const.h"
#include <cmath>

void QuickSort(std::vector<float>& data, int left, int right) {
    if (left >= right) return;

    float pivot = data[(left + right) / 2];
    int i = left, j = right;

    while (i <= j) {
        while (data[i] < pivot) i++;
        while (data[j] > pivot) j--;
        if (i <= j) {
            std::swap(data[i], data[j]);
            i++;
            j--;
        }
    }

    QuickSort(data, left, j);
    QuickSort(data, i, right);
}

std::pair<float, float> calc_const(std::vector<float> data) {
    QuickSort(data, 0, data.size() - 1);

    std::pair<float, float> median_sigma;

    median_sigma.first = (data.size() % 2 == 1) ? data[data.size() / 2] : (data[data.size() / 2] + data[data.size() / 2 - 1]) / 2;

    std::vector<float> temp;

    for (float i : data) {
        temp.push_back(std::abs(i - median_sigma.first));
    }

    QuickSort(temp, 0, temp.size() - 1);

    median_sigma.second = (temp.size() % 2 == 1) ? temp[temp.size() / 2] : (temp[temp.size() / 2] + temp[temp.size() / 2 - 1]) / 2;

    return median_sigma;
}