/*
 ==============================================================================
 
 MidiGate.cpp
 Created: 4 Apr 2018 3:48:23pm
 Author:  Matthias Pueski
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "AuxOut.h"
#include "Connection.h"

using juce::Justification;
using juce::ImageCache;

//==============================================================================
AuxOut::AuxOut()
{
    setSize(120,140);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(18,2);
    setName("Aux out");
    
    editable = false;
    prefab = true;
    
    createProperties();
}

AuxOut::~AuxOut()
{
    delete volumeValue;
    delete volumeListener;
}

void AuxOut::createProperties() {
    volumeValue = new Value();
    volumeListener = new ValueListener(*volumeValue, this);
}

void AuxOut::configurePins() {
    Pin* p1 = new Pin(Pin::Type::AUDIO);
    p1->direction = Pin::Direction::IN;
    p1->setName("L");
    
    Pin* p2 = new Pin(Pin::Type::AUDIO);
    p2->direction = Pin::Direction::IN;
    p2->setName("R");
    
    addPin(Pin::Direction::IN,p1);
    addPin(Pin::Direction::IN,p2);
    
}

void AuxOut::paint(juce::Graphics &g) {
    Module::paint(g);
    g.drawImageAt(ImageCache::getFromMemory(BinaryData::auxmodule_png, BinaryData::auxmodule_pngSize),25,40);
}

void AuxOut::process() {
    
}

void AuxOut::setChannelIndex(int index) {
    this->channelIndex = index;
}

int AuxOut::getChannelIndex() {
    return channelIndex;
}

juce::Array<PropertyComponent*>& AuxOut::getProperties() {
    
    properties = juce::Array<PropertyComponent*>();
    volumeProp = new SliderPropertyComponent(*volumeValue,"Volume",0,1,0.01,1.0,true);
    
    properties.add(volumeProp);
    
    return properties;
}

