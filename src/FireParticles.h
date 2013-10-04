#pragma once

#include "ofVbo.h"
#include "ofTexture.h"
#include "ofVec2f.h"
#include "ofShader.h"
#include <vector>

class FireParticles {
public:
	
	FireParticles();
	~FireParticles();
	
	void setup(float particlePercentage); // particlePercentage = multiplier for global particle rate
	void draw();
	void shutdown();
	
	// rectangle particles will appear in
	void setRect(ofRectangle rect);
	
	void addImpulse();
	void setIntensity(ofVec2f intensity);
	void setBumpAmount(float bumpAmount);
	void setBumpWidth(float bumpWidth);

private:
	
	static dispatch_queue_t _particleQueue();
	
	void update();
	void addParticles(size_t particlesToAdd);
	void updateParticleData();
	void removeDeadParticles();
	void clearParticles();
	
	dispatch_source_t _particleTimer;
	
	ofVbo _particleVbo;
	
	vector<ofVec2f> _verts;
	vector<ofVec2f> _vels;
	vector<ofFloatColor> _colors;
	vector<ofVec2f> _dampens;

	ofTexture _particleTex;
	ofShader _particleShader;
	float _bumpAmount;
	float _bumpWidth;
	
	ofRectangle _rect;
	ofVec2f _particleIntensity;
	float _particleNoiseIndex;
	
	bool _isRunning;
	bool _doImpulse;
	
	size_t MAX_PARTICLES;
	pair<size_t, size_t> P_RATE;
};
