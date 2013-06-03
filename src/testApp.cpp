/*
 *  testApp.cpp
 *  A simple mediaPlayer made with OpenFrameworks
 *  Homepage : http://www.fluate.net/code/pinacotek
 *  Source : https://github.com/fluaten/Pinacotek 
 *  Created by Nicolas Boillot on April/May 2013.
 *  Made with ofxFadable by Rui Madeira
 *  Made with ofxNotify by Tim
 *  Made with ofxExtras by Companje 
 *  Made with ofxIniSettings by Companje
 *  Copyright 2012-2013 fluaten. All rights reserved.
 */

#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetDataPathRoot("../../");

    LoadConfig(); // Load setting from settings.conf
    
    ofBackground(redBackgroundColor,greenBackgroundColor,blueBackgroundColor);
    // Color background
    
    startTime = 1; // timer init
    
	ofSetFrameRate(framerate);
    ofSetVerticalSync(true);
    ofSetFullscreen(StartfullScreen); // Start in fullscreen
    
	screenWidth		= ofGetScreenWidth();
	screenHeight	= ofGetScreenHeight();
    
    BackImage.loadMovie(backgroundImagePath); // Load Background
    BackImage.setFadeSeconds(fadeTime); // Fade in/out time
    BackImage.setAlpha(0);
    BackImage.fadeIn(); // Fade in at start
    
    LoadMedia(); // Load first media
    mediaPosition = 0;
    mediaPlayer.setFadeSeconds(fadeTime); // Fade in/out time
    mediaPlayer.setAlpha(0);
    mediaPlayer.fadeIn(); // Fade in at start

#ifdef GROWL
    ofxNotify::setMessagesLifeTime(NotifyTime); // Growl message duration
    ShowSettings(); // Show settings at start
#endif

    
}

