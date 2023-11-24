//
//  Mixer.cpp
//  Synthlab - App
//
//  Created by Matthias Pueski on 23.04.18.
//  Copyright © 2018 Pueski. All rights reserved.
//

#include "Mixer.h"
#include <vector>
#include "AuxOut.h"
#include "AudioIn.h"
#include "AudioOut.h"

Mixer::Mixer() {   
    outputChannels.reserve(2);
    inputChannels.reserve(2);
    auxChannels.reserve(2);
    channels.reserve(2);
}

Mixer::~Mixer() {
    removeAllChannels();
}

void Mixer::addChannel(Mixer::Channel *channel) {
    channels.push_back(channel);
}

std::vector<Mixer::Channel*>& Mixer::getChannels() {
    return channels;
}

int Mixer::getNumInputs() {
    
    int numinputs = 0;
    
    for (int i = 0; i < channels.size();i++) {
        if (channels.at(i)->channelType == Channel::Type::IN) {
            numinputs++;
        }
    }
    
    return numinputs;
}

int Mixer::getNumOutputs() {
    
    int numoutputs = 0;
    
    for (int i = 0; i < channels.size();i++) {
        if (channels.at(i)->channelType == Channel::Type::OUT) {
            numoutputs++;
        }
    }
    
    return numoutputs;
}

int Mixer::getNumAuxBusses() {
    
    int busses = 0;
    
    for (int i = 0; i < channels.size();i++) {
        if (channels.at(i)->channelType == Channel::Type::AUX) {
            busses++;
        }
    }
    
    return busses;
}

Mixer::Channel* Mixer::getChannel(Mixer::Channel::Type type, int index) {
    
    int channelNum = 0;
    
    for (int i = 0; i < channels.size();i++) {
        if (channels.at(i)->channelType == type) {
            if( channelNum == index) {
                return channels.at(i);
            }
            channelNum++;
        }
       
    }
    return nullptr;
}

void  Mixer::removeChannel(int index) {
    for (std::vector<Channel*>::iterator it = channels.begin();it != channels.end();) {
        if ((*it)->index == index) {
            delete (*it);
            it = channels.erase(it);
        }
        else {
            ++it;
        }

    }
}

void Mixer::removeAllChannels() {      
    for (std::vector<Channel*>::iterator it = channels.begin();it != channels.end();) {
        delete (*it);
        it = channels.erase(it);
    }
}

void Mixer::clearChannels() {
    if (outputChannels.size() > 0)
        outputChannels.clear();

    if (inputChannels.size() > 0)
        inputChannels.clear();

    if (auxChannels.size() > 0)
        auxChannels.clear();
}

bool Mixer::channelIsValid(int channel) {
    if (outputChannels.at(0)->getPins().at(channel)->getConnections().size() == 1 &&
        outputChannels.at(0)->getPins().at(channel)->getConnections().at(0) != nullptr &&
        outputChannels.at(0)->getPins().at(channel)->getConnections().at(0)->getAudioBuffer() != nullptr) {
        return true;
    }
    return false;
}

bool Mixer::auxChannelIsValid(int channel, int subchannel) {
    if (auxChannels.at(channel)->getPins().at(subchannel)->getConnections().size() == 1 &&
        auxChannels.at(channel)->getPins().at(subchannel)->getConnections().at(0) != nullptr &&
        auxChannels.at(channel)->getPins().at(subchannel)->getConnections().at(0)->getAudioBuffer() != nullptr) {
        return true;
    }
    return false;
}