//
//  AddModuleAction.h
//  Synthlab - App
//
//  Created by Matthias Pueski on 16.04.18.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Modules/Module.h"
#include "../SynthEditor.h"
#include "../Modules/ModuleUtils.h"

class RemoveSelectedAction : public juce::UndoableAction {
    
public:
    RemoveSelectedAction(SynthEditor* editor, Mixer* mixer);
    ~RemoveSelectedAction(){};
    
    virtual bool perform() override;
    virtual bool undo() override;
    void removeSelectedItem();

    
private:
    
    SynthEditor* editor;
    Module* module = nullptr;
    std::vector<Connection*> removedConnections;
    std::vector<Module*> removedModules;
    Mixer* mixer;
};