//--------------------------------------------------------------
void testApp::update(){
    
    ofSetWindowTitle(titleWindow + " | " + mediaFileName);
    
    /*if (brightness) {
        
        unsigned char * pix = mediaPlayer.getPixels();
        
        //for a 3 channel rgb image
        for(int i = 0; i < mediaPlayer.width * mediaPlayer.height * 3; i++){
            pix[i] += MIN(brightnessLevel, 255-pix[i]); //this makes sure it doesn't go over 255 as it will wrap to 0 otherwise.
        }
    }
    */ 
    
    mediaPlayer.updateFade(); // update media fadein
    mediaPlayer.update(); // update media
    
    if (BackgroundImage) {
        BackImage.updateFade(); // update Background Image fadein
        BackImage.update(); // update Background Image fadein
    }
    
    actualTime = ofGetElapsedTimeMillis(); // Timer actual time
    
    if ((startTime + timerDuration - (fadeTime*1000) - (delayBlack*1000) < actualTime) && (diaporamaMode) && (mediaPositionMax>1) ) {
        mediaPlayer.fadeOut(); // Diaporama mode
    }
    
    if ((startTime + timerDuration < actualTime) && (diaporamaMode) && (mediaPositionMax>1)) {
        // Skip to next media in folder or loop if last file
        if (mediaPosition<=mediaPositionMax-1) {
            mediaPosition++;
        } else {
            mediaPosition = 0;
        }
        
        LoadMedia(); // Load new media from folder
        
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    if (mediaPlayer.isFrameNew() && (stopLoop)) { mediaPlayer.stop();
        // Stop playing media
    }
    
    if (BackgroundImage) {
        BackImage.draw(0, 0, BackImage.getWidth(),BackImage.getHeight());
        // Draw background image
    }
    
    if (ofGetWindowMode() == OF_WINDOW) {
        mediaPlayer.draw(0, 0, mediaWidth, mediaHeight); // Draw media in windows
        
    } else { // Fullscreen media ratio
        
        if ( ((float)mediaWidth/(float)mediaHeight) <= ((float)screenWidth/(float)screenHeight) ) {
			
			SizescreenWidth = (mediaWidth*screenHeight)/mediaHeight;
			SizescreenHeight = screenHeight;
			XresizeScreenWidth = (screenWidth/2) - (SizescreenWidth/2);
			YresizeScreenHeight = 0;
            
		} else {
			
			SizescreenWidth = screenWidth;
			SizescreenHeight = (mediaHeight*screenWidth)/mediaWidth;
			XresizeScreenWidth = 0;
			YresizeScreenHeight = (screenHeight/2) - (SizescreenHeight/2);
		}
		

        if (!Zoom) {
            // Draw max size in fullscreen
             mediaPlayer.draw(XresizeScreenWidth, YresizeScreenHeight,
                           SizescreenWidth, SizescreenHeight);
        } else {
            // Draw original size in fullscreen
            mediaPlayer.draw((screenWidth/2)-(mediaWidth/2), (screenHeight/2)-(mediaHeight/2), mediaWidth, mediaHeight);
            
        }
        
        ofHideCursor(); // Hide cursor
    }
    if (growl) {
        ofxNotify::draw(); // Draw growl style notification
    }
  


    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
			
		case 'f':
		case ' ': // Fullscreen mode
			ofToggleFullscreen();
            
            if (ofGetWindowMode() == OF_FULLSCREEN) {
            
#ifdef GROWL
                ofxNotify() << "Fullscreen " << screenWidth << "x" << screenHeight;
#endif
            } else {
             
#ifdef GROWL
                ofxNotify() << "Window " << mediaWidth << "x" << mediaHeight;
#endif
            }
            
            if (ofGetWindowMode() == OF_WINDOW) {
                ofSetWindowShape(mediaWidth, mediaHeight);
                ofSetWindowPosition((screenWidth/2)- (mediaWidth/2), (screenHeight/2)-(mediaHeight/2));
            }
            
			break;
        case OF_KEY_RIGHT: // Next media
            if (mediaPositionMax>1) {
                if (mediaPosition<mediaPositionMax-1) {mediaPosition++;} else {mediaPosition = 0;}
                LoadMedia();
            }
            break;
        case OF_KEY_LEFT: // Previous media
            if (mediaPositionMax>1) {
                mediaPosition--;
                if (mediaPosition<0) { mediaPosition = mediaPositionMax-1;}
                LoadMedia();
                
            }
            break;

        case OF_KEY_DOWN:
            mediaPlayer.setFrame(mediaPlayer.getCurrentFrame()-(10 * 25));
            
#ifdef GROWL
            if(growl) {
                int mediaPercent = ((float)mediaPlayer.getCurrentFrame() / (float)mediaPlayer.getTotalNumFrames()) * 100;
                ofxNotify() << "Rewind video << " << mediaPercent << " %";
            }
            
#endif
            break;
            
        case OF_KEY_UP:
            mediaPlayer.setFrame(mediaPlayer.getCurrentFrame()+(10 * 25));
#ifdef GROWL
            if(growl) {
                  int mediaPercent = ((float)mediaPlayer.getCurrentFrame() / (float)mediaPlayer.getTotalNumFrames()) * 100;
                ofxNotify() << "Forward video >> " << mediaPercent << " %";
            }
            
#endif
            break;
    
        case 's': // Show settings
            ShowSettings();
            break;
            // Framerate --------------
            
        case 't':
            
#ifdef GROWL
            fpsnotify = ofToString(ofGetFrameRate(), 2)+"fps";
            ofxNotify() << "framerate : " << fpsnotify;
#endif
            break;
            
            
        case 'h':
            framerate--;
            framerate--;
            if (framerate < 1) framerate = 1;
            ofSetFrameRate(framerate);
#ifdef GROWL
            ofxNotify() << "framerate " << framerate;
#endif
            break;
        case 'j':
            framerate++;
            framerate++;
            if (framerate > 100) framerate = 100;
            ofSetFrameRate(framerate);
#ifdef GROWL
            ofxNotify() << "framerate " << framerate;
#endif
            break;
            
            
        case 'k':
            ofSetFrameRate(12);
#ifdef GROWL
            ofxNotify() << "framerate 12";
#endif
            break;
        case 'l':
            ofSetFrameRate(25);
#ifdef GROWL
            ofxNotify() << "framerate 25";
#endif
            break;
        case 'm':
            ofSetFrameRate(30);
#ifdef GROWL
            ofxNotify() << "framerate 30";
#endif
            break;
        case 'M':
            ofSetFrameRate(300);
#ifdef GROWL
            ofxNotify() << "framerate 300";
#endif
       
        case 'w':
                 stopLoop = !stopLoop;
            if (!stopLoop) {
                mediaPlayer.setPaused(false);
            } else {
                mediaPlayer.setPaused(true);
            }
            
#ifdef GROWL
            if (stopLoop) {
                ofxNotify() << "------ stop";
            } else {
                ofxNotify() << "+++++ start";
            }
#endif
            
            break;
            
        case OF_KEY_RETURN:
            stopLoop = !stopLoop;
            if (!stopLoop) {
                mediaPlayer.setPaused(false);
            } else {
                mediaPlayer.setPaused(true);
            }
            
#ifdef GROWL
            if (stopLoop) {
                ofxNotify() << "------ stop";
            } else {
                ofxNotify() << "+++++ start";
            }
#endif
            
            break;
            

        case 'c':
            mediaPlayer.nextFrame();

            
#ifdef GROWL
            ofxNotify() << "next frame";
#endif
   break;
            
        case 'x':
            mediaPlayer.previousFrame();
            
            
#ifdef GROWL
            ofxNotify() << "previous frame";
#endif
            break;
     
            
        case 'd':
            diaporamaMode = !diaporamaMode;
#ifdef GROWL
            if (!diaporamaMode) {
                ofxNotify() << "------ stop diaporama";
            } else {
                ofxNotify() << "+++++ start diaporama";
            }
#endif
            
            break;
            
        case 'z':
            Zoom = !Zoom;
#ifdef GROWL
            if (!Zoom) {
                ofxNotify() << "------ zoom";
            } else {
                ofxNotify() << "++++++ zoom";
            }
#endif
          break;
            
        case 'n':
            sortInverse = !sortInverse;
#ifdef GROWL
            if (!sortInverse) {
                ofxNotify() << "Sort files";
            } else {
                ofxNotify() << "Sort files inverse";
            }
#endif
            break;
            
            
            
            
        case 'v':
            ExportPng.allocate(mediaWidth,mediaHeight,OF_IMAGE_COLOR);
            ExportPng.setFromPixels(mediaPlayer.getPixels(), mediaWidth, mediaHeight, OF_IMAGE_COLOR);
            frameid = ofToString(ofGetFrameNum());
            ExportPng.saveImage(mediaFileName + "_" + frameid + ".png");
            ExportPng.clear();
            
#ifdef GROWL
            ofxNotify() << "PNG | " << mediaFileName << "_" << frameid << ".png";
            ofxNotify() << totalFrameNumber << "f " << mediaWidth << "x" << mediaHeight;
#endif

            break;
            
            
        case 'b':
            BackgroundImage = !BackgroundImage;
#ifdef GROWL
            if (!BackgroundImage) {
                ofxNotify() << "------ No image in background";
            } else {
                ofxNotify() << "+++++++++ Image in background";
            }
#endif
            break;
            
            
        case 'r':
            reverse = !reverse;
            if (reverse) { mediaPlayer.setSpeed(-playerSpeed); // 0.04 = 5s/frame
            } else { mediaPlayer.setSpeed(playerSpeed); // 0.04 = 5s/frame
            }
#ifdef GROWL
            if (reverse) {
                ofxNotify() << "Reverse play";
            } else {
                ofxNotify() << "Normal play";
            }
#endif
            break;
            
            
        
          case 'i': // Info about image  
#ifdef GROWL
            if (notification) {
                
                ofxNotify() << totalFrameNumber << "f "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPosition+1) << "/" << mediaPositionMax;
                ofxNotify() << mediaFileName ;
                
            }
#endif
            break;
        case 'u': // Update page
            ofLaunchBrowser("http://www.fluate.net/code/pinacotek");
        break;
        case '0':
            mediaPlayer.setSpeed(0.01);
			playerSpeed = 0.01;
#ifdef GROWL
            ofxNotify() << "Animation speed 0.01";
			
#endif
            break;
        case '1':
            mediaPlayer.setSpeed(1);
			playerSpeed = 1;
#ifdef GROWL
            ofxNotify() << "Animation speed 1";
			
#endif
            break;
        case '2':
            mediaPlayer.setSpeed(2);
			playerSpeed = 2;
#ifdef GROWL
            ofxNotify() << "Animation speed 2";
#endif
            break;
        case '3':
            mediaPlayer.setSpeed(3);
            playerSpeed = 3;
#ifdef GROWL
            ofxNotify() << "Animation speed 3";
#endif
            break;
        case '4':
            mediaPlayer.setSpeed(4);
			playerSpeed = 4;
#ifdef GROWL
            ofxNotify() << "Animation speed 4";
#endif
            break;
        case '5':
            mediaPlayer.setSpeed(5);
			playerSpeed = 5;
#ifdef GROWL
            ofxNotify() << "Animation speed 5";
#endif
            break;
        case '6':
            mediaPlayer.setSpeed(6);
			playerSpeed = 6;
#ifdef GROWL
            ofxNotify() << "Animation speed 6";
#endif
            break;
        case '7':
            mediaPlayer.setSpeed(7);
			playerSpeed = 7;
#ifdef GROWL
            ofxNotify() << "Animation speed 7";
#endif
            break;
        case '8':
            mediaPlayer.setSpeed(8);
			playerSpeed = 8;
#ifdef GROWL
            ofxNotify() << "Animation speed 8";
#endif
            break;
        case '9':
            mediaPlayer.setSpeed(9);
			playerSpeed = 9;
#ifdef GROWL
            ofxNotify() << "Animation speed 9";
#endif
            break;
        case '/':
            mediaPlayer.setSpeed(0.03);
			playerSpeed = 0.03;
#ifdef GROWL
            ofxNotify() << "Animation speed 0.03 // 2 sec";
#endif
            break;
        case '*':
            mediaPlayer.setSpeed(0.015); // 5 sec
			playerSpeed = 0.015;
#ifdef GROWL
            ofxNotify() << "Animation speed 0.015  // 5 sec";
#endif
            break;
            
        case '+':
        case '=':
            volume += 0.1;
            if (volume > 1) volume = 1;
            mediaPlayer.setVolume(volume);
#ifdef GROWL
            ofxNotify() << "Volume " << volume*100;
#endif
            break;
            
        case '-':
            volume -= 0.1;
            if (volume < 0) volume = 0;
            mediaPlayer.setVolume(volume);
#ifdef GROWL
            ofxNotify() << "Volume " << volume*100;
#endif
            break;
            
            
        case OF_KEY_PAGE_UP:
            brightnessLevel += 1;
            if (brightnessLevel > 100) brightnessLevel = 100;
            
#ifdef GROWL
            ofxNotify() << "Brightness " << brightnessLevel;
#endif
            break;
            
        case OF_KEY_PAGE_DOWN:
            brightnessLevel -= 1;
            if (brightnessLevel < 0) brightnessLevel = 0;
          
#ifdef GROWL
            ofxNotify() << "Brightness " << brightnessLevel;
#endif
            break;
            
        case 'W':
            brightness = !brightness;
#ifdef GROWL
            if (!brightness) {
                ofxNotify() << "Brightness off";
            } else {
                ofxNotify() << "Brightness on";
            }
#endif
            break;
            
        case '$':
            mediaPlayer.setSpeed(0.00125); // 64 sec
			playerSpeed = 0.00125;
#ifdef GROWL
            ofxNotify() << "Animation speed 0.00125 // 64 sec";
#endif
            break;
        case '`':
            mediaPlayer.setSpeed(0.0025); // 32 sec
			playerSpeed = 0.0025;
#ifdef GROWL
            ofxNotify() << "Animation speed 0.0025  // 32 sec";
#endif
            break;
            
        case ',':
            mediaPlayer.setSpeed(0.005); // 16 sec
			playerSpeed = 0.005;
#ifdef GROWL
            ofxNotify() << "Animation speed 0.005 // 16 sec";
#endif
            break;

        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (button==0) {
        if ((x>mediaWidth-20) && (y<20)) {
			ofToggleFullscreen();
            
            if (ofGetWindowMode() == OF_FULLSCREEN) {
                //   ofHideCursor();
#ifdef GROWL
                ofxNotify() << "Fullscreen " << screenWidth << "x" << screenHeight;
#endif
            } else {
                //   ofShowCursor();
#ifdef GROWL
                ofxNotify() << "Window " << mediaWidth << "x" << mediaHeight;
#endif
            }
        }  else if ((x>mediaWidth-20) && (y<mediaHeight-20)) {

            stopLoop = !stopLoop;
            if (!stopLoop) {
                mediaPlayer.setPaused(false);
            } else {
                mediaPlayer.setPaused(true);
            }
            
#ifdef GROWL
            if (stopLoop) {
                ofxNotify() << "------ stop";
            } else {
                ofxNotify() << "+++++ start";
            }
#endif
        } else {
        // Next media
        if (mediaPositionMax>0) {
            if (mediaPosition<mediaPositionMax-1) {mediaPosition++;} else {mediaPosition = 0;}
            LoadMedia();
        }
        }
    }  else if ((button==1) || (button==2) || (button==3)) {
        
        // Previous media
        if (mediaPositionMax>=0) {
            mediaPosition--;
            if (mediaPosition<0) { mediaPosition = mediaPositionMax-1;}
            LoadMedia();
           } 
        
    } 
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo info){
    
  
    if( info.files.size() > 0 ){
        
        string pathDrag= info.files[0];
        string pathExt = pathDrag.substr (pathDrag.length()-4,pathDrag.length());
        cout << "Path " << pathDrag << endl;
        cout << "Extension " << pathExt << endl;
        
        if ((pathExt == ".mov") || (pathExt == ".Mov") || (pathExt == ".avi") || (pathExt == ".Avi") || (pathExt == ".media") || (pathExt == ".mp4") || (pathExt == ".mpg") || (pathExt == "mpeg" ) || (pathExt == ".Mpg") || (pathExt == ".Mpeg") || (pathExt == ".MOV") || (pathExt == ".AVI") || (pathExt == ".MP4") || (pathExt == ".MPG") || (pathExt == "MPEG") || (pathExt == ".media") || (pathExt == ".gif") || (pathExt == ".GIF")
            ) {
              stopLoop = TRUE;
            bool loadDragSuccess = mediaPlayer.loadMovie(pathDrag);
            
#ifdef GROWL
            ofxNotify() << "Load Video / New folder" << pathDrag;
#endif
            
            if (loadDragSuccess) {
                // mediaPlayer.loadMovie(pathDrag);
                mediaPlayer.setLoopState(OF_LOOP_NORMAL);
                // OF_LOOP_NONE, OF_LOOP_NORMAL, OF_LOOP_PALINDROME
                mediaPlayer.play();
                mediaPlayer.setVolume(volume);
                mediaWidth = mediaPlayer.getWidth(); // media width
                mediaHeight = mediaPlayer.getHeight(); // media height
                
                totalFrameNumber = mediaPlayer.getTotalNumFrames();
                startTime = actualTime;
                if (ofGetWindowMode() == OF_WINDOW) {
                    ofSetWindowShape(mediaWidth, mediaHeight);
                    ofSetWindowPosition((screenWidth/2)- (mediaWidth/2), (screenHeight/2)-(mediaHeight/2));
                }

          
            }
            
            stopLoop = FALSE;      
            
        }

        
        if ((pathExt == ".png") || (pathExt == ".jpg") || (pathExt == "tiff") || (pathExt == ".bmp") || (pathExt == "jpeg") || (pathExt == ".PNG") || (pathExt == ".JPG") || (pathExt == "TIFF") || (pathExt == ".BMP") || (pathExt == "JPEG"))
            
        {   stopLoop = TRUE;
            bool loadDragSuccess = mediaPlayer.loadMovie(pathDrag);
            
#ifdef GROWL
            ofxNotify() << "Load image / New folder " << pathDrag << " "<<pathExt ;
#endif
            
            if (loadDragSuccess) {
                
                //  mediaPlayer.loadMovie(pathDrag);
                mediaPlayer.setLoopState(OF_LOOP_NORMAL);
                // OF_LOOP_NONE, OF_LOOP_NORMAL, OF_LOOP_PALINDROME
                mediaPlayer.play();
                
                mediaWidth = mediaPlayer.getWidth(); // media width
                mediaHeight = mediaPlayer.getHeight(); // media height
                
                totalFrameNumber = mediaPlayer.getTotalNumFrames();
                startTime = actualTime;
                if (ofGetWindowMode() == OF_WINDOW) {
                    ofSetWindowShape(mediaWidth, mediaHeight);
                    ofSetWindowPosition((screenWidth/2)- (mediaWidth/2), (screenHeight/2)-(mediaHeight/2));
                }

            }
            stopLoop = FALSE;
      }
    
        
        path =  pathDrag.erase(pathDrag.find_last_of("/"), string::npos);
        mediaFolder.listDir(path);
        mediaFolder.sort(); // sorting dir ?
        mediaPositionMax = mediaFolder.numFiles();
        
        
#ifdef GROWL
        ofxNotify() << " New path " << path;
#endif
           mediaFileName = path;


        
    }
    
    }

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::LoadMedia(){
    
    #ifndef WIN32
    //ofSetDataPathRoot("../../media/");
    #endif
	
    ofDirectory mediaFolder(path);
    
    if (mediaExtAllowed !="all" ) {
           mediaFolder.allowExt(mediaExtAllowed);
    } else {
        
    mediaFolder.allowExt("gif");
    mediaFolder.allowExt("mov");
    mediaFolder.allowExt("avi");
    mediaFolder.allowExt("mp4");
    mediaFolder.allowExt("mpg");
    mediaFolder.allowExt("mpeg");
    mediaFolder.allowExt("png");
    mediaFolder.allowExt("jpg");
    mediaFolder.allowExt("bmp");
    mediaFolder.allowExt("tiff");
    mediaFolder.allowExt("jpeg");
    
    }
    
    if(!mediaFolder.exists()){ mediaFolder.create(true); }
    mediaFolder.listDir(path);
    // mediaFolder.sort(); // sorting dir ?

    if (mediaFolder.isDirectoryEmpty(path)) {
        mediaPlayer.loadMovie("Contents/Resources/loader.gif");
        mediaPositionMax = 1;
        mediaPlayer.setLoopState(OF_LOOP_NORMAL);
        // OF_LOOP_NONE, OF_LOOP_NORMAL, OF_LOOP_PALINDROME
        mediaPlayer.play();
        mediaWidth = mediaPlayer.getWidth(); // media width
        mediaHeight = mediaPlayer.getHeight(); // media height
        mediaFileName = "No file in " +  path;
        totalFrameNumber = mediaPlayer.getTotalNumFrames();

    } else {
        
        if (sortInverse) {
        mediaPlayer.loadMovie(mediaFolder.getPath(mediaPositioninverse));
            mediaFileName = mediaFolder.getName(mediaPositioninverse);
        } else {
        mediaPlayer.loadMovie(mediaFolder.getPath(mediaPosition));
            mediaFileName = mediaFolder.getName(mediaPosition);
            }
        
        
        
        mediaPositionMax = mediaFolder.numFiles();
        mediaPositioninverse = mediaPositionMax - mediaPosition - 1;
        mediaPlayer.setLoopState(OF_LOOP_NORMAL);
        // OF_LOOP_NONE, OF_LOOP_NORMAL, OF_LOOP_PALINDROME
        mediaPlayer.play();
        mediaPlayer.setSpeed(playerSpeed);
        mediaPlayer.setVolume(volume); // Volume if video
        mediaWidth = mediaPlayer.getWidth(); // media width
        mediaHeight = mediaPlayer.getHeight(); // media height
        
        mediaFileName.erase(mediaFileName.find_last_of("."), string::npos);
        totalFrameNumber = mediaPlayer.getTotalNumFrames();
    }

    startTime = actualTime; // Reset timer
    if (ofGetWindowMode() == OF_WINDOW) {
        ofSetWindowShape(mediaWidth, mediaHeight); // Change window size
        // ofSetWindowPosition((screenWidth/2)- (mediaWidth/2), (screenHeight/2)-(mediaHeight/2));
    }
    
    mediaPlayer.fadeIn();
    BackImage.fadeIn();
    
#ifdef GROWL
    if (notification) {
        if (sortInverse) {
            if ((totalFrameNumber!=1) && (totalFrameNumber<=250)) {
                ofxNotify() << totalFrameNumber << "f "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPositioninverse+1) << "/" << mediaFolder.numFiles();
                
                
            } else  if (totalFrameNumber>250) {
                
                
                ofxNotify() <<"Video " << (int)mediaPlayer.getDuration() << "s " << totalFrameNumber << "f "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPositioninverse+1) << "/" << mediaFolder.numFiles();
                
                
                
                
            } else {
                ofxNotify() << "Image "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPositioninverse+1) << "/" << mediaFolder.numFiles();
                
            
            
            }
            

            ofxNotify() << mediaFileName ;        
        } else {            
            if ((totalFrameNumber!=1) && (totalFrameNumber<=250)) {
                ofxNotify() << totalFrameNumber << "f "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPositioninverse+1) << "/" << mediaFolder.numFiles();
                
                
            } else  if (totalFrameNumber>250) {
                
                
                ofxNotify() <<"Video " << (int)mediaPlayer.getDuration() << "s " << totalFrameNumber << "f "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPositioninverse+1) << "/" << mediaFolder.numFiles();
                
                
                
            } else {
                ofxNotify() << "Image "
                << mediaWidth << "x" << mediaHeight << " "
                << (mediaPositioninverse+1) << "/" << mediaFolder.numFiles();
                
                
                
            }
            
            ofxNotify() << mediaFileName ;
        }
    }
    
