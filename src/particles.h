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

class Particles {
public:
    Particles(ofRectangle _bounds);
    ~Particles();
    
    void draw();
    void update();
private:
    ofxBox2d box2d;
    ofRectangle world;
    vector<ofxBox2dCircle> particles;
};

#endif /* defined(__snb2013_simple__particles__) */
