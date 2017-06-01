//
//  audioBank.h
//  project5 - a combo of project1 and project3 for user testing
//
//  Created by Mercedes  Luangamath on 5/17/17.
//
//

#ifndef audioBank_h
#define audioBank_h
#include <array>
#define GAIN 0.5
#define VOL 1.5
#define LENGTH 441000//1000000// //dependent on sample size
#define DURATION 0.25
#define MAXSECONDS 30
#define VOLUMESTORAGE 400
#define BANKSIZE 3
#define SECONDS 60

class audioBank{
public:
    int size, rate, recPos, r1, r2;
    int playPos, p1, p2;
    int x, y, r, numClick, seconds;
    
    bool rstat, stat;
    float tempL, tempR, pan;
    float buffer[LENGTH], b1[LENGTH], b2[LENGTH];

    
    ofSoundStream inputS;
    ofSoundStream outputS;
    
    vector <float> volHist;
    vector <float> inputL, out1L, out2L;
    vector <float> inputR, out1R, out2R;
    
    float smoothedVol = 0.0;
    float scaledVol = 0.0;
    int N = LENGTH * DURATION;
    int sec = ofGetSeconds();
    float b1Length, b2Length, b3Length;
    
    //-----------------------------------------------------------------------------------------
    audioBank(){};
    
