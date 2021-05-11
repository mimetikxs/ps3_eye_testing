#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class ofApp: public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();

    ofVideoGrabber grabber;

    ofParameterGroup parameters;
    ofParameter<int> gain;
    ofParameter<int> exposure;
    ofParameter<int> sharpness;
    ofParameter<int> hue;
    ofParameter<int> brightness;
    ofParameter<int> contrast;
    ofParameter<int> redBalance;
    ofParameter<int> greenBalance;
    ofParameter<int> blueBalance;
    ofParameter<bool> bTestPattern;
    ofParameter<bool> bDebayer;
    
    void applyParameters();
    void onParameterChange(ofAbstractParameter& p);

    // de-bayer
    ofImage imgDebayer;

    ofxPanel gui;
};
