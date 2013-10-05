//
//  particles.cpp
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-26.
//
//

#include "particles.h"

enum AbletonControls {
	kSpookyVol = 1,
	kFireIntensity,
	kFireA,
	kFireB,
	kFireC,
	kFireD
};


const size_t SPOOKY_VERTS = 9000;
const ofVec2f FBO_SIZE(640, 480);



Particles::Particles(ofRectangle _bounds) {
    world = _bounds;
    curX = world.getWidth()/4;
    curY = world.getHeight()/4;
    setupMidi();
    setupParticles();
    camVec.clear();
}

Particles::~Particles() {
    for(size_t i = 0; i < fireParticles.size(); ++i) {
		fireParticles[i].shutdown();
	}
}

void Particles::setupParticles() {
    ofBackground(0);
	ofEnablePointSprites();
	ofClear(0);
    
	fireParticles.resize(4);
	stokeParams.resize(fireParticles.size());
	fbos.resize(fireParticles.size());
	
	//ofSetWindowShape(1280, 800);
	
	for(size_t i = 0; i < fireParticles.size(); ++i) {
#ifdef ONE_FBO
		fireParticles[i].setup(1);
#else
		fireParticles[i].setup(1 / (float)fireParticles.size());
#endif
		fireParticles[i].setBumpAmount(90);
	}
	
	for(size_t i = 0; i < fbos.size(); ++i) {
		fbos[i].allocate(FBO_SIZE.x, FBO_SIZE.y, GL_RGBA32F_ARB);
		fbos[i].begin();
		ofClear(0);
		fbos[i].end();
	}
	
	setupParticleRects();
	
	for(int i = 0; i < SPOOKY_VERTS; i++) {
		spookyVerts.push_back(ofVec2f(ofRandom(FBO_SIZE.x), ofRandom(FBO_SIZE.y)));
	}
	
	spookyVbo.setVertexData(&spookyVerts.front(), SPOOKY_VERTS, GL_DYNAMIC_DRAW);
	
	ofDisableArbTex();
	ofLoadImage(spookyTex, "dot.png");
	spookyShader.load("shaders/spooky-shader");
	
	spookyShader.begin();
	{
		vector<float> spookyPointSizes;
		for(size_t i = 0; i < spookyVerts.size(); i++) {
			spookyPointSizes.push_back(ofRandom(1, 9));
		}
		
		int pointSizeAttribute = spookyShader.getAttributeLocation("pointSize");
		spookyVbo.setAttributeData(pointSizeAttribute, &spookyPointSizes.front(), 1, SPOOKY_VERTS, GL_STATIC_DRAW);
	}
	spookyShader.end();
}

void Particles::setupParticleRects() {
    for(size_t i = 0; i < fireParticles.size(); ++i) {
		ofRectangle rect(0, 0, fbos[i].getWidth(), fbos[i].getHeight());
		fireParticles[i].setRect(rect);
	}
}

void Particles::draw() {
    ofEnableAlphaBlending();
 //   ofEnableBlendMode(OF_BLENDMODE_ADD);
    for(size_t i = 0; i < fireParticles.size(); ++i) {
#ifdef ONE_FBO
		if(i != 0) continue;
#endif
		fbos[i].begin();
		{
			ofEnableAlphaBlending();
			ofSetColor(bgColor.r, bgColor.g, bgColor.b, ofMap(stokeParams[i].intensity, 0, 1, 10, 30));
			ofRect(0, 0, fbos[i].getWidth(), fbos[i].getHeight());
			
			ofSetColor(255);
			ofEnableBlendMode(OF_BLENDMODE_ADD);
            
			if(showSpooky(i)) {
				spookyShader.begin();
				spookyShader.setUniform1f("visibility", ofMap(stokeParams[i].spookyVisibility, 1, 0, 0, 0.05, true));
				spookyShader.setUniform1f("colorMod", ofNoise(impulseIndex * 0.2));
				spookyTex.bind();
				{
					spookyVbo.draw(GL_POINTS, 0, SPOOKY_VERTS);
				}
				spookyTex.unbind();
				spookyShader.end();
			}
			
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofSetColor(255);
			fireParticles[i].draw();
		}
		fbos[i].end();
	}
	
	// draw FBOs to screen
	ofSetColor(255);
	ofDisableAlphaBlending();
	

	const ofVec2f fboDrawSize(640, 480);
    fbos[0].draw(0, 0, fboDrawSize.x, fboDrawSize.y);
	fbos[1].draw(fboDrawSize.x, 0, fboDrawSize.x, fboDrawSize.y);
	fbos[2].draw(fboDrawSize.x*2, 0, fboDrawSize.x, fboDrawSize.y);
	fbos[3].draw(fboDrawSize.x*3, 0, fboDrawSize.x, fboDrawSize.y);
    ofEnableAlphaBlending();
    
    
}

