#include <iostream>
#include <fstream>
#include "find_const.h"

/*
1) найти медиану данных ✓
2) найти среднеквадратичное отклонение от медианы (среднеквадратичное = 8.40942) ✓
3) на основе это получить горки и просто шум
    3.1) функции для смещение границ горки ближе к "полу"
4) просто вывод
*/

using namespace std;

int main() {
    fstream file("..\\resource\\detector_src_32f.bin", ios::in | ios::binary);

    if (!file.is_open()) {
        cout << "Wrong path" << endl;

        return 1;
    }

    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<float> data(size/sizeof(float));

    file.read(reinterpret_cast<char*>(data.data()), size);

    auto [median, sigma] = calc_const(data);

    return 0;
}