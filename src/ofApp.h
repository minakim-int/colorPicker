#pragma once

#include "ofMain.h"

#define THR 30 //set threshold of Hue Value
#define CLUSTER_NUM 256/THR-1
#define PICKNUM 5

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		//----------Loading Image--------------//
		bool initial; //true: before open file
		bool imgLoaded;
		ofImage img;
		int filesize;
		string filePath;

		//------------Image Processing-------//
		bool loading;
		ofPixels pix;
		vector<ofColor> colors;
		bool fin;
		int changedHueIndex[CLUSTER_NUM];
		int changedSatIndex[CLUSTER_NUM];
		int changedValIndex[CLUSTER_NUM];
		int changeTest;
		template <typename T>
		void getColors(vector<T>& v, const ofImage& img, ofPixels& pix);
		template <typename T>
		void indexCheck(const vector<T>& v, int startIdx, int endIdx, int arr[], char mode);
		///////////////////////////////////////////////////
		vector<pair<int, int>> idxforsat;
		vector<pair<int, int>> idxforval;
		vector<pair<int, int>> idxforpallete;
		vector<ofColor> pallete; //Result Colors
		///////////////////////////////////////////////////
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};