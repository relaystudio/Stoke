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
    setupGUI(); 
    
    radius = cam.getWidth() / 64;
    
}

Camera::~Camera() {
    exit();
}

void Camera::update() {
    updateCamera();
}

void Camera::draw(ofPoint pos) {
    ofPushMatrix();
    ofTranslate(pos);
    ofScale(scale,scale);
#ifdef FIREWIRE
    frame.draw(0,0,cam.getWidth(),cam.getHeight());
#else
    cam.draw(0, 0, cam.getWidth(), cam.getHeight());
#endif
    flow.draw(0, 0, cam.getWidth(), cam.getHeight());
    ofPushStyle();
        ofSetLineWidth(4);
        ofSetColor(100, 100, 255);
        bounds.draw();
        ofSetColor(100, 255, 100);
        active.draw();
    ofPopStyle();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(cam.getWidth()*scale, 0);
    vector<float> values = getValues(10);
    for(size_t i=0;i<values.size();i++) {
        ofRect(0,0,values[i]*100,5);
        ofLog() << i << ":" << values[i];
        ofTranslate(0,5);
    }
    ofPopMatrix();
    gui->draw();
}

void Camera::toggleGui(bool _toggle) {
    gui->setVisible(_toggle);
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
#ifdef FIREWIRE
      cam.setup();
    cout << "camera: " << cam.getLibdcCamera() << endl;
    cout << "camera: " << cam.getImageType() << endl;
    frame.allocate(cam.getWidth(),cam.getHeight(),cam.getImageType());
#else
    if(cam.listDevices().size() >0) cam.initGrabber(640,480);
    else ofLog(OF_LOG_ERROR) << "No cameras";
#endif
}

void Camera::updateCamera() {
#ifdef FIREWIRE
    if(cam.grabVideo(frame)) {
        frame.update();
        updateFlow(&frame.getPixelsRef());
    }
#else
    cam.update();
    if(cam.isFrameNew()) {
        updateFlow(cam.getPixelsRef());
    }
#endif
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

void Camera::updateFlow(ofPixels * _frame) {
    flow.setWindowSize(16);
#ifdef FIREWIRE
    flow.calcOpticalFlow(frame);
#else
    flow.calcOpticalFlow(cam);
#endif
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

    return attr;
}

vector<float> Camera::getValues(int _count) {
    std::vector<float> v;
    long distance;
    for(size_t i=0;i<active.size();i++) {
        distance = bounds.getCentroid2D().squareDistance(active[i]);
        v.push_back(distance);
    }
    
    if(v.size()>2) {
        vector<float>::iterator min_v = std::min_element(v.begin(),v.end());
        vector<float>::iterator max_v = std::max_element(v.begin(),v.end());
        long mn = min_v[0];
        long mx = max_v[0];
        
        for(size_t i=0;i<v.size();i++) {
            v[i] = (v.at(i) - mn) / (mx - mn);
        }
    }
    
    return v;
}


///////////
////////

void Camera::setupGUI() {
    
    float dim = 24;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    
    vector<string> framerates;
    //    framerates.push_back("Max");
	framerates.push_back("60");
	framerates.push_back("30");
	framerates.push_back("15");
    framerates.push_back("7.5");
    vector<string> imageCaptureSizes;
	imageCaptureSizes.push_back("1280x960");
	imageCaptureSizes.push_back("640x480");
    
    unsigned int newMin = 0;
    unsigned int newMax = 0;
    unsigned int newValue = 0;
    
    gui = new ofxUICanvas(cam.getWidth(),0,800,480);
    gui->addWidgetDown(new ofxUILabel("FIREFLY MV USB CONTROL", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 1));
    gui->addWidgetDown(new ofxUIFPSSlider(length-xInit, dim*.25, 0, 100, 60.0, "FPS SLIDER"));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 1));
    gui->addWidgetDown(new ofxUIToggle(20, 20, false, "BLOCKING"));
    cam.getBrightnessRawRange(&newMin, &newMax);
    newValue = cam.getBrightnessRaw();
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, newMin, newMax, newValue, "BRIGHTNESS", OFX_UI_FONT_SMALL));
    cam.getGammaRawRange(&newMin, &newMax);
    newValue = cam.getGammaRaw();
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, newMin, newMax, newValue, "GAMMA", OFX_UI_FONT_SMALL));
    cam.getGainRawRange(&newMin, &newMax);
    newValue = cam.getGainRaw();
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, newMin, newMax, newValue, "GAIN", OFX_UI_FONT_SMALL));
    cam.getExposureRawRange(&newMin, &newMax);
    newValue = cam.getExposureRaw();
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, newMin, newMax, newValue, "EXPOSURE", OFX_UI_FONT_SMALL));
    cam.getShutterRawRange(&newMin, &newMax);
    newValue = cam.getShutterRaw();
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, newMin, newMax, newValue, "SHUTTER", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIRadio( dim, dim, "FRAMERATE (FPS)", framerates, OFX_UI_ORIENTATION_HORIZONTAL));
    gui->addWidgetDown(new ofxUIRadio( dim, dim, "IMAGE CAPTURE SIZE (PX)", imageCaptureSizes, OFX_UI_ORIENTATION_HORIZONTAL));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 1));
