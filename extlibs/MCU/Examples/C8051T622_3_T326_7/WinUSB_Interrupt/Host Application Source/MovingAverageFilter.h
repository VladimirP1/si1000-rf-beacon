///////////////////////////////////////////////////////////////////////////////
// MovingAverageFilter.h
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include <vector>

///////////////////////////////////////////////////////////////////////////////
// CMovingAverageFilter Class
///////////////////////////////////////////////////////////////////////////////

class CMovingAverageFilter
{
// Constructor/Destructor
public:
    CMovingAverageFilter(int numSamples);
    ~CMovingAverageFilter(void);

// Public Methods
public:
    double Add(double sample);
    double Average();
    void Reset();

// Private Members
private:
    int                 m_numSamples;
    std::vector<double> m_samples;
};
