/*
  ==============================================================================

    SynthlabAudioProcessor.h
    Created: 18 May 2018 10:06:16am
    Author:  Matthias Pueski

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthEditor.h"

class Module;
class SynthEditor;
class Sampler;
class Mixer;
class SynthlabAudioProcessor {
    
public:
    
    SynthlabAudioProcessor(Project* p, Mixer* mixer);
    virtual ~SynthlabAudioProcessor() ;
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    void handleIncomingMidiMessage (juce::MidiInput* source, const juce::MidiMessage& message, Module* root);
    void sendGateMessage(Module* module, int channel, int velocity,int note,bool on);
    void sendNoteMessage(Module* module, int channel, int note);
    void sendClockMessage(Module* module, int channel);
    void sendMidiStartMessage(Module* module, int channel);
    void sendMidiStopMessage(Module* module, int channel);
    void sendControllerMessage(Module* module, int channel, int controller, float value);
    void sendPitchBendMessage(Module *module, int channel, int pitch);
    void processModule(Module* m);
    
    void refreshMidiInputs();
    void enableAllMidiInputs();
    void disableAllMidiInputs();
    
    int getNumActiveChannels(int i);
    int getCpuLoad();
    
    void prepareModule(Module* m, float sampleRate, int bufferSize);

    std::vector<AudioOut*> outputChannels;
    std::vector<AudioIn*> inputChannels;
    std::vector<AuxOut*> auxChannels;
    
    SynthEditor* editor;
    Project* project;
    volatile bool running = true;
private:
    long currentTime = 0;
    long lastTime = 0;

    Sampler* defaultSampler = nullptr;
    Mixer* mixer = nullptr;
    
    int cpuLoad = 0;
    int currentSample = 0;
    
    // default settings just in case
    float sampleRate;
    float buffersize;
      
    
};
