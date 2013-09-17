//
//  camera.cpp
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-26.
//
//

#include "camera.h"

using namespace cv;
using namespace ofxCv;

Camera::Camera() {
    
}

Camera::~Camera() {
    
}

void Camera::update() {
    
}

void Camera::draw() {
    
}

/******************************
Boundary setup
******************************/

void Camera::setBounds(vector<ofPoint> *bounds) {
    
}

void Camera::resetBounds() {
    
}

void Camera::setBoundsFromPolyline(ofPolyline * line) {
    
}

ofPolyline * Camera::getBounds() {
    
}
/******************************
 Camera technicals
 ******************************/

void Camera::setupCamera() {
    if(cam.listDevices().size() >0) cam.initGrabber(640,480);
    else ofLog(OF_LOG_ERROR) << "No cameras";
}

void Camera::updateCamera() {
    cam.update();
    if(cam.isFrameNew()) {
        updateFlow(toCv(cam.getPixelsRef()));
    }
}


