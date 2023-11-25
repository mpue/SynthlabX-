/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainComponent.h"
using namespace juce;
//==============================================================================
/**
*/
class SynthlabXAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthlabXAudioProcessorEditor (SynthlabXAudioProcessor&, Project* project, Mixer* mixer);
    ~SynthlabXAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override;
    MainComponent* mainComponent = nullptr;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SynthlabXAudioProcessor& audioProcessor;
    Project* project;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthlabXAudioProcessorEditor)
};
