#pragma once
#include <vector>
#include "ScoreRecord.h"

// Enumeración para seleccionar el algoritmo de ordenamiento
enum class SortAlgorithm {
    BUBBLE_SORT,
    QUICK_SORT
};

// Interfaz base para los algoritmos de ordenamiento
class Sorter {
public:
    virtual ~Sorter() = default;
    virtual void sort(std::vector<ScoreRecord>& records) = 0;
};

// Algoritmo O(n^2): Ordenamiento Burbuja
class BubbleSorter : public Sorter {
public:
    void sort(std::vector<ScoreRecord>& records) override;
};

// Algoritmo O(n log n): QuickSort
class QuickSorter : public Sorter {
private:
    void quickSort(std::vector<ScoreRecord>& records, int low, int high);
    int partition(std::vector<ScoreRecord>& records, int low, int high);
public:
    void sort(std::vector<ScoreRecord>& records) override;
};