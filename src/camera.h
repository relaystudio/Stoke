//
//  camera.h
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-26.
//
//

#ifndef __snb2013_simple__camera__
#define __snb2013_simple__camera__

#include <iostream>
#include "ofMain.h"
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
    void setBounds(vector<ofPoint> * bounds);
    void resetBounds();
    void setBoundsFromPolyline(const ofPolyline * line);
    ofPolyline* getBounds();
    
    // Camera technical shizzle
    void setupCamera();
    void updateCamera();
    
    void setupFlow();
    void updateFlow(const cv::Mat & frame);
    
    
private:
    ofVideoGrabber cam;
    ofPolyline bounds;
    ofxCv::FlowFarneback flow;
};

#endif /* defined(__snb2013_simple__camera__) */
