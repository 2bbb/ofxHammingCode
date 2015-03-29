#pragma once

#include "ofMain.h"
#include "ofxHammingCode.h"

class ofApp : public ofBaseApp {
public:
    void setup() {
        testH74();
        cout << endl;
        testH74SECDED();
        ofExit();
    }
    
    void testH74() {
        ofLogNotice() << "H74" << endl;
        uint8_t d;
        d = 175;
        
        ofLogNotice() << "raw data: " << (int)d;
        uint16_t h = ofxHammingCode::H74::encode(d);
        ofLogNotice() << "encoded data: " << h;
        
        if(ofxHammingCode::H74::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        cout << endl;

        h = ofxHammingCode::H74::encode(d);
        h ^= 4;
        ofLogNotice() << "add 1bit noise: " << h;
        
        if(ofxHammingCode::H74::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        ofxHammingCode::H74::correct(h);
        ofLogNotice() << "corrected data: " << h;
        
        if(ofxHammingCode::H74::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        cout << endl;
        
        h = ofxHammingCode::H74::encode(d);
        h ^= 5;
        ofLogNotice() << "add 2bit noise (this will be judged incorrect and can't correct): " << h;
        
        if(ofxHammingCode::H74::isCorrect(h)) {
            ofLogNotice() << "decoded: (but, incorrect) " << (int)ofxHammingCode::H74::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
            
            ofxHammingCode::H74::correct(h);
            ofLogNotice() << "corrected data (but, incorrect) :  " << h;
            
            if(ofxHammingCode::H74::isCorrect(h)) {
                ofLogNotice() << "decoded (but, incorrect) : " << (int)ofxHammingCode::H74::decode(h);
            } else {
                ofLogNotice() << "incorrect data";
            }
        }
    }
    
    void testH74SECDED() {
        ofLogNotice() << "H74SECDED [single error correction, double error detection]" << endl;
        uint8_t d;
        d = 175;
        
        ofLogNotice() << "raw data: " << (int)d;
        uint16_t h = ofxHammingCode::H74SECDED::encode(d);
        ofLogNotice() << "encoded data: " << h;
        
        if(ofxHammingCode::H74SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        cout << endl;
        
        h = ofxHammingCode::H74SECDED::encode(d);
        h ^= 4;
        ofLogNotice() << "add 1bit noise: " << h;
        
        if(ofxHammingCode::H74SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        ofxHammingCode::H74SECDED::correct(h);
        ofLogNotice() << "corrected data: " << h;
        
        if(ofxHammingCode::H74SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        cout << endl;
        
        h = ofxHammingCode::H74SECDED::encode(d);
        h ^= 5;
        ofLogNotice() << "add 2bit noise: " << h;
        
        if(ofxHammingCode::H74SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
            
            if(ofxHammingCode::H74SECDED::isCorrectable(h)) {
                ofLogNotice() << "this data is correctable";
                
                ofxHammingCode::H74SECDED::correct(h);
                ofLogNotice() << "corrected data: " << h;
                
                if(ofxHammingCode::H74SECDED::isCorrect(h)) {
                    ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74SECDED::decode(h);
                } else {
                    ofLogNotice() << "incorrect data";
                }
            } else {
                ofLogNotice() << "this data can't correctable";
            }
        }
    }
};