#endif
    
}


//--------------------------------------------------------------

#ifdef GROWL
void testApp::ShowSettings(){
    
    ofxNotify() << "Fade in/out duration : " << fadeTime << "s";
    ofxNotify() << "Play speed : " << playerSpeed;
    ofxNotify() << "Framerate : " << framerate << "fps";
    ofxNotify() << "media duration : " << timerDuration/1000 << "s";
    ofxNotify() << "Volume : " << volume;

    
    int diaporamaDuration = (timerDuration/1000) * mediaPositionMax;
    if (diaporamaDuration>=60) {
        diaporamaDuration = diaporamaDuration / 60;
        ofxNotify() << "Diaporama duration : " << diaporamaDuration << " min";
    } else{
        ofxNotify() << "Diaporama duration : " << diaporamaDuration << "s";
    }
}
#endif

//--------------------------------------------------------------
void testApp::LoadConfig() {
    
    
    if (conf.load("../settings.txt")) {
        ofLogNotice("Config. Loaded");
    } else {
        ofLogNotice("Config. Error");
    }
    
    titleWindow  = conf.get("General.Title", (string)"");
    
    mediaExtAllowed  = conf.get("General.Extension", (string)"all");
    
    path  = conf.get("General.Path", (string)"media"); // Folder with media
    sortInverse = conf.get("General.sortInverse", 0);
    volume = ((float)conf.get("General.Volume", 70)/100);
    playerSpeed = (float)(conf.get("General.playerSpeed",1.0f));
    
    framerate = conf.get("General.framerate", 30); // fps
    StartfullScreen = conf.get("General.StartfullScreen", 1);
    Zoom = conf.get("General.ZoomfullScreen", 1);
    
    redBackgroundColor = conf.get("Background.redBackgroundColor", 0); // red 0 to 255 in background
    greenBackgroundColor = conf.get("Background.greenBackgroundColor", 0); // green 0 to 255 in background
    blueBackgroundColor = conf.get("Background.blueBackgroundColor", 0); // blue 0 to 255 in background
    BackgroundImage = conf.get("Background.BackgroundImage", 1); // Background image in fullscreen
    backgroundImagePath  = conf.get("Background.BackgroundImagePath", (string)"Contents/Resources/fondwallxs.gif"); // backgroundImagePath 1920x1200 in fullscreen
    
    timerDuration = ( conf.get("Diaporama.timerDuration", (int)20) ) * (1000); // 60 = 60sec
    diaporamaMode = conf.get("Diaporama.diaporamaMode", false);
    
    growl = conf.get("Text.Notification", false);
    notification = conf.get("Text.filenameNotification", false);
    NotifyTime = conf.get("Text.notifyTime", (int)2) * (1000); // 7 = 7sec

    fadeTime = conf.get("Fadein.fadeTime", (float)0.6); //  sec
    delayBlack = conf.get("Fadein.delayBlack",  (int)1); //  sec
    
    brightnessLevel = 100;
    
    conf.clear();
    
}
