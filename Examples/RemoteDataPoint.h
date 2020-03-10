#ifndef _REMOTE_DATA_POINT_H
#define _REMOTE_DATA_POINT_H

#include <sstream>

/// @brief Shared data structure to send between remote systems. 
/// User defined remote delegate data arguments must have:
/// 1) A default constructor
/// 2) Define operator<< and operator>> functions for the type passed
struct RemoteDataPoint
{
public:
    RemoteDataPoint(int x, int y) : m_x(x), m_y(y) {}
    RemoteDataPoint() : m_x(0), m_y(0) {}
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

private:
    int m_y;
    int m_x;

    // Insert or extract any stream any type of RemoteDataPoint. You normally only 
    // need to define one insertion and one extraction function. These example just
    // shows all possible combinations (reference, pointer, ...).
    friend std::ostream& operator<< (std::ostream &out, const RemoteDataPoint& data);
    friend std::ostream& operator<< (std::ostream &out, const RemoteDataPoint* data);
    friend std::ostream& operator<< (std::ostream &out, RemoteDataPoint** data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPoint& data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPoint* data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPoint** data);
};

#endif