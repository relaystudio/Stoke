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
    setupCamera();
    setupFlow();
}

Camera::~Camera() {
    
}

void Camera::update() {
    updateCamera();
}

void Camera::draw(ofPoint pos) {
    ofPushMatrix();
    ofTranslate(pos);
    ofScale(.5,.5);
    cam.draw(0, 0);
    flow.draw(0, 0, cam.getWidth(), cam.getHeight());
    bounds.draw();
    ofPopMatrix();
}

/******************************
Boundary setup
******************************/

void Camera::setBounds(vector<ofPoint> *bounds) {
    
}

void Camera::resetBounds() {
    
}

void Camera::setBoundsFromPolyline(const ofPolyline * line) {
    bounds = *line;
    bounds.close(); // Bounds must be closed
}

ofPolyline * Camera::getBounds() {
    return &bounds;	
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





/******************************
 Flow technicals
 ******************************/
void Camera::setupFlow() {
    flow.setPyramidScale(.5);
    flow.setNumLevels(3);
    flow.setWindowSize(100);
    flow.setUseGaussian(false);
    flow.setPolyN(2);
}

void Camera::updateFlow(const cv::Mat &frame) {
    flow.calcOpticalFlow(cam);
}
