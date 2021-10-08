#include "ofApp.h"

void ofApp::setup() 
{
	player = std::make_unique<ExtremeGPUVideoPlayer>();
	player->load("visual_short.gv", "sound_short.mp3");
	player->play();

	ofSetWindowShape(player->getWidth(), player->getHeight());
}
void ofApp::update() 
{
	player->update();
}
void ofApp::draw() 
{
	player->draw(0, 0);
}
void ofApp::keyPressed(int key) 
{
	if (key == 's')
		player->stop();
	if (key == 'p')
		player->play();

	if (key == ' ')
		player->setPaused(!player->isPaused());
}
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}