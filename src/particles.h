//
//  particles.h
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-26.
//
//

#ifndef __snb2013_simple__particles__
#define __snb2013_simple__particles__

#include <iostream>

#include "ofMain.h"
#include "ofxBox2d.h"
#include "FireParticles.h"

class Particles {
public:
    Particles(ofRectangle _bounds);
    ~Particles();
    
    void draw();
    void update();
    void randomWalk();
    void createGenerator(ofRectangle world);
    void addEmitter(ofPoint _pnt);
    void clearEmitters();
private:
//    ofxBox2d box2d;
    ofRectangle world;
//    vector<ofxBox2dCircle> particles;
    FireParticles fireParticles;
    //vector<FireParticles> * particles;
    float impulseIndex;
	
    vector<ofPoint> emitters;
    
	ofColor bgColor;
	ofColor mouseColor;
	ofVec2f intensity;
    int curX,curY;
};

#endif /* defined(__snb2013_simple__particles__) */
