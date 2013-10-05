 //
//  Viewport.h
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-25.
//
//

#ifndef __snb2013_simple__Viewport__
#define __snb2013_simple__Viewport__

#include <iostream>
#include "ofMain.h"

#define N_LAYER 5

class Viewport : public ofRectangle {
  
public:
    Viewport(ofPoint loc, int w, int h);
    ~Viewport();
    
    void draw();
    void update();
    
    void setLayer(int layer, ofPixels * mask);
    ofImage * getLayer(int layer);
    
    void setCurrentLayer(float normalized);
    void resetLayer(int layer);
    void saveToImage(int layer, string path);
    void loadImages(int projector);
    
private:
    ofImage layers[N_LAYER];
    int fade[N_LAYER];
    int currentLayer;
    
    
};

#endif /* defined(__snb2013_simple__Viewport__) */
