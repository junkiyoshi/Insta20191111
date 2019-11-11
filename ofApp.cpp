#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->color = ofColor(39);
	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();

	for (auto i = 0; i < 150; i++) {
	
		this->setLineToMesh(this->mesh, 30, 250);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);

	this->mesh.drawWireframe();

	ofSetColor(this->color);
	ofDrawSphere(glm::vec3(), 50);

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setLineToMesh(ofMesh& mesh, float start_radius, float end_radius) {

	int start_index = mesh.getVertices().size();

	auto noise_seed_x = ofRandom(1000);
	auto noise_seed_y = ofRandom(1000);
	auto noise_seed_z = ofRandom(1000);
	auto base_deg = ofRandom(360);
	for (auto radius = start_radius; radius < end_radius; radius += 5) {

		auto location = glm::vec3(radius * cos(base_deg * DEG_TO_RAD), radius * sin(base_deg * DEG_TO_RAD), 0);

		auto angle_x = ofMap(ofNoise(noise_seed_x, radius * 0.0003 - ofGetFrameNum() * 0.0003), 0, 1, -PI * 2, PI * 2);
		auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));

		auto angle_y = ofMap(ofNoise(noise_seed_y, radius * 0.0003 - ofGetFrameNum() * 0.0003), 0, 1, -PI * 2, PI * 2);
		auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));

		auto angle_z = ofMap(ofNoise(noise_seed_z, radius * 0.0003 - ofGetFrameNum() * 0.0003), 0, 1, -PI * 2, PI * 2);
		auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

		mesh.addVertex(glm::vec4(location, 0) * rotation_z * rotation_y * rotation_x);
		
		auto threshold = start_radius + (end_radius - start_radius) * 0.6;
		if (radius > threshold) {
		
			mesh.addColor(ofColor(this->color, ofMap(radius, threshold, end_radius, 255, 0)));
		}
		else {

			mesh.addColor(this->color);
		}
	}

	for (auto i = start_index; i < mesh.getVertices().size() - 1; i++) {

		mesh.addIndex(i);
		mesh.addIndex(i + 1);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}