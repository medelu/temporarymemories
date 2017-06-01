#include "ofApp.h"
#define SIZE 42336000//256
//Only 44100 Hz or up(48000) Hz
#define RATE 44100 //samples per sec
#define FRAMERATE 46
#define RAD 15
#define DPI 72
#define FS 14


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(ofColor::gray);
    ofSetFrameRate(FRAMERATE);
    ofSetVerticalSync(true);
    
    sound = audioBank(SIZE, RATE, this);
    
    ofTrueTypeFont::setGlobalDpi(DPI);
    verdana14.load("verdana.ttf", FS, true, false);
    verdana14.setLineHeight(FS + 2);
    verdana14.setLetterSpacing(1);
    
    sound.setPan(10.5);
    
    numClick = 0;
    status = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    sound.update();
    if(numClick > 4){
        numClick = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //not calling stat true when triggered
    sound.draw();

    //Text or prompt
    ofSetColor(ofColor::white);
    verdana14.drawString("We yearn to say things we wish", ofGetWidth()/3, ofGetHeight()/5);
    verdana14.drawString("at a certain moment in our lives.", ofGetWidth()/3, ofGetHeight()/5 + 15);
    verdana14.drawString("Relieve yourself from your unsaid words.", ofGetWidth()/3*2, ofGetHeight()/5*4);
    verdana14.drawString("Share your moments with us.", ofGetWidth()/3*2, ofGetHeight()/5*4 +15);
    
    //Directions
    verdana14.drawString("Spacebar - start", ofGetWidth()/5 * 2 - 20, ofGetHeight() - 20);
    verdana14.drawString("S - stop", ofGetWidth()/5 * 2 + 150, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case ' ':
            //visual.ON();
            status = true;
            sound.startRecording();
            //sound.trackTrig();
            sound.stopAudio();
            numClick++;
            break;
        case 's':
            //visual.OFF();
            status = false;
            sound.stopRecording();
            sound.startAudio();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    if(numClick == 1){
    sound.audioIn1(input, bufferSize, nChannels);
    }
    else if(numClick == 2){
    sound.audioIn2(input, bufferSize, nChannels);
    }
    else if(numClick == 3){
    sound.audioIn3(input, bufferSize, nChannels);
    }
    else{
        sound.trackTrig();
    }
    sound.volGrab(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    //Default
    sound.audioOut1(output, bufferSize, nChannels);
    sound.audioOut2(output, bufferSize, nChannels);
    sound.audioOut3(output, bufferSize, nChannels);
    //Audio effects
    //sound.echoEffect(output, bufferSize, nChannels);
}

