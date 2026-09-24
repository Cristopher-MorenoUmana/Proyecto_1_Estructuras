#include "ScoreTable.h"
#include <fstream>
#include <iostream>

ScoreTable::ScoreTable(const std::string& file)
    : filename(file), sorter(std::make_unique<QuickSorter>())
{
    loadFromFile();
}

void ScoreTable::setAlgorithm(SortAlgorithm algo) {
    if (algo == SortAlgorithm::BUBBLE_SORT) {
        sorter = std::make_unique<BubbleSorter>();
    }
    else {
        sorter = std::make_unique<QuickSorter>();
    }
}

void ScoreTable::loadFromFile() {
    records.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    ScoreRecord rec;
    while (file >> rec.name >> rec.score) {
        records.push_back(rec);
    }
    file.close();
    sort();
}

void ScoreTable::saveToFile() const {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& rec : records) {
        file << rec.name << " " << rec.score << "\n";
    }
    file.close();
}

void ScoreTable::sort() {
    if (sorter) {
        sorter->sort(records);
    }
}

bool ScoreTable::isHighScore(int score) const {
    if (score <= 0) return false;
    if (records.size() < 10) return true;
    return score > records.back().score;
}

void ScoreTable::addScore(const std::string& name, int score) {
    records.push_back({ name, score });
    sort();

    // Mantener únicamente el Top 10
    if (records.size() > 10) {
        records.pop_back();
    }

    saveToFile();
}