//    gui->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 2.0, 0.0, red, "RED", "LESS RED", "MORE RED", OFX_UI_FONT_SMALL));
//    gui->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 2.0, 0.0, green, "GREEN", "LESS GREEN", "MORE GREEN", OFX_UI_FONT_SMALL));
//    gui->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 2.0, 0.0, blue, "BLUE", "LESS BLUE", "MORE BLUE", OFX_UI_FONT_SMALL));
//    
    
    // OPENCV STUFF
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 0.1, 3., .5, "PYRAMIDSCALE", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 2, 8, 4, "NUMLEVELS", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 1, 16, 8, "WINDOWSIZE", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 1, 8., 2, "POLYN", OFX_UI_FONT_SMALL));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, .5, 3., 1.5, "POLYSIGMA", OFX_UI_FONT_SMALL));
 
    
    
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 1));
    gui->addWidgetDown(new ofxUIToggle(20, 20, false, "FULLSCREEN"));
    ofAddListener(gui->newGUIEvent, this, &Camera::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
    gui->setVisible(false);
    gui->setAutoDraw(false);
    gui->setAutoUpdate(true);
}



void Camera::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

//--------------------------------------------------------------
void Camera::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "BLOCKING") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.setBlocking(toggle->getValue());
        cout << "blocking: " << toggle->getValue() << endl;
        cout << "getBlocking(): " << cam.getBlocking() << endl;
    } else if(e.widget->getName() == "BRIGHTNESS") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        brightness = slider->getScaledValue();
        cam.setBrightnessRaw(brightness);
        cout << "brightness: " << cam.getBrightnessRaw() << endl;
    } else if(e.widget->getName() == "GAMMA") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        gamma = slider->getScaledValue();
        cam.setGammaRaw(gamma);
        cout << "gamma: " << cam.getGammaRaw() << endl;
    } else if(e.widget->getName() == "GAIN") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        gain = slider->getScaledValue();
        cam.setGainRaw(gain);
        cout << "gain: " << cam.getGainRaw() << endl;
    } else if(e.widget->getName() == "EXPOSURE") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        exposure = slider->getScaledValue();
        cam.setExposureRaw(exposure);
        cout << "exposure: " << cam.getExposureRaw() << endl;
    } else if(e.widget->getName() == "SHUTTER") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        shutter = slider->getScaledValue();
        cam.setShutterRaw(shutter);
        cout << "shutter: " << cam.getShutterRaw() << endl;
    } else if(e.widget->getName() == "60") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.setFrameRate(60.0);
        cam.flushBuffer();
        cout << "framerate: " << cam.getFrameRate() << endl;
    } else if(e.widget->getName() == "30") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.setFrameRate(30.0);
        cam.flushBuffer();
        cout << "framerate: " << cam.getFrameRate() << endl;
    } else if(e.widget->getName() == "15") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.setFrameRate(15.0);
        cam.flushBuffer();
        cout << "framerate: " << cam.getFrameRate() << endl;
    } else if(e.widget->getName() == "7.5") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.setFrameRate(7.5);
        cam.flushBuffer();
        cout << "framerate: " << cam.getFrameRate() << endl;
    } else if(e.widget->getName() == "1280x960") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.flushBuffer();
        cam.setSize(1328, 1048);
        cam.setFrameRate(15.0);
        setupCamera();
        cout << "Width: " << cam.getWidth() << endl;
        cout << "Height: " << cam.getHeight() << endl;
    } else if(e.widget->getName() == "640x480") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cam.flushBuffer();
        cam.setSize(640, 480);
        setupCamera();
        cout << "Width: " << cam.getWidth() << endl;
        cout << "Height: " << cam.getHeight() << endl;
    } else if(e.widget->getName() == "RED") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        rValue = slider->getScaledValue();
        cout << "rValue: " << slider->getScaledValue() << endl;
    } else if(e.widget->getName() == "BLUE") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        bValue = slider->getScaledValue();
        cout << "bValue: " << slider->getScaledValue() << endl;
    } else if(e.widget->getName() == "GREEN") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        gValue = slider->getScaledValue();
        cout << "gValue: " << slider->getScaledValue() << endl;
    } else if(e.widget->getName() == "FULLSCREEN") {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
        cout << "fullscreen: " << toggle->getValue() << endl;
    } // OKAY< time for CV
    
    /*
     flow.setPyramidScale(.5);
     flow.setNumLevels(4);
     flow.setWindowSize(8);
     flow.setUseGaussian(false);
     flow.setPolyN(2);
     flow.setPolySigma(1.5);
     */
    
    else if (e.widget->getName() == "PYRAMIDSCALE") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        flow.setPyramidScale(slider->getValue());
    } else if( e.widget->getName() == "NUMLEVELS") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        flow.setNumLevels(slider->getValue());
    } else if( e.widget->getName() == "WINDOWSIZE") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        flow.setWindowSize(slider->getValue());
    } else if( e.widget->getName() == "POLYN") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        flow.setPolyN(slider->getValue());
    } else if( e.widget->getName() == "POLYSIGMA") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        flow.setPolySigma(slider->getValue());
    }
}

