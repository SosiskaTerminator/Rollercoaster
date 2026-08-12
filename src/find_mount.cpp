#include "find_mount.h"
#include "find_const.h"
#include<cmath>

void reference(std::fstream& outfile, std::vector<float>& data, int min, int max) {
    size_t n = data.size();
    if (n == 0) return;

    auto [median, sigma] = calc_const(data);

    float high = median + 2.0f * sigma,
        low  = median + sigma / 2;

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

void find_mount_func(std::fstream& outfile, std::vector<float>& data, int min, int max) {
    size_t n = data.size();
    if (n == 0) return;

    auto [median, mad] = calc_const(data);
    const float sigma = 1.4826 * mad;

    std::vector<double> pref(n + 1, 0.0);
    for (int i = 0; i < n; i++) { pref[i + 1] = pref[i] + data[i]; }

    auto avg = [&](unsigned left, unsigned rigth) -> double {
        return (pref[rigth + 1] - pref[left]) / double(rigth - left + 1);
    };

    const int half = 1;
    std::vector<float> smoothedData(n);
    for (int i = 0; i < n; i++) {
        smoothedData[i] = float(avg(i > half ? i - half:0, i + half < n ? i + half:n - 1));
    }

    const float high = median + sigma * 1.5, low = median + sigma / 2;

    size_t i = 0;
    while (i < n) {
        if (smoothedData[i] > high) {
            size_t left = i, right = i;
            while (right + 1 < n && smoothedData[right + 1] > low) ++right;
            while (left > 0 && smoothedData[left  - 1] > low) --left;

            unsigned int width = right - left + 1;
            if (width >= min && width <= max)
                outfile << left << ' ' << right << '\n';

            i = right + 1;
        } else {
            ++i;
        }
    }
}