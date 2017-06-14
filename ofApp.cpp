#include "ofApp.h"
#define SIZE 42336000//256
//Only 44100 Hz or up(48000) Hz
#define RATE 44100 //samples per sec
#define FRAMERATE 46
#define RAD 15
#define DPI 72
#define FS 24


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
    
    numClick = 0;
    status = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    sound.update();
    if(numClick > 3){
        numClick = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //not calling stat true when triggered
    sound.draw();

    //Text or prompt
    ofSetColor(ofColor::aliceBlue);
    verdana14.drawString("We yearn to say things we wish,", ofGetWidth()/8, ofGetHeight()/5);
    verdana14.drawString("At a certain moment in our lives.", ofGetWidth()/8, ofGetHeight()/5 + 25);
    verdana14.drawString("Relieve yourself from your unsaid words.", ofGetWidth()/8, ofGetHeight()/5 + 50);
    verdana14.drawString("Share your moments with us.", ofGetWidth()/8, ofGetHeight()/5 + 75);

    //Directions
    //verdana14.drawString("Spacebar(twice) - start", ofGetWidth()/5 * 2 - 20, ofGetHeight() - 20);
    //verdana14.drawString("S - stop", ofGetWidth()/5 * 2 + 150, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case ' ':
            if(status){
                sound.startRecording();
                sound.stopAudio();
                numClick++;
                status = false;
            }
            else{
                sound.stopRecording();
                sound.startAudio();
                status = true;
            }
            break;
        case 's':
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
    sound.volGrab(input, bufferSize, nChannels);
    if(numClick == 1){
        sound.audioIn1(input, bufferSize, nChannels);
    }
    if(numClick == 2){
        sound.audioIn2(input, bufferSize, nChannels);
    }
    if(numClick == 3){
        sound.audioIn3(input, bufferSize, nChannels);
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    sound.audioOut(output, bufferSize, nChannels);
}