    audioBank(int sampleSize, int sampleRate, ofBaseApp * app){
        size = sampleSize;  //also the buffer size
        rate = sampleRate;
        rstat = false;
        
        //External audio devices checkup
        inputS.printDeviceList();
        inputS.getNumInputChannels();
        outputS.getNumOutputChannels();
        
        inputL.assign(size, 0.0);
        inputR.assign(size, 0.0);
        
        inputS.setup(app, 0, 2, rate, size, 4);
        outputS.setup(app, 2, 0, rate, size, 4);
        
        inputS.stop();
        outputS.stop();
        
        pan = 0;
        //numClick = 0;
        seconds = 0;
        
    }
      //-----------------------------------------------------------------------------------------
    void setPan(float value){
        pan = value;
    }
    //For multi recording track
    void trackTrig(){
        if(numClick == 3){
            numClick = 0;
        }
        numClick++;
        cout << " Num of clicks: ";
        cout << numClick;
        cout << " ";
    }
    //Trigger methods for recording
    void startRecording(){
        rstat = true;
        inputS.start();
        cout << " ON";
    }
    void stopRecording(){
        rstat = false;
        inputS.stop();
        cout << " OFF";
    }
    //Playback/output stream to start
    void startAudio(){
        outputS.start();
    }
    void stopAudio(){
        outputS.stop();
    }
     //-----------------------------------------------------------------------------------------
    /**
     * Visualization/animation of voice to be recorded
     */
    void update(){
        if(rstat){
            //scaling the volume from 0-1 range
            scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
            //record the volume data into array
            volHist.push_back(scaledVol);
            //drop oldest vol in the array
            if(volHist.size() >= VOLUMESTORAGE){
                volHist.erase(volHist.begin(), volHist.begin() +1);
            }
        }
    }
    void draw(){
        if(rstat){
            //Saves changes done to the graphics
            ofPushStyle();
            //Save changes the coordinate of the positon in relation to window
            ofPushMatrix();
                ofTranslate(15.0, 15.0);
                //Overall visual dynamic of pulsing circle
                ofSetColor(ofColor:: red);
                ofFill();
                ofSetCircleResolution(60);
                ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, scaledVol * 110.0f);
            ofPopMatrix();
            ofPopStyle();
        }
        else{
            //Removes animation
            ofColor color;
            color.set(ofColor::gray);
            ofClear(color);
        }
    }
    //-----------------------------------------------------------------------------------------
    /*
     * Recieve the input audio stream
     * Recording will be stored in the buffer that will store the sound values as an element
     */
    void audioIn1(float * input, int bufferSize, int nChannels){
        if(rstat){
            //Store left and right input sounds into left&right input array
            for(int i = 0; i < bufferSize; i++){
                //Verison 2 Recording audio using sample positon
                if(recPos < LENGTH){
                    buffer[recPos] = input[i * 2];
                    buffer[recPos] = input[i * 2 + 1];
                    recPos++;
                }
                else{
                    stopRecording();
                    startAudio();
                    rstat = false;
                }
            }
            b1Length = recPos;
        }//end
    }
    void audioIn2(float * input, int bufferSize, int nChannels){
        for(int i = 0; i < bufferSize; i++){
            if(r1 < LENGTH){
                b1[r1] = input[i * 2];
                b1[r1] = input[i * 2 +1];
                r1++;
            }
            else{
                stopRecording();
                startAudio();
                rstat = false;
            }
        }
        b2Length = r1;
    }
    void audioIn3(float * input, int bufferSize, int nChannels){
        for(int i = 0; i < bufferSize; i++){
            if(r2 < LENGTH){
                b2[r2] = input[i * 2];
                b2[r2] = input[i * 2 +1];
                r2++;
            }
            else{
                stopRecording();
                startAudio();
                rstat = false;
            }
        }
        b3Length = r2;
    }
    /*
     * Animates your voice that is being recorded through a pulsing circle
     */
    void volGrab(float * input, int bufferSize, int nChannels){
        if(rstat){
            float rms = 0.0;
            int numCounted = 0;
            //grabing and storing the values of live audio
            for(int i= 0; i < bufferSize; i++){
                float sampleL = input[i * 2];
                float sampleR = input[i * 2 + 1];
                //grabbing the sound values from left and right channels
                rms += sampleL * sampleL;
                rms += sampleR * sampleR;
                numCounted +=2;
            }
            //Mean of rms
            rms /= (float)numCounted;
            //Root of rms
            rms = sqrt(rms);
            //avg sample values will be filtered to visual animation
            smoothedVol *= 0.93;
            smoothedVol += 0.05 * rms;
        }
    }
     //-----------------------------------------------------------------------------------------
    /*
     * Default playback of sound when audio is stopped
     */
    void audioOut1(float * output, int bufferSize, int nChannels){
        if(!rstat){
            //Recalling the entire buffer of the stream
            for(int i = 0; i < bufferSize; i++){
                //Version2 with specifc position of the array
                if(playPos < LENGTH){
                    output[i * nChannels] = buffer[playPos] * GAIN; //* -1; //simulate echo effect
                    output[i * nChannels + 1] = buffer[playPos] * GAIN;
                    playPos++;
                }
            }//end of loop
        }
    }
    void audioOut2(float * output, int bufferSize, int nChannels){
        if(!rstat){
            //Recalling the entire buffer of the stream
            for(int i = 0; i < bufferSize; i++){
                if(p1 < b2Length){
                    //Altered audio
                    output[i * nChannels] =  b1[p1] * GAIN; //* -1; //simulate echo effect
                    output[i * nChannels + 1] = b1[p1] * GAIN;
                    p1++;
                }
            }//end of loop
        }
    }
    void audioOut3(float * output, int bufferSize, int nChannels){
        if(!rstat){
            //Recalling the entire buffer of the stream
            for(int i = 0; i < bufferSize; i++){
                if(p2 < b3Length){
                    //Altered audio
                    output[i * nChannels] =  b2[p2] * GAIN; //* -1; //simulate echo effect
                    output[i * nChannels + 1] = b2[p2] * GAIN;
                    p2++;
                }
            }//end of loop
        }
    }
    //-----------------------------------------------------------------------------------------
    /*
     * adds ontop on audio // Memory leaks
     */
    void echoEffect(float * output, int bufferSize, int nChannels){
        //if(!rstat){
            for(int i = 0; i < bufferSize*nChannels; i++){
                if(playPos < b1Length){
                    //Altered audio
                    output[i * nChannels] =  buffer[playPos] * 0.2 * VOL; //* -1; //simulate echo effect
                    output[i * nChannels + 1] = buffer[playPos] * 0.2;
                    playPos++;
                }
                //Will loop the sound, prevent clicking
                playPos %= N;
                if(p1 < b2Length){
                    //Altered audio
                    output[i * nChannels] =  b1[p1] * 0.2 * VOL; //* -1; //simulate echo effect
                    output[i * nChannels + 1] = b1[p1] * 0.2;
                    p1++;
                }
                //Will loop the sound, prevent clicking
                p1 %= N;
                if(p2 < b3Length){
                    //Altered audio
                    output[i * nChannels] =  b2[p2] * 0.2 * VOL; //* -1; //simulate echo effect
                    output[i * nChannels + 1] = b2[p2] * 0.2;
                    p2++;
                }
                //Will loop the sound, prevent clicking
                p2 %= N;
            }//end of loop
        }
    //}
};
#endif /* audioBank_h */
