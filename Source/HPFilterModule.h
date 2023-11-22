/*
  ==============================================================================

    HighPassFilter
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Module.h"

class HighPassFilter;

//==============================================================================
/*
*/
class HPFilterModule : public Module
{
public:
    HPFilterModule(double sampleRate, int buffersize);
    ~HPFilterModule();

    void paint (juce::Graphics& g) override;

    void process() override;
    virtual void configurePins() override;
    virtual juce::String getCategory() override {
        return "Filter";
    }
    
    virtual juce::String getDescription() override {
        return "Replace me!!!";
    }

private:
    
    HighPassFilter* filter;

    juce::Image* image;
    int currentSample = 0;
    
    float frequency = 10000;
    float resonance = 1;
    float mod = 1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HPFilterModule)
};
