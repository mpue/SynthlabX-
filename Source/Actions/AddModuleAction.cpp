//
//  AddModuleAction.cpp
//  Synthlab - App
//
//  Created by Matthias Pueski on 16.04.18.
//

#include "AddModuleAction.h"
#include "../Modules/Module.h"
#include "../AudioOut.h"
#include "../AudioIn.h"
#include "../AuxOut.h"
#include "../PrefabFactory.h"
#include "../AudioManager.h"
#include "../Modules/TerminalModule.h"
#include "../Modules/ImageModule.h"
#include "../Mixer.h"

using juce::String;

AddModuleAction::AddModuleAction(SynthEditor* editor, Mixer* mixer,juce::Point<int> position, long moduleId) {
    this->editor = editor;
    this->mixer = mixer;
    this->position = position;
    this->moduleId = moduleId;
}

bool AddModuleAction::undo() {
    
    if (module == nullptr) {
        return false;
    }
    
    for (std::vector<Module*>::iterator it = editor->getModule()->getModules()->begin();it != editor->getModule()->getModules()->end();) {
        
        if ((*it)->getIndex() == module->getIndex()) {
            editor->removeChangeListener((*it));
            // delete (*it);
            it = editor->getModule()->getModules()->erase(it);
            editor->removeChildComponent((*it));
        }
        else {
            ++it;
        }
    }
    
    editor->repaint();
    
    
    return true;
}

bool AddModuleAction::perform() {
    
    if (moduleId < 1) {
        return false;
    }
    
    Module* m = module;
    
    if (m == nullptr) {
        m = PrefabFactory::getInstance()->getPrefab(moduleId, editor->getSampleRate(), editor->getBufferSize());
    }
    
    AudioManager* audioManager = AudioManager::getInstance();
    AudioOut* out;

    // If the module is an audio input, output or aux we need to take special care
    // in all three cases we add connections to the mixer
    
    if ((out = dynamic_cast<AudioOut*>(m)) != NULL) {

        int numCurrentOutputs = static_cast<int>(mixer->getOutputChannels().size()) * 2;

        // take care! we have N input channels but from the audio interfaces point of view we have twice as much channels!
        
        bool useDefaultOutput = false;
        
        if (audioManager->getOutputChannelNames().size() == 2) {
            String channelName = audioManager->getOutputChannelNames().getReference(0);
            useDefaultOutput = true;
        }

        // audio out found, the mixer needs an audio out object
        
        if (numCurrentOutputs < audioManager->getNumActiveOutputs() || (useDefaultOutput && numCurrentOutputs == 0)) {
            mixer->getOutputChannels().push_back(out);
            // guess the channel name
            String channelName = audioManager->getOutputChannelNames().getReference(static_cast<int>(mixer->getOutputChannels().size()) - 1);
            int channelIndex = editor->addChannel(channelName, Mixer::Channel::Type::OUT);
            out->setChannelIndex(channelIndex);
            m->setName(channelName);
            // set default volumes
            editor->getMixerPanel()->setVolume(channelIndex, out->getVolume());
            editor->getMixerPanel()->setModule(channelIndex, out);
            if (!editor->isRunning()) {
                editor->setRunning(true);
            }
        }
        else {
            delete m;
            return false;
        }

    }

    // add an aux channel, which is routable
    
    AuxOut* aux;
    
    if ((aux = dynamic_cast<AuxOut*>(m)) != NULL) {
        mixer->getAuxChannels().push_back(aux);
        String channelName = "Aux "+ String(mixer->getAuxChannels().size());
        int channelIndex = editor->addChannel(channelName, Mixer::Channel::Type::AUX);
        aux->setChannelIndex(channelIndex);
        m->setName(channelName);
        editor->getMixerPanel()->setVolume(channelIndex, aux->getVolume());
        editor->getMixerPanel()->setModule(channelIndex, aux);
        if (!editor->isRunning()) {
            editor->setRunning(true);
        }
    }
    
    AudioIn* in;
    
    if ((in = dynamic_cast<AudioIn*>(m)) != NULL) {
        
        int numCurrentInputs = static_cast<int>(mixer->getInputChannels().size()) * 2; // take care! we have N input channels but from the audio interfaces point of view we have twice as much channels!
        
        if (numCurrentInputs < audioManager->getNumActiveInputs()) {
        
           mixer->getInputChannels().push_back(in);
            String channelName = audioManager->getInputChannelNames().getReference(static_cast<int>(mixer->getInputChannels().size()) - 1);
            m->setName(channelName);
            int channelIndex = editor->addChannel(channelName, Mixer::Channel::Type::IN);
            in->setChannelIndex(channelIndex);
            editor->getMixerPanel()->setVolume(channelIndex, in->getVolume());
            editor->getMixerPanel()->setModule(channelIndex, in);
            if (!editor->isRunning()) {
                editor->setRunning(true);
            }
        }
        else {
            delete m;
            return false;
        }
    }
    
    TerminalModule* t;
    
    if ((t = dynamic_cast<TerminalModule*>(m)) != NULL) {
        Pin* p = new Pin(Pin::Type::VALUE);
        t->getPins().at(0)->setTerminal(p);
        p->listeners.push_back(t);
        p->setName(t->getName());
        t->addChangeListener(p);
        if (t->getDirection() == TerminalModule::Direction::IN) {
            editor->getModule()->addPin(Pin::Direction::IN, p);
        }
        else {
            editor->getModule()->addPin(Pin::Direction::OUT, p);
        }
        t->setIndex(p->index);
        
        editor->getModule()->repaint();
        
    }

    editor->addChangeListener(m);
    
    m->setTopLeftPosition(position);
    m->savePosition();
    m->setUiPosition(position.x, position.y);
    m->saveUiPosition();
    
    editor->addAndMakeVisible(m);
    editor->getModule()->getModules()->push_back(m);
    
    ImageModule* im;
    
    if ((im = dynamic_cast<ImageModule*>(m)) != NULL) {
        im->toBack();   
    }
    
    m->setSelected(true);

    editor->getSelectionModel().getSelectedModules().push_back(m);
    editor->repaint();
    
    module = m;
    
    editor->setDirty(true);
    
    return true;
}
