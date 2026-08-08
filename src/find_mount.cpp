#include "find_mount.h"
#include "find_const.h"
#include<cmath>

void find_func(std::fstream& outfile, std::vector<float>& data, int min, int max) {
    size_t n = data.size();
    if (n == 0) return;

    auto [median, sigma] = calc_const(data);

    size_t half = 2, win = 2 * half + 1;
    std::vector<double> pref_sum(n + 1, 0.0);
    for (size_t i = 0; i < n; ++i) pref_sum[i + 1] = pref_sum[i] + data[i];
    auto avg = [&](size_t l, size_t r) {
        return (pref_sum[r + 1] - pref_sum[l]) / double(r + 1 - l);
    };
    std::vector<float> s(n);
    for (size_t i = 0; i < n; ++i)
        s[i] = float(avg(i > half ? i - half : 0,
                         i + half < n ? i + half : n - 1));

    float high = median + 2.0f * sigma,
        low  = median + sigma / std::sqrt(float(win));

    size_t i = 0;
    while (i < n) {
        if (s[i] > high) {
            size_t left = i, right = i;
            while (right + 1 < n && s[right + 1] > low) ++right;
            while (left > 0 && s[left  - 1] > low) --left;

            unsigned int width = right - left;
            if (width >= min && width <= max)
                outfile << left << ' ' << right << '\n';

            i = right + 1;
        } else {
            ++i;
        }
    }
}
