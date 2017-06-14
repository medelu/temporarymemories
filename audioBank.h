//
//  audioBank.h
//  project5 - a combo of project1 and project3 for user testing
//
//  Created by Mercedes  Luangamath on 5/17/17.
//
//

#ifndef audioBank_h
#define audioBank_h

#define GAIN 0.25
#define VOL 3
#define LENGTH 441000
#define DURATION 0.2
#define VOLUMESTORAGE 250


class audioBank{
public:
    
    ofSoundStream inputS;
    ofSoundStream outputS;
    
    int numONClick, numOFFClick, size, rate, x, y, r;
    //playback/recording variables
    int recPos1, recPos2, recPos3;
    int playPos1, playPos2, playPos3;
    int N = LENGTH * DURATION;
    bool recStat;
    //buffer stream the the audio will be stored into
    float s1[LENGTH], s2[LENGTH], s3[LENGTH];
    //the lenght of each buffer after recorded
    float b1Length, b2Length, b3Length;
    //Draw variables
    vector <float> volHist, inputL, inputR;
    float smoothedVol = 0.0;
    float scaledVol = 0.0;
    
    //-----------------------------------------------------------------------------------------
    audioBank(){};
    
    audioBank(int sampleSize, int sampleRate, ofBaseApp * app){
        size = sampleSize;  //also the buffer size
        rate = sampleRate;
        recStat = false;
        //Check variable for playback and recording audio
        numONClick = 0;
        numOFFClick = 0;
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
    }
      //-----------------------------------------------------------------------------------------
    //Trigger methods for recording
    void startRecording(){
        if(numONClick <= 3){
            recStat = true;
            inputS.start();
            numONClick++;
            cout << " ON";
            cout << numONClick;
            if(numONClick == 1){
                recPos1 = 0;
            }
            if(numONClick == 2){
                recPos2 = 0;
            }
            if (numONClick == 3){
                recPos3 = 0;
            }
        }
        else{
            numONClick = 0;
        }
    }
    void stopRecording(){
        recStat = false;
        inputS.stop();
    }
    //Playback/output stream to start
    void startAudio(){
        if(numOFFClick <= 3){
            outputS.start();
            numOFFClick++;
            cout << " OFF";
            cout << numOFFClick;
            if(numOFFClick == 1){
                playPos1 = 0;
            }
            if(numOFFClick == 2){
                playPos2 = 0;
            }
            if (numOFFClick == 3){
                playPos3 = 0;
            }
        }
        else{
            numOFFClick = 0;
        }
    }
    void stopAudio(){
        outputS.stop();
    }
     //-----------------------------------------------------------------------------------------
    /**
     * Visualization/animation of voice being recorded
     */
    void update(){
        if(recStat){
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
        if(recStat){
            //Saves changes done to the graphics
            ofPushStyle();
            //Save changes the coordinate of the positon in relation to window
            ofPushMatrix();
                ofTranslate(15.0, 15.0);
                //Overall visual dynamic of pulsing circle
                ofSetColor(ofColor:: orange);
                ofFill();
                ofSetCircleResolution(60);
                ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, scaledVol * 120.0f);
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
        if(recStat){
            //Store left and right input sounds into left&right input array
            for(int i = 0; i < bufferSize; i++){
                //Write inputs into positions
                if(recPos1 < LENGTH - 1){
                    s1[recPos1] += input[i * 2];     //writes out input from left channel
                    s1[recPos1] += input[i * 2 + 1]; //writes out input from right channel
                    recPos1++;
                }
                else{
                    recStat = false;
                }
            }
            b1Length = recPos1;
        }//end
    }
    void audioIn2(float * input, int bufferSize, int nChannels){
        for(int i = 0; i < bufferSize; i++){
            if(recPos2 < LENGTH - 1){
                s2[recPos2] += input[i * 2];
                s2[recPos2] += input[i * 2 +1];
                recPos2++;
            }
            else{
                recStat = false;
            }
        }
        b2Length = recPos2;
    }
    void audioIn3(float * input, int bufferSize, int nChannels){
        for(int i = 0; i < bufferSize; i++){
            if(recPos3 < LENGTH -1){
                s3[recPos3] += input[i * 2];
                s3[recPos3] += input[i * 2 +1];
                recPos3++;
            }
            else{
                recStat = false;
            }
        }
        b3Length = recPos3;
    }
    /*
     * Animates your voice that is being recorded through a pulsing circle
     */
    void volGrab(float * input, int bufferSize, int nChannels){
        if(recStat){
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
        if(!recStat){
            //Recalling the entire buffer of the stream
            for(int i = 0; i < bufferSize; i++){
                output[i] = 0;
                if(playPos1 < b1Length){
                    output[i * nChannels] += s1[playPos1] * GAIN;
                    output[i * nChannels + 1] += s1[playPos1] * GAIN;
                    playPos1++;
                }
                else{
                    playPos1 %= N; //loops <-- allows all 3 recording to overlap and play simultanously
                }
                if(playPos2 < b2Length){
                    //Altered audio
                    output[i * nChannels] += s2[playPos2] * VOL;
                    output[i * nChannels + 1] += s2[playPos2] * VOL;
                    playPos2++;
                }
                else{
                    playPos2 %= N;  //loops
                }
                if(playPos3 < b3Length){
                    //Altered audio
                    output[i * nChannels] +=  s3[playPos3] * GAIN;
                    output[i * nChannels + 1] += s3[playPos3] * GAIN;
                    playPos3++;
                }
                else{
                    playPos3 %= N;  //loops
                }
            }
        }
    }

};
#endif /* audioBank_h */
