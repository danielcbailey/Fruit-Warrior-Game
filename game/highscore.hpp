

#ifndef HIGH_SCORE_INCLUDED
#define HIGH_SCORE_INCLUDED

#include <string>
#include <list>
#include "util/csv.hpp"

class HighScores {
    private:

    static CSVParser* csv;

    public:

    typedef struct {
        std::string name;
        int score;
    } HighScore;

    static void initHighScores();

    //if -1 that means it did not make the top 5
    static int checkScoreIndex(int score);

    static std::list<HighScore> getTopFive();

    static void insertScore(std::string name, int score);
};

#endif //HIGH_SCORE_INCLUDED