//
//  Viewport.cpp
//  snb2013_simple
//
//  Created by Andrew Lovett Barron on 2013-08-25.
//
//

#include "Viewport.h"

Viewport::Viewport(ofPoint loc, int w, int h) {
    setPosition(loc);
    setWidth(w);
    setHeight(h);
    
    for(int i=0;i<N_LAYER;i++) {
        layers[i].allocate(getWidth(), getHeight(), OF_IMAGE_COLOR_ALPHA);
    }
}

Viewport::~Viewport() {
    
}

void Viewport::update() {
    
}

void Viewport::draw() {
    
}

void Viewport::setLayer(int layer, ofPixels * mask) {
    if(layer < N_LAYER) {
        
        layers[layer].setFromPixels(*mask);
        layers[layer].update();
    }//layers[layer] = *mask;
    else ofLog() << "Layers must be less than " << N_LAYER;
}

ofImage Viewport::getLayer(int layer) {
    if(layer < N_LAYER) return layers[layer];
    else return layers[0];
}

void Viewport::setCurrentLayer(float normalized) {
    currentLayer = ofMap(normalized, 0,1,0,N_LAYER-1);
    fade[currentLayer] = ofMap(normalized, 0,1,0,255);
}