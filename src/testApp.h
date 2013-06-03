/*
 *  testApp.h
 *  A simple mediaPlayer made with OpenFrameworks
 *  Homepage : http://www.fluate.net/code/Pinacotek
 *  Source : https://github.com/fluaten/Pinacotek
 *  Created by Nicolas Boillot on May 2013.
 *  Made with ofxFadable by Rui Madeira
 *  Made with ofxNotify by Tim
 *  Made with ofxExtras by Companje
 *  Made with ofxIniSettings by Companje
 *  Copyright 2012-2013 fluaten. All rights reserved.
 */


#ifndef _PINACOTEK
#define _PINACOTEK

#define GROWL				// Disable notification

#include "ofMain.h"

#include "ofxFadable.h"
#include "ofxNotify.h"

#include "ofxExtras.h"
#include "ofxIniSettings.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();
    
    void LoadConfig();
    void LoadMedia();
    void ShowSettings();

	float fadeTime, playerSpeed, volume;
	int mediaWidth, mediaHeight, framerate;
    int brightness, brightnessLevel;
	int screenWidth,screenHeight, mediaPosition, timerDuration;
	int	XresizeScreenWidth, YresizeScreenHeight;
    int SizescreenHeight, SizescreenWidth;
    int mediaPositionMax, mediaPositioninverse, startTime, actualTime;
    int totalFrameNumber, delayBlack, NotifyTime;
    int redBackgroundColor,greenBackgroundColor,blueBackgroundColor;
	bool StartfullScreen, stopLoop, notification, sortInverse;
    bool BackgroundImage, diaporamaMode, Zoom, reverse, growl;
    string mediaFileName, titleWindow, fpsnotify, frameid;
    string path, backgroundImagePath, mediaExtAllowed;
    
    ofxIniSettings conf;
    ofDirectory mediaFolder;
    
    ofxFadableVideo mediaPlayer, BackImage;
    ofImage	ExportPng;

};

#endif
