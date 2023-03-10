/*
  ==============================================================================

    AudioHelpers.h
    Created: 2 Sep 2019 10:57:34am
    Author:  dev

  ==============================================================================
*/

#pragma once

#include <cmath>

namespace tote_bag
{
namespace audio_helpers
{

inline float linearInterp (float x1, float x2, float frac)
{
    return (1.0f - frac) * x1 + frac * x2;
}

template <typename T>
inline void checkUnderflow (T& x)
{
    if (abs (x) < std::numeric_limits<T>::epsilon())
        x = 0.0f;
}

template <typename T>
class SimpleOnePole
{
public:
    void reset() { prevValue = 0.0; }

    void prepare (T newCoeff) { coeff = newCoeff; }

    T processSample (T inputSample)
    {
        // whoops! looks like you forgot to set the coefficient
        assert (coeff != -1);

        auto output = coeff * (prevValue - inputSample) + inputSample;
        prevValue = output;
        return output;
    }

private:
    T prevValue, coeff = -1;
};

template <typename FloatType>
FloatType max (FloatType x, FloatType y)
{
    x -= y;
    x += abs (x);
    x *= 0.5;
    x += y;
    return (x);
}

template <typename FloatType>
FloatType min (FloatType x, FloatType y)
{
    x = y - x;
    x += abs (x);
    x *= 0.5;
    x = y - x;
    return (x);
}

/** a fast clipping algorithm source: https://www.musicdsp.org/en/latest/Other/81-clipping-without-branching.html
    it's noted in the description that it is not so accurate for small numbers
*/
template <typename FloatType>
FloatType fastClip (FloatType x, FloatType minimumValue, FloatType maximumValue)
{
    const FloatType x1 = abs (x - minimumValue);
    const FloatType x2 = abs (x - maximumValue);
    x = x1 + (minimumValue + maximumValue);
    x -= x2;
    x *= 0.5;
    return (x);
}

/** Returns cosh(x), clamping input beforehand to prevent overflow.
    The bounds used are found by brute force: e.g. running std::cosh with values
    increasing until overflow occurs. I am assuming here that this is well
    above the values we would expect to see in "reasonable" use, but
    it has yet to be verified. A more sophisticated approach may be needed
    if aliasing becomes an issue.
 */
template <typename FloatType>
inline FloatType ClampedCosh (FloatType x)
{
    static constexpr FloatType coshMin = -710.0;
    static constexpr FloatType coshMax = 710.0;

    return std::cosh (std::clamp (x, coshMin, coshMax));
}

} // namespace audio_helpers
} // namespace tote_bag