void Particles::update() {
    impulseIndex += 0.1;
	
	size_t targetColumn = ofMap(intensityVector.x, -1, 1, 0, fireParticles.size());
	for(size_t i = 0; i < stokeParams.size(); ++i) {
		float columnTarget = camVec[i].y;
		
		if(stokeParams[i].intensity < columnTarget) {
			stokeParams[i].intensity = ofLerp(stokeParams[i].intensity, columnTarget, 0.5);
			fireParticles[i].addImpulse();
		} else {
			stokeParams[i].intensity = ofLerp(stokeParams[i].intensity, columnTarget, 0.01);
		}
	}
	
	totalIntensity = 0;
	for(size_t i = 0; i < stokeParams.size(); ++i) {
		totalIntensity += stokeParams[i].intensity;
	}
	totalIntensity /= stokeParams.size();
	totalSpooky = 1 - totalIntensity;
	
	targetColor = ofColor(ofMap(totalIntensity, 0, 1, spookyBlue.r, fireRed.r, true),
						  ofMap(totalIntensity, 0, 1, spookyBlue.g, fireRed.g, true),
						  ofMap(totalIntensity, 0, 1, spookyBlue.b, fireRed.b, true));
	
	for(size_t i = 0; i < fireParticles.size(); ++i) {
		fireParticles[i].setIntensity(ofVec2f(ofSignedNoise(impulseIndex * 0.1, i), stokeParams[i].intensity));
	}
	
	for(size_t i = 0; i < spookyVerts.size(); ++i) {
		const float multi = 0.2;
		float bonusMulti = 0;
		
		// every once in awhile, have 1/6 of the spooky particles get a burst of movement
		if(!(i % 6) && ofNoise(impulseIndex) > 0.6) {bonusMulti = 1.9;}
		
		spookyVerts[i] += ofVec2f(ofSignedNoise(impulseIndex * 0.1, i * 0.2),
								  ofSignedNoise(impulseIndex * 0.1, i * 0.1)) * (multi + bonusMulti);
	}
	spookyVbo.updateVertexData(&spookyVerts[0], spookyVerts.size());
	
	for(size_t i = 0; i < stokeParams.size(); ++i) {
		StokeParams& p = stokeParams[i];
		if(p.spookyVisibility > p.intensity) {
			p.spookyVisibility = ofLerp(p.spookyVisibility, p.intensity, 0.005);
		} else {
			p.spookyVisibility = p.intensity;
		}
	}
	
	// a bit of background flicker
	float flickerAmount = ofNoise(impulseIndex) * 7. * totalIntensity;
	bgColor.r += flickerAmount;
	bgColor.g += flickerAmount * 0.5;
	bgColor.lerp(targetColor, 0.15);
	
//#ifdef CONSTANT_INTENSITY
	for(size_t i = 0; i < stokeParams.size(); ++i) {
		stokeParams[i].intensity = 0.9;
		stokeParams[i].spookyVisibility = 0.9;
	}
//#endif
    
    updateMidi();
}

bool Particles::showSpooky(int which){return stokeParams[which].intensity < 0.2;}


void Particles::createGenerator(ofRectangle world) {
  //  FireParticles * newPart;
//    newPart = new FireParticles();
//    fireParticles.setup(ofVec2f(world.getWidth(), world.getHeight()));
//    fireParticles.setIntensity(ofVec2f(.1,1.0)*3);
//   // particles->push_back(*newPart);
}

void Particles::addEmitter(ofPoint _pnt) {
    emitters.push_back(_pnt);
}

void Particles::clearEmitters() {
    emitters.clear();
}

void Particles::changeIntensity(ofPoint pnt) {
    intensityVector = ofVec2f(ofMap(pnt.x, 0, 640, -1, 1, true),
							  ofMap(pnt.y, 0, 480, 1, 0, true));
}


void Particles::setVectors(vector<ofVec2f> vec) {
    camVec = vec;
}

///////////////////////////////////
////////// MIDI STUFFS  ///////////
///////////////////////////////////

void Particles::setupMidi() {
    midiOut.openVirtualPort("Stoke MIDI");
}

void Particles::updateMidi() {
	midiOut.sendControlChange(1, kFireIntensity, totalIntensity * 127);
	midiOut.sendControlChange(1, kFireA, stokeParams[0].intensity * 127);
	midiOut.sendControlChange(1, kFireB, stokeParams[1].intensity * 127);
	midiOut.sendControlChange(1, kFireC, stokeParams[2].intensity * 127);
	midiOut.sendControlChange(1, kFireD, stokeParams[3].intensity * 127);
	midiOut.sendControlChange(1, kSpookyVol, ofMap(totalSpooky, 0, 1.25, 0, 127, true));
}
