#include "Sorter.h"
#include <utility>

// Implementación de Bubble Sort (O(n^2)) - Orden Descendente
void BubbleSorter::sort(std::vector<ScoreRecord>& records) {
    size_t n = records.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (records[j].score < records[j + 1].score) {
                std::swap(records[j], records[j + 1]);
            }
        }
    }
}

// Implementación de QuickSort (O(n log n)) - Orden Descendente
void QuickSorter::sort(std::vector<ScoreRecord>& records) {
    if (!records.empty()) {
        quickSort(records, 0, static_cast<int>(records.size()) - 1);
    }
}

void QuickSorter::quickSort(std::vector<ScoreRecord>& records, int low, int high) {
    if (low < high) {
        int pi = partition(records, low, high);
        quickSort(records, low, pi - 1);
        quickSort(records, pi + 1, high);
    }
}

int QuickSorter::partition(std::vector<ScoreRecord>& records, int low, int high) {
    int pivot = records[high].score;
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        // Para orden descendente buscamos valores mayores al pivote
        if (records[j].score > pivot) {
            i++;
            std::swap(records[i], records[j]);
        }
    }
    std::swap(records[i + 1], records[high]);
    return i + 1;
}