#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    pw = 640;
    ph = 480;
    fbo.allocate(pw * N_PROJECTOR, ph);
    canvas.allocate(pw,ph);
    for(size_t i=0;i<N_PROJECTOR;i++){
        pview[i] = new Viewport(ofPoint(i*N_PROJECTOR,0),pw,ph);
    }
    
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    canvas.begin();
    ofClear(0);
    canvas.end();
    rectBuf=new ofRectangle(ofPoint(),ofPoint());
    edit=save=false;
    
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
    fbo.begin();
    ofClear(0);
    fbo.end();
}

void testApp::drawGraphics() {
    fbo.draw(0,0);
}


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

void testApp::updateEdit() {
    if(edit) {
        canvas. begin();
        pview[editCanvas]->getLayer(editLayer).draw(0,0);
        
        ofSetColor(255, 0, 0);
        for(size_t i=0;i<rect.size();i++) {
            ofRect(rect.at(i));
        }
        
        canvas.end();
        
        if(save) {
            
            canvas.readToPixels(pview[editCanvas]->getLayer(editLayer).getPixelsRef());
            pview[editCanvas]->getLayer(editLayer).update();
            
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
        ofDrawBitmapString("Number keys for layer, qwer for canvas", 10,10);
        ofDrawBitmapString("s to save to layer/canvas, r to reset", 10,20);
        ofPopMatrix();
        ofSetColor(255,0,0);
        if(!rectBuf->isEmpty()) ofRect(*rectBuf);
    }
}

void testApp::drawEditGrid() {
    int gap=20;
    ofPushMatrix();
    ofSetColor(255);
    for(int i=0;i<pw;i+=gap) {
        ofLine(i,0,i,ph);
        if(i<ph) ofLine(0,i,pw,i);
    }
    ofPopMatrix();
}


///////////////////////////////////////////////////
////////////          Events              /////////
///////////////////////////////////////////////////


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key < N_LAYER) {
        editLayer = key;
    }
    
    switch(key) {
        case '`':
            setEditMode(!getEditMode());
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