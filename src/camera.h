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
    void draw(ofPoint pos, float scale); // Calibration and debug
    
    // Make entry points
    void addPoint(ofPoint point);
    void closePoints();
    void setCircle(vector<ofPoint> * bounds);
    void resetCircle();
    void setCircleFromPolyline(const ofPolyline * line);
    ofPolyline* getCircle();
    ofRectangle getBounds(float scale);
    
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
