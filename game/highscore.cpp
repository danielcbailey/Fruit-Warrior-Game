

#include "highscore.hpp"
#include "core/files.hpp"

CSVParser* HighScores::csv;

void HighScores::initHighScores() {
    File* f = new File(_globalFileSystem, "highScores.csv", O_RDWR);
    csv = new CSVParser(f);
    //since file will be used by csv parser, it will never be deleted (or closed)
}

int HighScores::checkScoreIndex(int score) {
    auto top = getTopFive();

    int i = 0;
    for (auto it = top.begin(); it != top.end(); ++it) {
        if (score > it->score) {
            return i;
        }
        i++;
    }

    return -1;
}

std::list<HighScores::HighScore> HighScores::getTopFive() {
    std::list<HighScore> ret;

    //the csv file should have a header, but won't use it for operations
    for (int i = 1; 5 >= i; i++) {
        ret.push_back({csv->getString(i, 0), csv->getInt(i, 1)});
    }

    return ret;
}

void HighScores::insertScore(std::string name, int score) {
    auto top = getTopFive();

    for (auto it = top.begin(); it != top.end(); ++it) {
        if (score > it->score) {
            top.insert(it, {name, score});
            break;
        }
    }

    if (top.size() > 5) {
        top.pop_back();
    }

    //saving the contents to the csv
    int i = 1;
    for (auto it = top.begin(); it != top.end(); ++it) {
        //csv->writeString(i, 0, it->name);
        //csv->writeInt(i, 1, it->score);
        i++;
    }
}