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
    
    radius = cam.getWidth() / 64;
    
}

Camera::~Camera() {
    
}

void Camera::update() {
    updateCamera();
}

void Camera::draw(ofPoint pos) {
    ofPushMatrix();
    ofTranslate(pos);
    ofScale(scale,scale);
    cam.draw(0, 0);
    flow.draw(0, 0, cam.getWidth(), cam.getHeight());
    ofPushStyle();
        ofSetLineWidth(4);
        ofSetColor(100, 100, 255);
        bounds.draw();
        ofSetColor(100, 255, 100);
        active.draw();
    ofPopStyle();
    ofPopMatrix();
}

ofRectangle Camera::getBounds() {
    return ofRectangle(0,0,cam.getWidth() * scale, cam.getHeight() * scale);
}

void Camera::setScale(float _scale) {
    scale = _scale;
}
/******************************
Boundary setup
******************************/

void Camera::addPoint(ofPoint point) {
    ofPoint p = point / scale;
    bounds.addVertex(p);
    active = bounds;
    
}

void Camera::closePoints() {
    bounds.close();
    active = bounds;
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
    flow.setNumLevels(4);
    flow.setWindowSize(8);
    flow.setUseGaussian(false);
    flow.setPolyN(2);
    flow.setPolySigma(1.5);
}

void Camera::updateFlow(const cv::Mat &frame) {
    flow.setWindowSize(16);
    flow.calcOpticalFlow(cam);
    if(bounds.isClosed()) {
        for(size_t i=0;i<bounds.size();i++) {
            ofRectangle area;
            ofPoint * p = &bounds[i];
            ofPoint * a = &active[i];
            float r = radius;
            int px, py, pw, ph;
            px = p->x-r < 1 ? 0 : p->x-r;
            py = p->y-r < 1 ? 0 : p->y-r;
            
            // IN MEMORY OF MY DERP
            // pw = p->x+r >= flow.getWidth()  ? flow.getWidth()-1 : p->x+r;
            // ph = p->y+r >= flow.getHeight() ? flow.getHeight()-1 : p->y+r;
            area.set(px, py, r*2, r*2);
            ofVec2f force = flow.getAverageFlowInRegion(area);
            a->set(a->x + force.x, a->y + force.y);
            a->set(*a + getAttraction(*a,*p));
//            a->set(a->x + p->x / abs(a->x - p->x),
//                   a->y + p->y / abs(a->y - p->y));
            
            
        }
    }
}

ofVec2f Camera::getAttraction(ofPoint &point, ofPoint &origin) {
    ofPoint p = point;
    ofPoint o = origin;
    ofPoint acc = ofPoint();
    float dist = p.squareDistance(o);
    float ratio = 1.0f ;
    
    acc.x += 0.1 * (o.x - p.x);
    
    acc.y += 0.1 * (o.y - p.y) ;
    
    ofPoint attr = acc * ratio ;

//    ofLog() << attr;
    return attr;
}
