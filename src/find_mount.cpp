#include "find_mount.h"
#include "find_const.h"
#include<cmath>

void find_func(std::fstream& outfile, std::vector<float>& data, int min, int max) {
    size_t n = data.size();
    if (n == 0) return;

    auto [median, sigma] = calc_const(data);

    float high = median + 2.0f * sigma,
        low  = median - sigma / 2;

    size_t i = 0;
    while (i < n) {
        if (data[i] > high) {
            size_t left = i, right = i;
            while (right + 1 < n && data[right + 1] > low) ++right;
            while (left > 0 && data[left  - 1] > low) --left;

            unsigned int width = right - left + 1;
            if (width >= min && width <= max)
                outfile << left << ' ' << right << '\n';

            i = right + 1;
        } else {
            ++i;
        }
    }
}
