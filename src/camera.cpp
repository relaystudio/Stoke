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

void Camera::draw(ofPoint pos, float scale) {
    ofPushMatrix();
    ofTranslate(pos);
    ofScale(scale,scale);
    cam.draw(0, 0);
    flow.draw(0, 0, cam.getWidth(), cam.getHeight());
    ofPushStyle();
    ofSetLineWidth(4);
    ofSetColor(100, 100, 255);
    bounds.draw();
    ofPopStyle();
    ofPopMatrix();
}

ofRectangle Camera::getBounds(float scale) {
    return ofRectangle(0,0,cam.getWidth() * scale, cam.getHeight() * scale);
}

/******************************
Boundary setup
******************************/

void Camera::addPoint(ofPoint point) {
    bounds.addVertex(point);
    
}

void Camera::closePoints() {
    bounds.close();
}

void Camera::setCircle(vector<ofPoint> *bounds) {
    
}

void Camera::resetCircle() {
    
}

void Camera::setCircleFromPolyline(const ofPolyline * line) {
    bounds = *line;
    bounds.close(); // Bounds must be closed
}

ofPolyline * Camera::getCircle() {
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
