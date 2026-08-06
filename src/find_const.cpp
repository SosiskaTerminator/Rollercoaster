#include "quicksort.h"
#include "find_const.h"
#include <math.h>

std::pair<float, float> calc_const(std::vector<float> data) {
    QuickSort(data, 0, data.size() - 1);

    std::pair<float, float> median_sigma;

    median_sigma.first = (data.size() % 2 == 1) ? data[data.size() / 2] : (data[data.size() / 2] + data[data.size() / 2 - 1]) / 2;

    double sr = 0;

    for (float i : data) {
        sr += pow(i - median_sigma.first, 2);
    }

    median_sigma.second = pow(sr/(data.size() - 1), 0.5);

    return median_sigma;
}