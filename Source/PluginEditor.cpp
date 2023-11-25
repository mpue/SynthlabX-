/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "CustomLookAndFeel.h"


//==============================================================================
SynthlabXAudioProcessorEditor::SynthlabXAudioProcessorEditor (SynthlabXAudioProcessor& p, Project* project, Mixer* mixer)
    : AudioProcessorEditor (&p), audioProcessor (p), project(project)
{
    LookAndFeel::setDefaultLookAndFeel(new CustomLookAndFeel());
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1280, 800);
    setResizable(true, true);
    mainComponent = new MainComponent(project, mixer);
    addAndMakeVisible(mainComponent);
    
}

void SynthlabXAudioProcessorEditor::visibilityChanged() {

    String appDataPath = File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName();
    String stateFilePath = appDataPath + "/state";
    juce::File stateFile = juce::File(stateFilePath);

    if (!isVisible()) {

        FileOutputStream outputStream(stateFile);

        if (!outputStream.openedOk()) {
            DBG("Failed to open file for writing");
            return;
        }

        outputStream.writeText(project->getCurrentFilePath(), false, false, "\n");
        outputStream.flush(); // Optional, but ensures all data is written to the file

        if (!outputStream.getStatus().wasOk()) {
            DBG("Failed to write to file");
        }
        else {
            DBG("File written successfully");
        }


    }
    else {
        if (!stateFile.exists()) {
            DBG("Failed does not exist");
            return;
        }

        FileInputStream inputStream(stateFile);

        if (!inputStream.openedOk()) {
            DBG("Failed to open file for reading");            
        }

        String target = inputStream.readEntireStreamAsString();

        if (target.length() > 0) {
            mainComponent->getEditor()->openFile(target);
        }
            

    }
}

SynthlabXAudioProcessorEditor::~SynthlabXAudioProcessorEditor()
{
    // mainComponent = nullptr;
}

//==============================================================================
void SynthlabXAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SynthlabXAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
