#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofSetLineWidth(1);
	ofNoFill();

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	int depth = max(ofGetWidth(), ofGetHeight());

	for (int i = 0; i < 512; i++) {
		Particle* particle = new Particle(ofVec3f(ofRandom(-ofGetWidth(), ofGetWidth()), ofRandom(-ofGetHeight(), ofGetHeight()), ofRandom(-depth, depth)), 200, ofRandom(50, 255), ofRandom(50, 255), ofRandom(50, 255));
		this->particles.push_back(particle);
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	for (int i = 0; i < this->particles.size(); i++) {
		this->particles[i]->think();
		if (!ofGetKeyPressed()) {
			this->particles[i]->flok(this->particles);
			this->particles[i]->borders();
		}
		this->particles[i]->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	this->cam.begin();
	
	for (int i = 0; i < this->particles.size(); i++) {
		this->particles[i]->draw();
	}

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
