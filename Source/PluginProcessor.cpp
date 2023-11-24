/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthEditor.h"

//==============================================================================
SynthlabXAudioProcessor::SynthlabXAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	project = new Project();


}

SynthlabXAudioProcessor::~SynthlabXAudioProcessor()
{
	// audioProcessor = nullptr;
}

//==============================================================================
const juce::String SynthlabXAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool SynthlabXAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SynthlabXAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SynthlabXAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SynthlabXAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int SynthlabXAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int SynthlabXAudioProcessor::getCurrentProgram()
{
	return 0;
}

void SynthlabXAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String SynthlabXAudioProcessor::getProgramName(int index)
{
	return {};
}

void SynthlabXAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void SynthlabXAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..	
	audioProcessor = new SynthlabAudioProcessor(sampleRate, samplesPerBlock, project);
	audioProcessor->prepareToPlay(sampleRate, samplesPerBlock);
}

void SynthlabXAudioProcessor::releaseResources()
{
	audioProcessor->running = false;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthlabXAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void SynthlabXAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	audioProcessor->processBlock(buffer, midiMessages);
}

//==============================================================================
bool SynthlabXAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthlabXAudioProcessor::createEditor()
{
	synthEditor = new SynthlabXAudioProcessorEditor(*this, project);
	
	return synthEditor;
}

//==============================================================================
void SynthlabXAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void SynthlabXAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SynthlabXAudioProcessor();
}
