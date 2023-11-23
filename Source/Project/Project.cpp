/*
  ==============================================================================

    Project.cpp
    Created: 21 Dec 2016 8:18:45pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#include "Project.h"

using juce::String;
using juce::File;
using juce::ValueTree;
using juce::ScopedPointer;
using juce::XmlElement;
using juce::XmlDocument;
using juce::OutputStream;


Project *Project::instance = NULL;

long  Project::DEFAULT_TRACK_LENGTH = 600;
int   Project::DEFAULT_TRACK_HEIGHT = 75;
float Project::DEFAULT_TEMPO = 100.0f;

void Project::addRecentFile(juce::String path) {
    
    if (recentFiles.size() > 5) {
        recentFiles.remove(0);
    }
    
    if (!recentFiles.contains(path)) {
        recentFiles.add(path);
    }
    
    String userHome = File::getSpecialLocation(File::userHomeDirectory).getFullPathName();
    File appDir = File(userHome+"/.Synthlab");
    
    if (!appDir.exists()) {
        appDir.createDirectory();
    }
    
    File configFile = File(userHome+"/.Synthlab/recent.xml");
    
    if (!configFile.exists()) {
        configFile.create();
    }
    else {
        configFile.deleteFile();
        configFile = File(userHome+"/.Synthlab/recent.xml");
    }
    
    ValueTree* v = new ValueTree("RecentFiles");
    
    for (int i = 0 ;i < recentFiles.size();i++) {
        
        ValueTree child = ValueTree("File");
        child.setProperty("path", recentFiles.getReference(i), nullptr);
        v->addChild(child, i, nullptr);
    }
    
    std::unique_ptr<OutputStream> os = configFile.createOutputStream();
    
    std::unique_ptr<XmlElement> xml = v->createXml();
    
    xml->writeToStream(*os, "");

    os = nullptr;
    xml = nullptr;
    delete v;
    
}



void Project::loadRecentFileList() {
    
    String userHome = File::getSpecialLocation(File::userHomeDirectory).getFullPathName();
    
    File appDir = File(userHome+"/.Synthlab");
    
    if (!appDir.exists()) {
        appDir.createDirectory();
    }
    
    File configFile = File(userHome+"/.Synthlab/recent.xml");
    
    if (configFile.exists()) {
        std::unique_ptr<XmlElement> xml = XmlDocument(configFile).getDocumentElement();
        ValueTree v = ValueTree::fromXml(*xml.get());
        
        for (int i = 0 ;i < v.getNumChildren();i++) {
            String path = v.getChild(i).getProperty("path");
            
            if(!recentFiles.contains(path)) {
                recentFiles.add(path);
            }
        }
        xml = nullptr;
    }
    
    
}

void Project::destroy() {
    delete instance;
}

juce::UndoManager* Project::getUndoManager() {
    return undoManager;
}

juce::StringArray& Project::getRecentFiles() {
    return recentFiles;
}


void Project::setSupplemental(MainTabbedComponent* component) {
    this->supplementalTab = component;
}

MainTabbedComponent* Project::getSupplemental(){
    return supplementalTab;
}

CustomLookAndFeel* Project::getLookAndFeel() {
    return lookAndFeel;
};

std::vector<juce::DialogWindow*>& Project::getOpenWindows() {
    return openWindows;
}

void Project::setDefaultSampler(Sampler* sampler) {
    this->defaultSampler = sampler;
}

Sampler* Project::getDefaultSampler() {
    return this->defaultSampler;
}

void Project::setRoot(Module* root) {
    this->root = root;
}

Module* Project::getRoot() {
    return root;
}

juce::Component* Project::getMain() {
    return main;
}

void Project::setMain(juce::Component* c) {
    main = c;
}

ApplicationCommandManager* Project::getCommandManager() {
    return commandManager;
};

void Project::setCommandManager(ApplicationCommandManager* commandManager) {
    this->commandManager = commandManager;
}

Project::AppMode Project::getAppMode() {
    return mode;
}

void Project::setNewFile(bool isNew) {
    this->newFile = isNew;
}

bool Project::isNewFile() {
    return newFile;
}

void Project::setCurrentFilePath(juce::String path) {
    this->currentFilePath = path;
}

String Project::getCurrentFilePath() {
    return currentFilePath;
}

void Project::setCurrentFileName(juce::String path) {
    this->currentFileName = path;
}

String Project::getCurrentFileName() {
    return currentFileName;
}

bool Project::isDirty() {
    return dirty;
}

void Project::setDirty(bool dirty) {
    this->dirty = dirty;
}

void Project::setTracklength(long length) {
    this->tracklength = length;
    sendChangeMessage();
};

long Project::getTrackLength() {
    return this->tracklength;
}

void Project::setName(String name) {
    this->name = name;
    sendChangeMessage();
}

String Project::getName() {
    return name;
}

void Project::addChangeListener(ChangeListener* listener) {
    ChangeBroadcaster::addChangeListener(listener);
}

ProjectConfig* Project::getConfig() {
    return config;
}

void Project::createNew(String name) {
    this->name = name;

    if (config != NULL) {
        delete config;
    }

    config = new ProjectConfig();
    clear();
}

float Project::getTempo() {
    return tempo;
}

void Project::setTempo(float tempo) {
    this->tempo = tempo;
}

void Project::saveCurrent(File output) {
    ValueTree v = config->getConfig();

    std::map<String, String>::iterator it;

    ValueTree audioFiles = ValueTree("AudioFiles");

    for (it = projectAudio.begin(); it != projectAudio.end(); ++it) {
        ValueTree file = ValueTree("File");
        file.setProperty("refId", it->first, nullptr);
        file.setProperty("path", it->second, nullptr);
        audioFiles.addChild(file, -1, nullptr);
    }


    std::unique_ptr<XmlElement> xml = v.createXml();
    xml->writeToFile(output, "");
    xml = nullptr;
}

void Project::save(File output) {

    setName(output.getFileNameWithoutExtension());

    config->setName(name);
    config->setBufferSize(bufferSize);
    config->setSampleRate(sampleRate);
    config->setTracklength(tracklength);
    config->setTempo(tempo);

    ValueTree v = config->getConfig();

    std::map<String, String>::iterator it;

    ValueTree audioFiles = ValueTree("AudioFiles");

    for (it = projectAudio.begin(); it != projectAudio.end(); ++it) {
        ValueTree file = ValueTree("File");
        file.setProperty("refId", it->first, nullptr);
        file.setProperty("path", it->second, nullptr);
        audioFiles.addChild(file, -1, nullptr);
    }

    v.addChild(audioFiles, -1, nullptr);

    std::unique_ptr<XmlElement> xml = v.createXml();
    xml->writeToFile(output, "");
    xml = nullptr;

}

void Project::load(File file) {
    std::unique_ptr<XmlElement> xml = XmlDocument(file).getDocumentElement();
    ValueTree v = ValueTree::fromXml(*xml.get());
    xml = nullptr;
    config->setConfig(v);
    setName(config->getName());
}

void Project::clear() {
    projectAudio.clear();
}

void Project::addAudioFile(String id, String path) {
    projectAudio.insert(make_pair(id, path));
}

String Project::getAudioPath(String id) {
    return projectAudio[id];
}
