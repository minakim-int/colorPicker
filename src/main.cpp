#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings; 
	settings.setSize(500, 500);
	settings.windowMode = OF_WINDOW; 
	settings.resizable = false;
	ofCreateWindow(settings);
	
	ofRunApp(new ofApp());

}
