/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiRecorder.h"
#include "Logger.h"
#include "RecordListener.h"
#include "MidiEventEditor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiRecorderPanel  : public juce::Component,
                           public MidiTools::Logger,
                           public RecordListener,
                           public juce::Button::Listener
{
public:
    //==============================================================================
    MidiRecorderPanel ();
    ~MidiRecorderPanel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    MidiRecorder* getRecorder();
    juce::TextEditor* getTextEditor();
    void log(juce::String message) override;
    void setDeviceManager(juce::AudioDeviceManager* manager);
    void changedState(StateChange* change) override;
    void incomingMessage(juce::MidiMessage* message) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* led_off_png;
    static const int led_off_pngSize;
    static const char* led_on_png;
    static const int led_on_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiRecorder* recorder;
    MidiEventEditor* editor;
    //[/UserVariables]

    //==============================================================================
    juce::ScopedPointer<juce::TextButton> recordButton;
    juce::ScopedPointer<juce::TextButton> playButton;
    juce::ScopedPointer<juce::TextEditor> textEditor;
    juce::ScopedPointer<juce::TextButton> clearButton;
    juce::ScopedPointer<juce::ImageButton> midiButton;
    juce::ScopedPointer<juce::TextButton> saveButton;
    juce::ScopedPointer<juce::TextButton> editButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiRecorderPanel)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
