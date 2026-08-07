#include <iostream>
#include <fstream>
#include <string>
#include "find_mount.h"

/*
1) найти медиану данных ✓
2) найти mad от медианы ✓ median +- sigma*2
3) на основе это получить горки и просто шум
    3.1) функции для смещение границ горки ближе к "полу"
4) просто вывод
*/

using namespace std;

int main() {
    cout << "Write the full path to the .bin-file containing the data to be read: ";
    string infilePath;
    getline(cin, infilePath);

    fstream infile(infilePath, ios::in | ios::binary);

    if (!infile.is_open()) {
        cout << "Wrong path " << endl;

        return 1;
    }

    cout << "Write the full path to the file containing the data to be written: ";
    string outfilePath;
    getline(cin, outfilePath);
    
    fstream outfile(outfilePath, ios::out | ios::trunc);

    cout << "Write the minimum and maximum length separated by a space: ";
    int min, max; cin >> min >> max;

    infile.seekg(0, ios::end);
    streamsize size = infile.tellg();
    infile.seekg(0, ios::beg);

    vector<float> data(size/sizeof(float));

    infile.read(reinterpret_cast<char*>(data.data()), size);

    return 0;
}