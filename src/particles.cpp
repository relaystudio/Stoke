//
//  particles.cpp
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-26.
//
//

#include "particles.h"

Particles::Particles(ofRectangle _bounds) {
    world = _bounds;
    curX = world.getWidth()/2;
    curY = world.getHeight()/2;
	ofEnablePointSprites();
    bgColor = mouseColor = ofColor(0);
    fireParticles.setup(ofVec2f(world.getWidth(), world.getHeight()));
    fireParticles.setIntensity(ofVec2f(.1,1.0)*3);
}

Particles::~Particles() {
    fireParticles.shutdown();
}

void Particles::draw() {
    ofPushStyle();
    ofBackground(bgColor);
    ofSetColor(255);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	fireParticles.draw();
    ofPopStyle();
}

void Particles::update() {
    impulseIndex += 0.1;
    randomWalk();
	
	if(ofNoise(impulseIndex) > 0.68) {
		fireParticles.addImpulse();
	}
	
	// a bit of background flicker
	float flickerAmount = ofNoise(impulseIndex) * 7. * intensity.y;
	bgColor.r += flickerAmount;
	bgColor.g += flickerAmount * 0.5;
	
	bgColor.lerp(mouseColor, 0.15);
}


void Particles::randomWalk() {
    int x, y;
    x = curX + ofRandom(-10,10);
    y = curY + ofRandom(-10,10);
    if(x>world.getWidth() || x<0) x = ofRandom(0,world.getWidth());
    if(y>world.getHeight() || y<0) y = ofRandom(0,world.getHeight());
    intensity = ofVec2f(ofMap(x, 0, ofGetWidth(), -1, 1, true),
						ofMap(y, 0, ofGetHeight(), 1, 0.1, true));
	
	mouseColor = ofColor(ofMap(intensity.y, 0.1, 1, 0,  50, true),
						 ofMap(intensity.y, 0.1, 1, 10, 30, true),
						 ofMap(intensity.y, 0.1, 1, 40,  0, true));
	
	fireParticles.setIntensity(intensity);
}
