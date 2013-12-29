
#ifndef _POSITION_H_
#define _POSITION_H_

#include "bbvector.h"

class Position {
    public:
        Position(Vector vector, unsigned short pos_rank, unsigned short vect_rank, unsigned short trend_rank);
        Position();
        ~Position();

    private:
        Vector position;

        unsigned short positionRanking;
        unsigned short vectorRanking;
        unsigned short trendRanking;

    public:
        unsigned short getPositionRanking();
        unsigned short getVectorRanking();
        unsigned short getTrendRanking();

        void setPositionRanking(unsigned short position);
        void setVectorRanking(unsigned short vector);
        void setTrendRanking(unsigned short trend);

        void getRankings(unsigned short* position, unsigned short* vector, unsigned short* trend);
        void setRankings(unsigned short position, unsigned short vector, unsigned short trend);

        Vector getVector();
        void setVector(Vector vector);
};

#endif
