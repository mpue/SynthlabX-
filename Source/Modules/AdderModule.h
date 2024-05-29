/*
  ==============================================================================

    MidiGate.h
    Created: 4 Apr 2018 3:48:23pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Module.h"

//==============================================================================
/*
*/
class AdderModule    : public Module
{
public:
    AdderModule(int buffersize);
    ~AdderModule();
   
    
    void paint (juce::Graphics& g) override;
    virtual void process() override;
    virtual void configurePins() override;
    
    virtual juce::String getCategory() override {
        return "Math";
    }
    
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdderModule)
};
