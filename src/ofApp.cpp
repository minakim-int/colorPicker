#include "ofApp.h"
#include <algorithm>

void startImage() {
	ofDrawBitmapString("[+]", 250, 100);
	ofDrawBitmapString("Drop your Image and", 180, 130);
	ofDrawBitmapString("Press ENTER to process", 170, 160);
}
void loadingImage() {
	ofBackground(0);
	ofDrawBitmapString("Loading...", 250, 250);
}
bool compareHue(const ofColor& a, const ofColor& b) {
	return a.getHue() < b.getHue();
}
bool compareSaturation(const ofColor& a, const ofColor& b) {
	return a.getSaturation() < b.getSaturation();
}
bool compareBrightness(const ofColor& a, const ofColor& b) {
	return a.getBrightness() < b.getBrightness();
}
template <typename T>
void ofApp::getColors(vector<T>& v, const ofImage& img, ofPixels& pix) {
	int imgW = img.getWidth();
	int imgH = img.getHeight();
	initial = false;
	v.resize(imgW*imgH); //init vector container
	pix = img.getPixels();
	for (int i = 0, idx = 0; i <imgW; i++) {
		for(int j= 0; j<imgH; j++){
			v[idx] = pix.getColor(i, j);
			idx++;
		}
		if (idx+1 == imgW * imgH) break;
	}
}
template <typename T>
void ofApp::indexCheck(const vector<T>& v, int startIdx, int endIdx, int store[CLUSTER_NUM], char mode) {
	int j = 0;
	int testHue;
	int testSat;
	int testVal;
	switch (mode) {
	case 'h':
		testHue = v[startIdx].getHue();
		//hue의 range(10단위)별로 원소가 몇 개 있나 확인하기
		store[CLUSTER_NUM] = { 0 };
		//int j = 0; //changedIndex배열용
		for (int i = 0; i < v.size(); i++) {
			//colors 전체 돌면서 hue 10의자리 바뀔때마다 index 체크해서 배열에 넣음
			int tempHue = v[i].getHue();
			if (tempHue / THR != testHue / THR) { //10으로 나누었을 때의 몫이 달라지는 시점
				store[j] = i;
				printf("Hue changed in %d, to %d\n", i, tempHue);
				j++;
				testHue += THR;
			}
		}
		break;

	case 's':
		testSat = v[startIdx].getSaturation();
		//hue의 range(10단위)별로 원소가 몇 개 있나 확인하기
		store[CLUSTER_NUM] = { 0 };
		//int j = 0; //changedIndex배열용
		for (int i = startIdx; i < endIdx; i++) {
			//colors 전체 돌면서 hue 10의자리 바뀔때마다 index 체크해서 배열에 넣음
			int tempSat = v[i].getSaturation();
			if (tempSat / THR != testSat / THR) { //10으로 나누었을 때의 몫이 달라지는 시점
				store[j] = i;
				printf("Hue[%d~%d] Saturation changed in %d | to sat %d\n", startIdx, endIdx, i, tempSat);
				j++;
				testSat += THR;
			}
		}
		break;

	case 'v':
		testVal = v[startIdx].getBrightness();
		store[CLUSTER_NUM] = { 0 };
		//int j = 0; //changedIndex배열용
		for (int i = startIdx; i < endIdx; i++) {
			//colors 전체 돌면서 hue 10의자리 바뀔때마다 index 체크해서 배열에 넣음
			int tempVal = v[i].getBrightness();
			if (tempVal / THR != testVal / THR) { //10으로 나누었을 때의 몫이 달라지는 시점
				store[j] = i;
				printf("Sat[%d~%d] Brightness changed in %d | to val %d\n", startIdx, endIdx, i, tempVal);
				j++;
				testVal += THR;
			}
		}
		break;
	}
}

typedef struct forcmp{
	pair<int, int> idx;
	int gap;
};

