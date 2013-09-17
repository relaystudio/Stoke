#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    pw = 640;
    ph = 480;
    ttf.loadFont("mono.ttf",20);
    fbo.allocate(pw * N_PROJECTOR, ph);
    canvas.allocate(pw,ph);
    for(size_t i=0;i<N_PROJECTOR;i++){
        pview[i] = new Viewport(ofPoint(i*N_PROJECTOR,0),pw,ph);
    }
    
    fbo.begin();
    ofClear(0);
    fbo.end();
    
	if(ofGetGLProgrammableRenderer()){
        ofLog() << "Loading gl3";
		shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
	}else{
        ofLog() << "Loading gl2";
		shader.load("shaders/noise.vert", "shaders/noise.frag");
	}
    
    canvas.begin();
    ofClear(0);
    canvas.end();
    rectBuf=new ofRectangle(ofPoint(),ofPoint());
    edit=save=false;
    editLayer=editCanvas=0;
    
    part = new Particles(ofRectangle(0,0,pw*N_PROJECTOR,ph));
    
}

//--------------------------------------------------------------
void testApp::update(){
    updateEdit();
    updateGraphics();
}

//--------------------------------------------------------------
void testApp::draw(){
    if(edit) drawEdit();
    else drawGraphics();
}
///////////////////////////////////////////////////
////////////        Edit Mode             /////////
///////////////////////////////////////////////////

void testApp::updateGraphics() {
    part->update();
    
    fbo.begin();
    ofClear(0);
    shader.begin();
    part->draw();
    shader.end();
    fbo.end();
}

void testApp::drawGraphics() {
    ofBackground(0);
    fbo.draw(0,0);
}

///////////////////////////////////////////////////
////////////        Graphics Mode         /////////
///////////////////////////////////////////////////




///////////////////////////////////////////////////
////////////        Edit Mode             /////////
///////////////////////////////////////////////////

void testApp::setEditMode(bool active) {
    edit = active;
}

bool testApp::getEditMode() {
    return edit;
}

void testApp::saveEditted() {
    save = true;
}

void testApp::resetCurrent() {
    pview[editCanvas]->resetLayer(editLayer);
}

void testApp::updateEdit() {
    if(edit) {
        canvas. begin();
        ofClear(0);
        pview[editCanvas]->getLayer(editLayer).draw(0,0);
        
        if(save) ofSetColor(255);
        else ofSetColor(255, 100, 100);
        for(size_t i=0;i<rect.size();i++) {
            ofRect(rect.at(i));
        }
        
        canvas.end();
        
        if(save) {
            ofPixels pix;
            canvas.readToPixels(pix);
            pview[editCanvas]->setLayer(editLayer,&pix);
            rect.clear();
            ofLog() << "Saving projector " << editCanvas << " layer " << editLayer;
            save=false;
            
        }
    }
}



void testApp::drawEdit() {
    if(edit) {
        ofPushMatrix();
        ofTranslate(editCanvas*pw, 0);
        canvas.draw(0, 0);
        drawEditGrid();
        ofSetColor(255,0,0);

        ttf.drawString("Number keys for layer, qwer for canvas", 10,10);
        ttf.drawString("s to save to layer/canvas, r to reset", 10,50);
        ttf.drawString("Canvas: " + ofToString(editCanvas) +
                        " Layer: " + ofToString(editLayer), 10,ofGetHeight()-10);
        ofPopMatrix();
        ofSetColor(255,0,0);
        if(!rectBuf->isEmpty()) ofRect(*rectBuf);
    }
}

void testApp::drawEditGrid() {
    int gap=20;
    ofPushMatrix();
    ofSetColor(255,127);
    for(int i=0;i<pw;i+=gap) {
        ofLine(i,0,i,ph);
        if(i<ph) ofLine(0,i,pw,i);
    }
    ofPopMatrix();
}


void testApp::exportSettings() {
    for(int i=0;i<N_PROJECTOR;i++) {
        for(int j=0;j<N_LAYER;j++) {
            pview[i]->saveToImage(j, ofToString(i)+"-"+ofToString(j));
        }
    }
}

void testApp::reloadSettings() {
    for(int i=0;i<N_PROJECTOR;i++) {
        pview[i]->loadImages(i);
    }
}

///////////////////////////////////////////////////
////////////          Events              /////////
///////////////////////////////////////////////////


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key <= 49+N_LAYER) { // Top row num key 1 = 49
        editLayer = key-49;
        ofLog() << "Set Layer to " << editLayer;
    }
    
    switch(key) {
        case '`':
            setEditMode(!getEditMode()); break;
        case 'x':
            resetCurrent(); break;
        case 'z':
            saveEditted(); break;
        case 'q':
            editCanvas = 0; break;
        case 'w':
            editCanvas = 1; break;
        case 'e':
            editCanvas = 2; break;
        case 'r':
            editCanvas = 3; break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if(edit && !rectBuf->isEmpty()){
        int offset = editCanvas*pw;
        rectBuf->setPosition(rectBuf->getX()-offset, rectBuf->getY());
        rect.push_back(*rectBuf);
        rectBuf->set(ofPoint(),ofPoint());
        ofLog() << "Finished rectangle" << rectBuf->getPosition() << "w" <<rectBuf->getWidth() << "h"<<rectBuf->getHeight();
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(edit) {
        rectBuf = new ofRectangle(ofPoint(x,y),ofPoint(x+1,y+1));
        
        ofLog() << "Creating rectangle" << rectBuf->getPosition() << "w" <<rectBuf->getWidth() << "h"<<rectBuf->getHeight();
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(edit) {
        rectBuf->set(rectBuf->getPosition(),ofPoint(x,y));
        ofLog() << "Drawing rectangle" << rectBuf->getPosition() << "w" <<rectBuf->getWidth() << "h"<<rectBuf->getHeight();
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}