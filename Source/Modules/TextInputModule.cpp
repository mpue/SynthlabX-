/*
 ==============================================================================
 
 Constant
 Created: 4 Apr 2018 3:48:23pm
 Author:  Matthias Pueski
 
 ==============================================================================
 */

#include "../JuceLibraryCode/JuceHeader.h"
#include "TextInputModule.h"

using juce::Justification;
using juce::Value;
using juce::PropertyComponent;
using juce::SliderPropertyComponent;
using juce::Array;
using juce::String;

//==============================================================================
TextInputModule::TextInputModule()
{
    
    setSize(120,30);
    nameLabel->setJustificationType (Justification::left);
    nameLabel->setTopLeftPosition(2,2);
    
    editable = false;
    prefab = true;
    
    createProperties();
    
}

void TextInputModule::createProperties() {
    valueValue = new Value();
    valueListener = new ValueListener(*valueValue, this);
}

TextInputModule::~TextInputModule()
{
    delete valueValue;
    delete valueListener;
}

void TextInputModule::configurePins(){
    Pin* p1 = new Pin(Pin::Type::VALUE);
    p1->direction = Pin::Direction::OUT;
    p1->setName("V");
    addPin(Pin::Direction::OUT,p1);
    
}

void TextInputModule::paint(juce::Graphics &g) {
    Module::paint(g);
}

juce::String TextInputModule::getValue() {
    return value;
}

void TextInputModule::setValue(juce::String value) {
    this->value = value;
    valueValue->setValue(value);
    this->pins.at(0)->setValue(value.getFloatValue());
    this->nameLabel->setText(name +" = "+String(value),juce::NotificationType::dontSendNotification);
}

void TextInputModule::setEditing(bool editing) {
    /*
     if (editing) {
     if (!this->editing) {
     this->editing = true;
     nameLabel->setVisible(false);
     nameEditor->setVisible(true);
     nameEditor->setText(getName());
     setInterceptsMouseClicks(false,true);
     nameEditor->setWantsKeyboardFocus(true);
     nameEditor->setSelectAllWhenFocused(true);
     nameEditor->grabKeyboardFocus();
     }
     }
     else {
     if (this->editing) {
     this->editing = false;
     setName(nameEditor->getText());
     this->nameLabel->setText(name +" = "+String(value),juce::NotificationType::dontSendNotification);
     nameLabel->setVisible(true);
     nameEditor->setVisible(false);
     setInterceptsMouseClicks(false,false);
     nameEditor->setWantsKeyboardFocus(false);
     }
     }
     */
    
}

juce::Array<PropertyComponent*>& TextInputModule::getProperties() {
    
    properties = juce::Array<PropertyComponent*>();
    valueProp = new TextPropertyComponent(*valueValue,"Value",16, false,true);
    
    properties.add(valueProp);
    
    return properties;
}

