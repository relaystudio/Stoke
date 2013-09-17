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
    box2d.init();
    box2d.setGravity(0.0,0.0);
    box2d.setFPS(30);
    
    // Make the bounds a bit bigger so we can do wrapping
    box2d.setBounds(ofPoint(world.x-10,world.y),
                    ofPoint(world.getWidth()+10,world.getHeight()));
    
    for (int i=0; i<100; i++) {
		float r = ofRandom(5,19);
		ofxBox2dCircle particle;
		particle.setPhysics(3.0, 0.53, 0.9);
		particle.setup(box2d.getWorld(), world.getWidth()/2, world.getHeight()/2, r);
		particles.push_back(particle);
		
	}
}

Particles::~Particles() {
    
}

void Particles::update() {
    box2d.update();
    for(size_t i=0;i<particles.size();i++) {
        ofxBox2dCircle * cur = &particles.at(i);
//        for(size_t j=1;i<particles.size();j++) {
//        particles.at(j).addRepulsionForce(cur->getPosition(), 4);
//        }
        ofPoint r = ofPoint(ofRandom(0,world.getWidth()), ofRandom(0,world.getHeight()));
        particles.at(i).addRepulsionForce(r, .09);
        particles.at(i).addAttractionPoint(r* .8, .1);
        
        particles.at(i).addAttractionPoint(ofGetMouseX(), ofGetMouseY(), .1);
        
        if( cur->getPosition().x < world.getPosition().x) {
            cur->setPosition(world.getWidth(), cur->getPosition().y);
        } else
        if( cur->getPosition().x > world.getWidth() ) {
            cur->setPosition(world.getPosition().x, cur->getPosition().y);
        }
    }
}

void Particles::draw() {
    ofFill();
    ofSetHexColor(0xf6c738);
    for(size_t i=0;i<particles.size();i++) {
        particles.at(i).draw();
    }
}