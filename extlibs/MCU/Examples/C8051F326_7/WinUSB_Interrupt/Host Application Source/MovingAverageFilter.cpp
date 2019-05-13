///////////////////////////////////////////////////////////////////////////////
// MovingAverageFilter.cpp
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MovingAverageFilter.h"

///////////////////////////////////////////////////////////////////////////////
// Namespaces
///////////////////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// CMovingAverageFilter Class - Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////

CMovingAverageFilter::CMovingAverageFilter(int numSamples)
{
    this->m_numSamples = numSamples;
}

CMovingAverageFilter::~CMovingAverageFilter(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// CMovingAverageFilter Class - Public Methods
///////////////////////////////////////////////////////////////////////////////

// Add a sample to the moving average,
// returning the average
double CMovingAverageFilter::Add(double sample)
{
    // Add the sample to the list
    m_samples.push_back(sample);

    // Shrink the list down to numSamples size
    if ((int)m_samples.size() > m_numSamples)
    {
        m_samples.erase(m_samples.begin(), m_samples.begin() + (m_samples.size() - m_numSamples));
    }

    // Return the moving average
    return Average();
}

// Return the moving average value
double CMovingAverageFilter::Average()
{
    double sum = 0;
    double average = 0;

    if (m_samples.size() > 0)
    {
        for (size_t i = 0; i < m_samples.size(); i++)
        {
            sum += m_samples[i];
        }

        average = (double)(sum / m_samples.size());
    }

    return average;
}
        
// Reset the list of samples
void CMovingAverageFilter::Reset()
{
    m_samples.clear();
}
