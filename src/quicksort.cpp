#include "quicksort.h"
#include <iostream>

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