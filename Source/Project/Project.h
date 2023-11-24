/*
  ==============================================================================

    Project.h
    Created: 21 Dec 2016 7:45:32pm
    Author:  Matthias Pueski

  ==============================================================================
*/

#ifndef PROJECT_H_INCLUDED
#define PROJECT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ProjectConfig.h"
#include "CustomLookAndFeel.h"
#include "MainTabbedComponent.h"
#include "AudioEngine/Sampler.h"
#include "PropertyView.h"
#include "Module.h"

#include <vector>
#include <map>

using namespace std;

class Project : public ChangeBroadcaster {
    
public:

    Project() {        
        this->tracklength = DEFAULT_TRACK_LENGTH;
        name = "empty Project";
        this->config = new ProjectConfig();
    }
    
    ~Project() {
         delete config;
    }

    static long DEFAULT_TRACK_LENGTH;
    static int  DEFAULT_TRACK_HEIGHT;
    static float DEFAULT_TEMPO;
    
    juce::StringArray& getRecentFiles();
    void loadRecentFileList();
    void addRecentFile(juce::String path);
      
    void setRoot(Module* root);
    Module* getRoot();
                
    String getCurrentFilePath();
    void setCurrentFilePath(String path);
    
    String getCurrentFileName();
    void setCurrentFileName(String path);
    
    bool isNewFile();
    void setNewFile(bool isNew);
    
    bool isDirty();
    void setDirty(bool dirty);    
    void setTracklength(long length);
    long getTrackLength();
    void setName(String name);    
    String getName();    
    void addChangeListener(ChangeListener* listener);
    ProjectConfig* getConfig();
    void createNew(String name);    
    float getTempo();    
    void setTempo(float tempo);
    void saveCurrent(File output);
    void save(File output);
    void load(File file);    
    void clear();
    void addAudioFile(String id, String path);        
    String getAudioPath(String id);
    double snap(double location, double raster);
    
protected:
        
    juce::StringArray recentFiles;
    CustomLookAndFeel* lookAndFeel;    
    std::vector<juce::DialogWindow*> openWindows;
    juce::Component* main = nullptr;
    Sampler* defaultSampler = nullptr;
    Module* root = nullptr;
    
    String currentFilePath;
    String currentFileName;
    bool newFile = true;
    bool dirty = false;
    
    int tolerance = 1;
   
    String name;
    long tracklength;
    double sampleRate;
    int bufferSize;
    float tempo = DEFAULT_TEMPO;
    
    map<String, String> projectAudio;
    
    ProjectConfig* config = NULL;
    
};



#endif  // PROJECT_H_INCLUDED
