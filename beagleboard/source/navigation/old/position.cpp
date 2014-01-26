
#include "position.h"

Position::Position(Vector vector, unsigned short pos_rank, unsigned short vect_rank, unsigned short trend_rank) {
    position = vector;
    positionRanking = pos_rank;
    vectorRanking = vect_rank;
    trendRanking = trend_rank;
}

Position::Position() {
    position = Vector();
    positionRanking = 0;
    vectorRanking = 0;
    trendRanking = 0;
}

Position::~Position() {}

unsigned short Position::getPositionRanking() { return positionRanking; }
unsigned short Position::getVectorRanking() { return vectorRanking; }
unsigned short Position::getTrendRanking() { return trendRanking; }

void Position::setPositionRanking(unsigned short position) { positionRanking = position; }
void Position::setVectorRanking(unsigned short vector) { vectorRanking = vector; }
void Position::setTrendRanking(unsigned short trend) { trendRanking = trend; }

void Position::getRankings(unsigned short* position, unsigned short* vector, unsigned short* trend) {
    *position = positionRanking;
    *vector = vectorRanking;
    *trend = trendRanking;

    return;
}

void Position::setRankings(unsigned short position, unsigned short vector, unsigned short trend) {
    positionRanking = position;
    vectorRanking = vector;
    trendRanking = trend;
}

Vector Position::getVector() { return position; }
void Position::setVector(Vector vector) { position = vector; }

