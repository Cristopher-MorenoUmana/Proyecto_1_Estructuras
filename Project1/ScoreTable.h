#pragma once
#include <vector>
#include <string>
#include <memory>
#include "ScoreRecord.h"
#include "Sorter.h"

class ScoreTable {
private:
    std::vector<ScoreRecord> records;
    std::string filename;
    std::unique_ptr<Sorter> sorter;

public:
    ScoreTable(const std::string& file = "highscores.txt");

    void setAlgorithm(SortAlgorithm algo);
    void loadFromFile();
    void saveToFile() const;

    bool isHighScore(int score) const;
    void addScore(const std::string& name, int score);
    void sort();

    const std::vector<ScoreRecord>& getRecords() const { return records; }
};