/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "ValentineParameters.h"
//==============================================================================
ValentineAudioProcessorEditor::ValentineAudioProcessorEditor (ValentineAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , processor (p)
{
    addAndMakeVisible (mainPanel);

    auto w = 0.0f;

    if (auto savedWidth = processor.getSavedGUIwidth())
        w = savedWidth;
    else
        w = startingWidth;

    setResizable (true, true);
    setResizeLimits (minimumWidth,
                     minimumWidth / ratio,
                     maximumWidth,
                     maximumWidth / ratio);
    getConstrainer()->setFixedAspectRatio (ratio);

    setSize (w, w / ratio);
}

ValentineAudioProcessorEditor::~ValentineAudioProcessorEditor()
{
    processor.saveGUIwidth (getWidth());
}

//==============================================================================
void ValentineAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void ValentineAudioProcessorEditor::resized()
{
    mainPanel.setBounds (getLocalBounds());
}