bool compareGap(const forcmp& a, const forcmp& b){
	return a.gap > b.gap;
}
void Ranking(const int index[CLUSTER_NUM], vector<pair<int, int>>& result, char mode) {
	forcmp arrforcmp[CLUSTER_NUM - 1];
	for (int i = 0; i < CLUSTER_NUM-1; i++) {
		arrforcmp[i].gap = index[i + 1] - index[i];
		arrforcmp[i].idx.first = index[i];
		arrforcmp[i].idx.second = index[i + 1];
	}
	sort(arrforcmp, arrforcmp + CLUSTER_NUM-1, compareGap);//CLUSTER_NUM-1
	for (int i = 0; i < PICKNUM; i++) {
		cout << i << "th dominant color is in index " << arrforcmp[i].idx.first <<"\n";
	}
	switch (mode) {
	case 'h':
		for (int i = 0; i < PICKNUM; i++) {
			result.push_back(arrforcmp[i].idx);
		}
		break;
	case 's':
	case 'v':
		result.push_back(arrforcmp[0].idx);
		break;
	}
	
}
void Ranking(const int index[CLUSTER_NUM], vector<pair<int, int>>& result, char mode, const vector<ofColor>& v) {
	forcmp arrforcmp[CLUSTER_NUM - 1];
	for (int i = 0; i < CLUSTER_NUM-1; i++) {
		arrforcmp[i].gap = index[i + 1] - index[i];
		arrforcmp[i].idx.first = index[i];
		arrforcmp[i].idx.second = index[i + 1];
	}
	sort(arrforcmp, arrforcmp + CLUSTER_NUM-1, compareGap);//CLUSTER_NUM-1
	for (int i = 0; i < PICKNUM; i++) {
		cout << i << "th dominant color is in index " 
			<< arrforcmp[i].idx.first << " HSB: "<< v[arrforcmp[i].idx.first].getHue() << " "
			<<v[arrforcmp[i].idx.first].getSaturation() << " " << v[arrforcmp[i].idx.first].getBrightness()<<"\n";
	}
	switch (mode) {
	case 'h':
		for (int i = 0; i < PICKNUM; i++) {
			result.push_back(arrforcmp[i].idx);
		}
		break;
	case 's':
	case 'v':
		result.push_back(arrforcmp[0].idx);
		break;
	}
	
}

void setPallete(int index[CLUSTER_NUM]) {
	//가장 차이가 큰 구역의 시작 인덱스, 끝 인덱스 구해서 
	//colors 벡터 그 인덱스 사이에서만 compareSaturation하기

}
//--------------------------------------------------------------
void ofApp::setup(){
	initial = true;
	imgLoaded = false;
	fin = false;
	loading = false;
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	//---------Default Window-----------//
	if (initial) {
		startImage();
		if (loading) {
		loadingImage();
		}
	}
	//---------Loading---------//
	//----------After Opening File-------------//
	if (fin) {
		ofSetColor(pallete[0]);
		ofDrawRectangle(0, 0, 250, 500);
		ofSetColor(pallete[1]);
		ofDrawRectangle(250, 0, 500, 250);
		ofSetColor(pallete[2]);
		ofDrawRectangle(250, 250, 375, 500);
		ofSetColor(pallete[3]);
		ofDrawRectangle(375, 250, 500, 375);
		ofSetColor(pallete[4]);
		ofDrawRectangle(375, 375, 500, 500);

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (imgLoaded&&key == OF_KEY_RETURN) {
		loading = true;
	}
	if (imgLoaded && key == OF_KEY_RETURN) {
		cout << "getting Colors...\n";
		getColors(colors, img, pix);
		cout << "sorting Colors...\n";
		ofSort(colors, compareHue);
		cout << "printing index\n";
		indexCheck(colors, 0, colors.size(), changedHueIndex, 'h');
		Ranking(changedHueIndex, idxforsat, 'h', colors); //Hue Ranking
		
		for (int i = 0; i < idxforsat.size(); i++) {
			sort(&colors[idxforsat[i].first], &colors[idxforsat[i].second], compareSaturation);
			indexCheck(colors, idxforsat[i].first, idxforsat[i].second, changedSatIndex, 's');
			Ranking(changedSatIndex, idxforval, 's', colors); 

			sort(&colors[idxforval[i].first], &colors[idxforval[i].second], compareBrightness);
			indexCheck(colors, idxforval[i].first, idxforval[i].second, changedValIndex, 'v');
			Ranking(changedValIndex, idxforpallete, 'v', colors);
			
			cout << "color pushed in idx " << i << "\n";
			pallete.push_back(colors[idxforpallete[i].first]);
			cout << pallete[i].getHue() << " " << pallete[i].getSaturation() <<" " << pallete[i].getBrightness()<<"\n";
		}

		fin = true;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (fin) {
		if (changeTest < sizeof(changedHueIndex) / sizeof(int));
			changeTest++;
	}
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
	if (dragInfo.files.size() > 0) {
		filesize = dragInfo.files.size();
		img.load(dragInfo.files[0]);
		filePath = dragInfo.files[0];
		img.resize(img.getWidth()/2, img.getHeight()/2);
		imgLoaded = true;
	}
}
		/*for (int i = 0; i < imgW*imgH; i++) {
			colors[i].r = (originPixelData[i * 3] / 10) * 10;
			colors[i].g = (originPixelData[i * 3 + 1] / 10) * 10;
			colors[i].b = (originPixelData[i * 3 + 2] / 10) * 10;
		}*/
