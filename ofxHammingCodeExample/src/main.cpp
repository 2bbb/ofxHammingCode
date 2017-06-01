#include "ofMain.h"
#include "ofxHammingCode.h"

class ofApp : public ofBaseApp {
public:
    void setup() {
        testH74();
        std::cout << std::endl;
        testH74SECDED();
        std::cout << std::endl;
        test3126(ofRandom(0, 1 << 25));
        std::cout << std::endl;
        testH3126SECDED(ofRandom(0, 1 << 25));
        std::cout << std::endl;
        
        ofExit();
    }
    
    void testH74() {
        ofLogNotice() << "==== H74 ====" << std::endl;
        std::uint8_t d;
        d = 175;
        
        ofLogNotice() << "raw data: " << (int)d;
        std::uint16_t h = ofxHammingCode::H74::encode(d);
        ofLogNotice() << "encoded data: " << h;
        
        if(ofxHammingCode::H74::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        std::cout << std::endl;
        
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
        
        std::cout << std::endl;
        
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
        ofLogNotice() << "==== H74::SECDED ====";
        ofLogNotice() << "[single error correction, double error detection]" << std::endl;
        std::uint8_t d;
        d = 175;
        
        ofLogNotice() << "raw data: " << (int)d;
        std::uint16_t h = ofxHammingCode::H74::SECDED::encode(d);
        ofLogNotice() << "encoded data: " << h;
        
        if(ofxHammingCode::H74::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        std::cout << std::endl;
        
        h = ofxHammingCode::H74::SECDED::encode(d);
        h ^= 4;
        ofLogNotice() << "add 1bit noise: " << h;
        
        if(ofxHammingCode::H74::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        ofxHammingCode::H74::SECDED::correct(h);
        ofLogNotice() << "corrected data: " << h;
        
        if(ofxHammingCode::H74::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        std::cout << std::endl;
        
        h = ofxHammingCode::H74::SECDED::encode(d);
        h ^= 5;
        ofLogNotice() << "add 2bit noise: " << h;
        
        if(ofxHammingCode::H74::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
            
            if(ofxHammingCode::H74::SECDED::isCorrectable(h)) {
                ofLogNotice() << "this data is correctable";
                
                ofxHammingCode::H74::SECDED::correct(h);
                ofLogNotice() << "corrected data: " << h;
                
                if(ofxHammingCode::H74::SECDED::isCorrect(h)) {
                    ofLogNotice() << "decoded: " << (int)ofxHammingCode::H74::SECDED::decode(h);
                } else {
                    ofLogNotice() << "incorrect data";
                }
            } else {
                ofLogNotice() << "this data can't correctable";
            }
        }
    }
    
    void test3126(const std::uint32_t d) {
        ofLogNotice() << "==== H3126 ====" << std::endl;
        
        ofLogNotice() << "raw data: " << d;
        uint32_t h = ofxHammingCode::H3126::encode(d);
        ofLogNotice() << "encoded data: " << h;
        
        if(ofxHammingCode::H3126::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        std::cout << std::endl;
        
        h = ofxHammingCode::H3126::encode(d);
        h ^= 8192;
        ofLogNotice() << "add 1bit noise: " << h << ", " << ofxHammingCode::H3126::decode(h);
        
        if(ofxHammingCode::H3126::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        ofLogNotice() << "do correct";
        ofxHammingCode::H3126::correct(h);
        ofLogNotice() << "corrected data: " << h << ", " << ofxHammingCode::H3126::decode(h);
        
        if(ofxHammingCode::H3126::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
    }
    
    void testH3126SECDED(const std::uint32_t d) {
        ofLogNotice() << "==== H3126::SECDED ====";
        ofLogNotice() << "[single error correction, double error detection]" << std::endl;
        
        ofLogNotice() << "raw data: " << d;
        std::uint32_t h = ofxHammingCode::H3126::SECDED::encode(d);
        ofLogNotice() << "encoded data: " << h;
        
        if(ofxHammingCode::H3126::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        std::cout << std::endl;
        
        h = ofxHammingCode::H3126::SECDED::encode(d);
        h ^= 1024;
        ofLogNotice() << "add 1bit noise: " << h << ", " << ofxHammingCode::H3126::SECDED::decode(h);
        
        if(ofxHammingCode::H3126::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        ofLogNotice() << "do correct";
        ofxHammingCode::H3126::SECDED::correct(h);
        ofLogNotice() << "corrected data: " << h << ", " << ofxHammingCode::H3126::SECDED::decode(h);
        
        if(ofxHammingCode::H3126::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
        }
        
        std::cout << std::endl;
        
        h = ofxHammingCode::H3126::SECDED::encode(d);
        h ^= 5;
        ofLogNotice() << "add 2bit noise: " << h;
        
        if(ofxHammingCode::H3126::SECDED::isCorrect(h)) {
            ofLogNotice() << "decoded: " << ofxHammingCode::H3126::SECDED::decode(h);
        } else {
            ofLogNotice() << "incorrect data";
            
            if(ofxHammingCode::H3126::SECDED::isCorrectable(h)) {
                ofLogNotice() << "this data is correctable";
                
                ofxHammingCode::H3126::SECDED::correct(h);
                ofLogNotice() << "corrected data: " << h;
                
                if(ofxHammingCode::H74::SECDED::isCorrect(h)) {
                    ofLogNotice() << "decoded: " << ofxHammingCode::H3126::SECDED::decode(h);
                } else {
                    ofLogNotice() << "incorrect data";
                }
            } else {
                ofLogNotice() << "this data can't correctable";
            }
        }
    }
};

//========================================================================
int main( ){
	ofSetupOpenGL(480, 480, OF_WINDOW);
	ofRunApp(new ofApp());
}
