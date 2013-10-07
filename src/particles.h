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
#include "ofxMidi.h"
#include "FireParticles.h"

const ofColor spookyBlue(0, 10, 50); // 0 10 50
//const ofColor fireRed(50, 30, 0);
const ofColor fireRed(155, 105, 40); //155 105 40

struct StokeParams {
	float intensity;
	float spookyVisibility;
	
	StokeParams()
	: intensity(0)
	, spookyVisibility(1)
	{ }
};

class Particles {
public:
    Particles(ofRectangle _bounds);
    ~Particles();
    
    void draw();
    void update();
    void createGenerator(ofRectangle world);
    void addEmitter(ofPoint _pnt);
    void clearEmitters();
    void setupParticles();
    void setupMidi();
    void updateMidi();
    void changeIntensity(ofPoint pnt);
    bool showSpooky(int which);
	void setupParticleRects();
    void setVectors(vector<ofVec2f> vec);
	
    float getMainVal();
    
    void increaseUpper();
    void decreaseUpper();
    void increaseLower();
    void decreaseLower();
    
private:
    ofxBox2d box2d;    ofRectangle world;
//    vector<ofxBox2dCircle> particles;
//    FireParticles fireParticles;
    //vector<FireParticles> * particles;
    float impulseIndex;
	
    vector<ofPoint> emitters;
    
	ofColor bgColor;
	ofColor mouseColor;
	ofVec2f intensity;
    int curX,curY;
	vector<FireParticles> fireParticles;
	vector<ofFbo> fbos;
	ofxMidiOut midiOut;
	
	float totalIntensity;
	float totalSpooky; //oOOoOooOoooo
	vector<StokeParams> stokeParams;
	
	ofColor targetColor;
    float mainVal;
	
	ofVec2f intensityVector;
	
	ofVbo spookyVbo;
	ofShader spookyShader;
	ofTexture spookyTex;
	vector<ofVec2f> spookyVerts;
    
    vector<ofVec2f> camVec;
    float upperRamp, lowerCutoff;
    
};

#endif /* defined(__snb2013_simple__particles__) */
