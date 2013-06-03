# Pinacotek

A simple media player to exhibit gifs, videos, images

Source : <https://github.com/fluaten/Pinacotek>

Author : Nicolas Boillot <http://www.fluate.net>

Put all media (images, videos, gifs) in folder media.
Rename files to define order of diaporama.

Drag and drop a gif, video, image on the window 
to play it and set the folder for the diaporama.

### Download (Mac OS X 10.4-10.8)

##<http://www.fluate.net/code/pinacotek>


### Required

For compilation :
Openframeworks 0.7.4
--
ofxExtras
ofxFadable
ofxIniSettings
ofxNotify

### Shortcuts

Press SPACE or F to go fullscreen 

Press LEFT or RIGHT / left click or right click to navigate in folder

Press UP or DOWN / to forward or rewind video

Press + = or - to change volume


Press W or RETURN to stop the loop 

Press X or C to skip to next/previous frame

Press R to reverse animation 

Press UP or DOWN to change animation speed

Press 0 to 9 to change animation speed

Press , * / to slow animation speed


Press J to M to change framerate 

Press T to see framerate 

Press I to have info on image

Press S to show diaporama settings


Press D to start/stop diaporama

Press N to inverse diaporama

Press Z to zoom image in fullscreen

Press B to show the background image in fullscreen


Press V to capture screen

Press U to visit the Homepage of Pinacotek


### Options in setting.txt

[General]

Title=Pinacotek

// Title of the window + filename


Extension=all

// Files allowed in the folder, put "all" for all media, 

// choose one in the list below to restrict 

// gif/mov/avi/mp4/mpg/mpeg/png/jpg/tiff/bmp/jpeg


Path=../media

//Path=../../../../../../../Users/admin/Desktop

// Path to the startup media folder


sortInverse=0
// Read file in inverse order

volume=70
// Default volume if video

playerSpeed=1.0
// media play speed 1.0 = normal speed , 0.03 = 2 sec/frame

framerate=25
// framerate of the app 25 = 25 fps

StartfullScreen=0
// Startup in fullscreen 1 = true

ZoomfullScreen=0
// Zoom image in fullscreen 1 = true


////////////////////////////

[Background]
redBackgroundColor=0
// Red color background in fullscreen 0 to 255

greenBackgroundColor=0
// Green color background in fullscreen 0 to 255

blueBackgroundColor=0
// Blue color background in fullscreen 0 to 255

BackgroundImage=0
// Background image in fullscreen 1 = true

BackgroundImagePath=Contents/Resources/fondwallxs.gif

// BackgroundImagePath in fullscreen, press B to show 
  
// ../fondwallxs.gif (at least 1920x1200)


////////////////////////////

[Diaporama]
diaporamaMode=0
// diaporama or single image //  1 = true  0 = false

timerDuration=50
// duration of each media : 60 = 60 sec

////////////////////////////

[Text]
Notification=1
// text notification on screen //  1 = true  0 = false

filenameNotification=1
// filename notification on screen on each load //  1 = true  0 = false

notifyTime=6
//notification text time on screen // 7 = 7sec

////////////////////////////

[Fadein]
fadeTime=5.0
// duration of fade to black 5.0 = 5 sec

delayBlack=1
// duration of a black frame during two fade 1 = 1 sec

### Simplified BSD License

   https://en.wikipedia.org/wiki/BSD_licenses
	
	Copyright 2012 Nicolas Boillot. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

       1. Redistributions of source code must retain the above copyright notice, this list of
          conditions and the following disclaimer.

       2. Redistributions in binary form must reproduce the above copyright notice, this list
          of conditions and the following disclaimer in the documentation and/or other materials
          provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY NICOLAS BOILLOT ''AS IS'' AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Maurice Svay OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those of the
    authors and should not be interpreted as representing official policies, either expressed
    or implied, of Nicolas Boillot.