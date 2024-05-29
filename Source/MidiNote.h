/*
  ==============================================================================

    MidiGate.h
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Modules/Module.h"

//==============================================================================
/*
*/
class MidiNote    : public Module
{
public:
    MidiNote();
    ~MidiNote();

    void paint (juce::Graphics& g) override;

    void note(int note);

    virtual void configurePins() override;
    
    
    virtual juce::String getCategory() override {
        return "MIDI";
    }
    
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiNote)
};
