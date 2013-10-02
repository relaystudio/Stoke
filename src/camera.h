//
//  camera.h
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-26.
//
//

#ifndef __snb2013_simple__camera__
#define __snb2013_simple__camera__

#define FIREWIRE 1

#include <iostream>
#include <algorithm>
#include <vector>
#include "ofMain.h"
#include "ofxLibdc.h"
#include "ofxCv.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"

class Camera {
public:
    Camera();
    ~Camera();
    
    void update(); // Update logics
    void draw(ofPoint pos); // Calibration and debug
    
    // Make entry points
    void addPoint(ofPoint point);
    void closePoints();
    void setCircle(vector<ofPoint> * bounds);
    void resetCircle();
    void setCircleFromPolyline(const ofPolyline * line);
    ofPolyline* getCircle();
    vector<float> getValues(int _count);
    ofRectangle getBounds();
    
    // Camera technical shizzle
    void setupCamera();
    void updateCamera();
    
    void setupFlow();
    void updateFlow(ofPixels * _frame);
    void setScale(float _scale);
    void toggleGui(bool _toggle);
    
private:
    ofVec2f getAttraction(ofPoint &point, ofPoint &origin);
    vector<int> values;
    float scale,radius;
#ifdef FIREWIRE
    ofxLibdc::Camera cam;
    ofImage frame;
#else
    ofVideoGrabber cam;
#endif
    ofPolyline bounds;
    ofPolyline active;
    ofxCv::FlowFarneback flow;
    
    /// OFX UI
    
    void guiEvent(ofxUIEventArgs &e);
    ofxUICanvas *gui;
    void setupGUI();
    void exit();
    bool hideGUI;
    bool drawPadding;
	float red, green, blue, alpha;
    float rValue, gValue, bValue, aValue;
    
    float brightness;
    float gamma;
    float gain;
    unsigned int exposure;
    unsigned int shutter;
    
};

#endif /* defined(__snb2013_simple__camera__) */
