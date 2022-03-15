//Author: Nicholas Hickey
//Title: Audio Analyser
//Date: November 18th, 2016

#pragma once
#include "ofMain.h"
#include "AudioAnalyser.h"
#include "ProjectConstants.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		//Create function for key pressed 
		void keyPressed(int key);

		//Set up the various data
		ofFmodSoundPlayer m_soundPlayer;  
		AudioAnalyser	  m_audioAnalyser;
		ofVec3f			  cameraPosition;
		ofCamera		  camera;

		//Set up the float data variables
		float				drumsVisual;
		float				drumsMovement_x;
		float				drumsMovement_y;
		float				drumsScale;
		float				speed;
		float			    bassDrum;
		float				snareDrum;
		float			    bassDrum2;
		float				snareDrum2;
		float			    bassDrum3;
		float				snareDrum3;
		float				bindata[26];
		float				speedrec;
		float				guitar;
		float				vocal;
		float				jitterX;
		float				jitterY;
		float				enemyPosX[70];
		float				enemyPosY;
		float				enemyPosZ[70];
		float				m_cameraPositionZ;
		float				m_cameraPositionX;
		float				m_cameraRadius;
		float				numX;
		float				numZ;
		float				m_angle;
		float				speedx;
		float				speedy;

		//Set up the bool data
		bool				speed2; 
		bool				toggle;

		//Set up image data
		ofImage				m_BgImg;
		ofImage				m_BgImg2;

		//Set up font data 
		ofTrueTypeFont	      m_font2;
		ofTrueTypeFont	      m_font;	
};
