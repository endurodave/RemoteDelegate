#ifndef _REMOTE_DATA_POINT_BINARY_H
#define _REMOTE_DATA_POINT_BINARY_H

#include <sstream>

/// @brief Shared data structure to send between remote systems. 
/// User defined remote delegate data arguments must have:
/// 1) A default constructor
/// 2) Define operator<< and operator>> functions for the type passed
struct RemoteDataPointBinary
{
public:
    RemoteDataPointBinary(int x, int y) : m_x(x), m_y(y) {}
    RemoteDataPointBinary() : m_x(0), m_y(0) {}
    int GetX() const { return m_x; }
    int GetY() const { return m_y; }

private:
    int m_x;
    int m_y;

    // Insert or extract any stream any type of RemoteDataPoint. You normally only 
    // need to define one insertion and one extraction function. These example just
    // shows all possible combinations (reference, pointer, ...).
    friend std::ostream& operator<< (std::ostream &out, const RemoteDataPointBinary& data);
    friend std::ostream& operator<< (std::ostream &out, const RemoteDataPointBinary* data);
    friend std::ostream& operator<< (std::ostream &out, RemoteDataPointBinary** data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPointBinary& data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPointBinary* data);
    friend std::istream& operator>> (std::istream &in, RemoteDataPointBinary** data);
};

#endif