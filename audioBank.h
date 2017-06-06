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
#define GAIN 0.25
#define VOL 3
#define LENGTH 441000
#define DURATION 0.2
#define MAXSECONDS 30
#define VOLUMESTORAGE 400
#define BANKSIZE 3
#define SECONDS 60

class audioBank{
public:
    int size, rate, x, y, r;
    int recPos, r1, r2;
    int playPos, p1, p2;
    int numClick, seconds;
    
    bool rstat, stat;
    float tempL, tempR;
    float s1[LENGTH], s2[LENGTH], s3[LENGTH];
    float b1Length, b2Length, b3Length;
    float offset, buff1, buff2, buff3;
    
    ofSoundStream inputS;
    ofSoundStream outputS;
    
    vector <float> volHist, inputL, inputR;
    
    float smoothedVol = 0.0;
    float scaledVol = 0.0;
    int N = LENGTH * DURATION;
    int sec = ofGetSeconds();
    
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

        //numClick = 0;
        seconds = 0;
        
    }
      //-----------------------------------------------------------------------------------------
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
        if(numClick == 1){
            recPos = 0;
        }
        else if(numClick == 2){
            r1 = 0;
        }
        else if (numClick == 3){
            r2 = 0;
        }
        else{
            //do nothing T.T
        }
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
        if(numClick == 1){
            playPos = 0;
        }
        else if(numClick == 2){
            p1 = 0;
        }
        else {
            p2 = 0;
        }
    }
    void stopAudio(){
        outputS.stop();
    }
    int getLength(float length){
        offset = length;
        return offset;
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
                //Write inputs into positions
                if(recPos < LENGTH -1){
                    s1[recPos] += input[i * 2];     //writes out input from left channel
                    s1[recPos] += input[i * 2 + 1]; //writes out input from right channel
                    recPos++;
                }
                else{
                    rstat = false;
                    startAudio();
                }
            }
            b1Length = recPos;
        }//end
    }
    void audioIn2(float * input, int bufferSize, int nChannels){
        for(int i = 0; i < bufferSize; i++){
            if(r1 < LENGTH - 1){
                s2[r1] += input[i * 2];
                s2[r1] += input[i * 2 +1];
                r1++;
            }
            else{
                rstat = false;
                startAudio();
            }
        }
        b2Length = r1;
    }
    void audioIn3(float * input, int bufferSize, int nChannels){
        for(int i = 0; i < bufferSize; i++){
            if(r2 < LENGTH -1){
                s3[r2] += input[i * 2];
                s3[r2] += input[i * 2 +1];
                r2++;
            }
            else{
                rstat = false;
                startAudio();
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
     * Playback of sounds, one at a time when audio is stopped
     */
    void audioOut(float * output, int bufferSize, int nChannels){
        if(!rstat){
            //Recalling the entire buffer of the stream
            for(int i = 0; i < bufferSize; i++){
                if(playPos < b1Length){
                    output[i * nChannels] += s1[playPos] * GAIN;
                    output[i * nChannels + 1] += s1[playPos] * GAIN;
                    playPos++;
                }
                playPos %= N; //loops <-- allows all 3 recording to overlap and play simultanously
                if(p1 < b2Length){
                    //Altered audio
                    output[i * nChannels] += s2[p1] * VOL;
                    output[i * nChannels + 1] += s2[p1] * VOL;
                    p1++;
                }
                p1 %= N;  //loops
                if(p2 < b3Length){
                    //Altered audio
                    output[i * nChannels] +=  s3[p2] * GAIN;
                    output[i * nChannels + 1] += s3[p2] * GAIN;
                    p2++;
                }
                p2 %= N;  //loops
            }
        }
    }
    //-----------------------------------------------------------------------------------------
    /*
     * adds ontop on audio // Memory leaks<-- might not use
     */
    void echoEffect(float * output, int bufferSize, int nChannels){
        if(!rstat){
            float audio1 = getLength(b1Length);
            float audio2 = getLength(b2Length);
            float audio3 = getLength(b3Length);
            //play same audio at offset sample position
            for(int i = 0; i < bufferSize*nChannels; i++){
                //silence delayed until copies plays
                output[i * nChannels] = 0;
                output[i * nChannels +1] = 0;
                int e1 = 0;
                int e2 = 0;
                int e3 = 0;
                if(playPos == b1Length/10){
                    if(e1 < b1Length){
                        //Altered audio
                        output[i * nChannels] =  s1[e1] * 0.2 * VOL; //* -1; //simulate echo effect
                        output[i * nChannels + 1] = s1[e1] * 0.2 * VOL;
                        e1++;
                    }
                }

            }//end of loop
        }
    }
};
#endif /* audioBank_h */
