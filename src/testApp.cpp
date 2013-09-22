#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    pw = 640;
    ph = 480;
    ttf.loadFont("mono.ttf",20);
    fbo.allocate(pw * N_PROJECTOR, ph,GL_RGBA);
    mask.allocate(pw * N_PROJECTOR,ph,GL_RGBA);
    cont.allocate(pw*N_PROJECTOR,ph,GL_RGBA);
    
    canvas.allocate(pw,ph);
    for(size_t i=0;i<N_PROJECTOR;i++){
        pview[i] = new Viewport(ofPoint(i*N_PROJECTOR,0),pw,ph);
    }
    
    cam = new Camera();
    
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    canvas.begin();
    ofClear(0);
    canvas.end();
    rectBuf=new ofRectangle(ofPoint(),ofPoint());
    edit=save=debug=false;
    editLayer=editCanvas=0;
    
    part = new Particles(ofRectangle(0,0,pw*N_PROJECTOR,ph));
    alpha.load("shaders/alpha");
}

//--------------------------------------------------------------
void testApp::update(){
    updateEdit();
    updateGraphics();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofClear(0);
    ofSetColor(255);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if(edit) drawEdit();
    else drawGraphics();
    if(debug) drawDebug();
}
///////////////////////////////////////////////////
////////////        Edit Mode             /////////
///////////////////////////////////////////////////

void testApp::updateGraphics() {
    part->update();
    cam->update();
    
    cont.begin();
    part->draw();
    cont.end();
    
    mask.begin();
    ofBackground(0);
    for(int i=0;i<N_PROJECTOR;i++) {
        ofPushMatrix();
        ofTranslate(i*pw, 0);
        for(int j=N_LAYER;j>0;j--) {
//            int trans = (ofGetMouseY() / ofGetHeight()) *255;
            ofSetColor(255,255,255,255);
            pview[i]->getLayer(j).draw(0,0);
        }
        ofPopMatrix();
    }

    mask.end();
        cont.getTextureReference().bind();
    mask.getTextureReference().bind();
    fbo.begin();
    ofClear(0,100);
    alpha.begin();
    alpha.setUniformTexture("tex0",cont,cont.getTextureReference().getTextureData().textureID);
    alpha.setUniformTexture("tex1",mask,mask.getTextureReference().getTextureData().textureID);
    int fWidth = fbo.getWidth();
    int fHeight = fbo.getHeight();
    
    // Draw to a quad
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);         glVertex3f(0, 0, 0);
    glTexCoord2f(fWidth, 0);        glVertex3f(fWidth, 0, 0);
    glTexCoord2f(fWidth, fHeight);  glVertex3f(fWidth, fHeight, 0);
    glTexCoord2f(0, fHeight);       glVertex3f(0, fHeight, 0);
    glEnd();
    
    alpha.end();
    
    fbo.end();
    mask.getTextureReference().unbind();
    cont.getTextureReference().unbind();
}

void testApp::drawGraphics() {
    ofBackground(0);
    fbo.draw(0,0);
}

///////////////////////////////////////////////////
////////////        Graphics Mode         /////////
///////////////////////////////////////////////////


void testApp::drawDebug() {
    cam->draw(ofPoint(0,0));
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

void testApp::resetCurrent() {
    pview[editCanvas]->resetLayer(editLayer);
}

void testApp::updateEdit() {
    if(edit) {
        canvas. begin();
        ofClear(0);
        ofSetColor(255,80,80);
        pview[editCanvas]->getLayer(editLayer).draw(0,0);
        
        if(save)  {
         ofBackground(0);
         ofSetColor(255,255,255,255);
            
        }
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
    ofLog() << "Settings saved";
}

void testApp::reloadSettings() {
    for(int i=0;i<N_PROJECTOR;i++) {
        pview[i]->loadImages(i);
    }
    ofLog() << "Settings loaded";
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
        case 'd':
            debug = !debug; break;
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
        case '[':
            exportSettings(); break;
        case ']':
            reloadSettings(); break;
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