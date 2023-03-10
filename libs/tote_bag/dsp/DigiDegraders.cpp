/*
  ==============================================================================

    SimpleZOH.cpp
    Created: 2 Sep 2019 10:40:50am
    Author:  dev

  ==============================================================================
*/

#include "DigiDegraders.h"
#include "tote_bag/dsp/AudioHelpers.h"

void SimpleZOH::setResampleOffset (double inHostSr)
{
    /*
        this allows the ZOH to act the same regardless of
        host SR...as long as RefSr is 44100 (most times the lowest possible)
        if host sr is larger than refSR, resample coeff will be increased
        by the ratio between the two. So if ref is 44100 and session is 88200,
        a selected downsample ratio of 2 will result in a downsample ratio of 4
     */
    auto currentSR = currentHostSampleRate.get();

    if (inHostSr != currentSR)
    {
        resampleOffset = 1.0f;
    }
}

void SimpleZOH::setParams (float inDownsampleCoeff, bool sampleRateChanged)
{
    downsampleCoeff = inDownsampleCoeff;
}

void SimpleZOH::processBlock (juce::AudioBuffer<float>& inAudio, int samplesPerBlock, int numChannels)
{
    auto intDownSampleCoeff = static_cast<int> (downsampleCoeff);
    auto frac = downsampleCoeff - intDownSampleCoeff;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto channelData = inAudio.getWritePointer (channel);

        for (int sample = 0; sample < samplesPerBlock; ++sample)
        {
            auto y1 = getZOHSample (channelData, sample, intDownSampleCoeff);
            auto y2 = getZOHSample (channelData, sample, intDownSampleCoeff + 1);
            channelData[sample] = tote_bag::audio_helpers::linearInterp (y1, y2, frac);
        }
    }
}

inline float SimpleZOH::getZOHSample (const float* channelData, int sampleIndex, int dsCoef)
{
    if (dsCoef <= 1)
        return channelData[sampleIndex];

    auto remainder = sampleIndex % dsCoef;

    return remainder == 0 ? channelData[sampleIndex] : channelData[sampleIndex - remainder];
}

//=====================================================================================

void Bitcrusher::setParams (float inBitDepth)
{
    bitDepth.set (inBitDepth);
}

void Bitcrusher::processBlock (juce::AudioBuffer<float>& inAudio, int samplesPerBlock, int numChannels)
{
    auto bD = bitDepth.get();
    auto intBitDepth = static_cast<int> (bD);
    auto frac = bD - intBitDepth;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        auto channelData = inAudio.getWritePointer (channel);

        for (int sample = 0; sample < samplesPerBlock; ++sample)
        {
            const float inputSample = channelData[sample];
            auto y1 = getBitcrushedSample (inputSample, intBitDepth);
            auto y2 = getBitcrushedSample (inputSample, intBitDepth + 1);

            channelData[sample] = tote_bag::audio_helpers::linearInterp (y1, y2, frac);
        }
    }
}

inline float Bitcrusher::getBitcrushedSample (float inputSample, int bits)
{
    auto q = 1.0f / (pow (2.0f, bits) - 1);

    return q * (floor (inputSample / q));
}
