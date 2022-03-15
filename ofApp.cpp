//Author: Nicholas Hickey
//Title: Audio Analyser
//Date: November 18th, 2016

#include "ofApp.h"
#include "ProjectConstants.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//Set the application basic settings
	ofSetVerticalSync(60.0f);
	//Set the screen size
	ofSetWindowShape(1280, 1200);
	//Set frame rate
	ofSetFrameRate(60);
	
	//Set toggle to false to give user option to see debug settings
	toggle = false;

	//Set up the variables
	speed2 = false;
	speedx = 0.0f;
	speedy = 0.0f;
	speedrec = 0.0f;
	snareDrum = 0;
	bassDrum = 0;
	vocal = 0;
	guitar = 0;
	drumsVisual = 0;
	drumsMovement_x = 0;
	drumsMovement_y = 0;
	drumsScale = 0;
	speed = 1.2f;
	m_angle = 0.0f;
	jitterX = 0;
	jitterY = 0;

	m_font.load("fonts/Oswald-Bold.ttf", 25, true);
	m_soundPlayer.load("coldplay.wav");
	m_soundPlayer.setLoop(false);
	m_soundPlayer.play();

	//Set up the bin data 
	for (int i = 0; i < 26; i++)
	{
		bindata[i] = 0;
	}


	//Initialize the project constants 
	m_BgImg.load(ProjectConstants::IMG_PATH_BG);
	m_BgImg2.load(ProjectConstants::IMG_PATH_BG2);
	ofSetFrameRate(ProjectConstants::PROJ_DESIRED_FRAMERATE);
	m_font2.load(ProjectConstants::FONT_PATH_MONSTERRAT2, 20, true);
	m_font.load(ProjectConstants::FONT_PATH_MONSTERRAT, 60, true);

	//Initialize audio analyser
	m_audioAnalyser.init(&m_soundPlayer, 20);

	for (int i = 0; i < 70; i++)
	{
		numX = rand() % 80 + (-40);
		numZ = rand() % 500 + 300;
		numZ = numZ * -1;

		enemyPosX[i] = numX;
		enemyPosZ[i] = numZ;
	}

	enemyPosY = 400;

	//Set up the camera initial position
	cameraPosition = ofVec3f(0.0f, 0.0f, 0.0f);

}

//--------------------------------------------------------------
void ofApp::update(){


	//Update the speedrec variable to equal the current frame num
	speedrec = ofGetFrameNum();
	//Angle is updates by 1.3 each time added to its current value
	m_angle += 0.3f;
	//Update the audio analyser
	m_audioAnalyser.update();


	//If the user presses right the drums movement x variable is incremented by a value of positive 1
	if (ofGetKeyPressed(OF_KEY_RIGHT))
	{
		drumsMovement_x += 1;
	}


	//If the user presses right the drums movement y variable is incremented by a value of negative 1
	if (ofGetKeyPressed(OF_KEY_UP))
	{
		drumsMovement_y -= 1;
	}

	//If the user presses right the drums movement y variable is incremented by a value of positive 1
	if (ofGetKeyPressed(OF_KEY_DOWN))
	{
		drumsMovement_y += 1;
	}

	//If the user presses right the drums movement x variable is incremented by a value of negative 1
	if (ofGetKeyPressed(OF_KEY_LEFT))
	{
		drumsMovement_x -= 1;
	}


	//Update the camera position in 3D space
	cameraPosition = ofVec3f(0.0f, 0.0f, 0.0f);
	camera.setPosition(0.0f, 0.0f, 0.0f);
	//Set the speed variable to update by a value of 2000
	speed += 2000.0f;

	//Set the camera position vector to the get position 
	ofVec3f cameraPosition = camera.getPosition(); //track the camera and palm positions to move the camera based on the players hand position
	//Update the move vector
	ofVec3f moveVec = (cameraPosition) * 0.5f;

	//Set camera position to move based on the linear average of the bass drum located in bin 0
	camera.setPosition(0.0f, 0.0f, (cameraPosition.z - (m_audioAnalyser.getLinearAverage(0) * 50)));

	//Set up second variables to store positions of camera x and camera x
	m_cameraPositionZ = cameraPosition.z;
	m_cameraPositionX = cameraPosition.x;

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	//Initialize red, blue and green variables to equal a random percentage of 255 to get a random colour
	float red = rand() % 255;
	float green = rand() % 255;
	float blue = rand() % 255;

	//Set circle resolution
	ofSetCircleResolution(100);
	ofSetColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofPushMatrix();

	//Set up the song time in seconds variable to store and display the time eleapsed
	float songTimeSeconds = m_soundPlayer.getPositionMS() / 1000.0f;
	
	//If the song time is less than or equal to 19.7 or greater than and equal to 114 the it draws background 1
	if (songTimeSeconds <= 19.9f || songTimeSeconds >= 114.0f)
	{
		m_BgImg.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
	}
	//Else is draws background 2
	else
	{
		m_BgImg2.draw(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);

	}

	//If song time is greater than 54 and less than 57 display the current song lyric text
	if (songTimeSeconds >= 54.0f && songTimeSeconds <= 57.4f || songTimeSeconds >= 128.5f && songTimeSeconds <= 132.8f)
	{
		ofSetColor(red, green, blue);
		m_font.drawString("SO HIGH, SO HIGH", 350, 300);
	}

	//Display and draw string of song information and time to the user
	ofSetColor(ofFloatColor::white);
	m_font2.drawString("Elapsed Time: " + ofToString(songTimeSeconds), 40, 130);
	m_font2.drawString("Hymm For The Weekend", 40, 70);
	m_font2.drawString("Artist: Coldplay", 40, 100);
	m_font2.drawString("Use Up, Down, Left, Right arrows to control orbs", 40, 950);

	//Set up the different frequency areas to display and analyze
	ofSetRectMode(OF_RECTMODE_CENTER);
	bassDrum = m_audioAnalyser.getLinearAverage(0);
	snareDrum = m_audioAnalyser.getLinearAverage(1);
	vocal = m_audioAnalyser.getLinearAverage(13);
	guitar = m_audioAnalyser.getLinearAverage(15);


	//If the toggle is true meaning the user has pressed the space bar then it runs the following code 
	if (toggle)
	{
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofSetColor(255);
		m_font2.drawString("Volume Levels", 100, 550); //drawing a label

		//Draw the three volume level rectangles based on volume levels
		ofDrawRectangle(100, 600, 10, m_audioAnalyser.getLeftLevel() * 100.0f);
		ofDrawRectangle(200, 600, 10, m_audioAnalyser.getRightLevel() * 100.0f);
		ofDrawRectangle(300, 600, 10, m_audioAnalyser.getMixLevel() * 100.0f);

		m_font2.drawString("Left", 100, 590);
		m_font2.drawString("Right", 200, 590);
		m_font2.drawString("Full Mix", 300, 590);

		m_font2.drawString("Bass", 100, 250);
		m_font2.drawString("Pluck", 200, 250);
		m_font2.drawString("Snare", 300, 250);
		
		//Draw the three frequency variables in a circle
		ofDrawCircle(100, 300, bassDrum * 0.1f);
		ofDrawCircle(200, 300, guitar * 0.1f);
		ofDrawCircle(300, 300, snareDrum * 0.1f);

		//Draw the three types of data: waveform, spectrum and linear averages
		m_audioAnalyser.drawWaveform(40, 700, 1000, 30); 
		m_audioAnalyser.drawSpectrum(40, 800, 1000, 128); 
		m_audioAnalyser.drawLinearAverages(40, 1000, 1200, 128);	

	}


	//Turn fill on
	ofFill();

	//Set the bin data[i] array to each of the 24 bin data values to be used later to draw
	for (int i = 0; i < 24; i++)
	{
		bindata[i] = m_audioAnalyser.getLinearAverage(i);
	}
	ofPushMatrix();
	
	//Begin the 3D camera 
	camera.begin();

	//Set the jitter bass varible equal to the bassdrum value so we can have things moved bassed on that frequency
	float jitterBass = bassDrum * 0.1f;
	jitterX = ofRandom(jitterBass, 1.0f);
	jitterY = ofRandom(jitterBass, 1.0f);


	//If the song time is withing the parameters of the stament then it draws the following code
	if (songTimeSeconds >= 57.4f && songTimeSeconds <= 95.0f  || songTimeSeconds >= 132.8f)
	{
		//Set the background to black 
		ofBackground(0, 0, 0);
		
		///If the vocal value of the frequency is greater than 90 than the background changes colour
		if (vocal > 90.0f)
		{
			ofBackground(red, green, blue);
		}

		//Drawing the rectangles and moving them 
		for (int i = 0; i < 70; i++)
		{
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofNoFill();
			ofPushMatrix();
			ofPushMatrix();
			ofSetColor(255, 255, 255);
			ofTranslate(0.0f, 0.0f, 0.0f);
			//Set the position to middle of the screen and move only the Z value forward
			ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, enemyPosZ[i] + 1500);
			//The z value os moved based on the audio frequency value at bin 0
			ofTranslate(0, 0, (((speedrec * 0.1f) - m_audioAnalyser.getLinearAverage(0)) * 0.2f));
			//If the bassdrum value is greater than 200 than rotate the everything 30
			if (bassDrum > 200.0f)
			{
				ofRotate(45);
			}
			//Draw the rectangle
			ofDrawRectangle(0, 0, 15, 15);
			ofPopMatrix();
			ofPopMatrix();
		}
	}

	//Else if the song elapsed time is less than or equal to 54.7 or greater than or equal to 95.0 the the following code is run
	else if (songTimeSeconds <= 57.4f || songTimeSeconds >= 95.0f)
	{
		
		m_audioAnalyser.drawWav(40, 700, 1000, 30);
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
		
		//If bassdrum variable frequency is greater than 200 than the movement of drumsx and y is incremented by 1
		if (bassDrum > 200.0f)
		{
			//drumsMovement_x += 1;
			//drumsMovement_y += 1;
		}

		if (bassDrum > 50.0f)
		{
			drumsScale += 0.001;
		}
		
		ofPushMatrix();
		//Rotate everything by m angle value
		ofRotate(m_angle);

		//Set the jiiter x and jitter y variables
		jitterX = ofRandom(jitterBass, 1.0f);
		jitterY = ofRandom(jitterBass, 1.0f);
		ofFill();
		ofPopMatrix();
		ofFill();

		//Set the colour to white 
		ofSetColor(ofFloatColor::white);
		float rotaterec = 0.0f;
		
		//Loop 50 times 
		for (int h = 0; h < 50; h++)
			{
				ofPushMatrix();
				//Loop 24 times for the bin data
				for (int i = 0; i < 24; i++)
				{
					ofPushMatrix();
					ofTranslate(h, i);
					ofRotate(rotaterec);
					//Rotate by m_angle + 50 x the h value of the loop
					ofRotate(m_angle + (50 * h));
					//Translate the z value by speedrec variable
					ofTranslate(h, i, speedrec * 0.1);
					//Translate based on drums movement x and y value
					ofTranslate(drumsMovement_x, drumsMovement_y);
					//draw circles based on bin data 
					ofDrawCircle(0, 0, bindata[i] * 0.02f);
					rotaterec += 15.8f;
				}
				ofPopMatrix();
			}
		ofPushMatrix();
		ofPopMatrix();
		float rotaterec2 = 0.0f;
		ofSetColor(red, green, blue);

		//Loop 8 times
		for (int h = 0; h < 8; h++)
		{
			//Loop 24 times
			for (int i = 0; i < 24; i++)
			{
				ofPushMatrix();
				//If the song time is within the parameters the code executes
				if (songTimeSeconds <= 19.9f || songTimeSeconds >= 114.0f)
				{
					//Initial circle analyser 
					ofTranslate(0, 0);
					ofRotate(180 + i * 20);
					ofSetColor(255, 255, 255);
					//Post the rectangles for the data 
					ofDrawRectangle(0, 0, 25, (bindata[i] * 1.5) + 10);
					rotaterec2 += 30.0f;
					ofSetColor(255, 255, 255);
					ofDrawCircle(0, 0, 80);
				}
				else
				{
					//Else if the time is within parameter execute the following allowing for a strobe effect on the circle analyser
					ofTranslate(0, 0);			
					ofTranslate(jitterX, 0);
					ofRotate(180 + i * 20);
					ofSetColor(255, 255, 255);
					//Cycle through bin data drawing a rectagle at each height value of the bin data
					ofDrawRectangle(0, 0, 25, (bindata[i] * 1.5) + 10);
					//Call on the red green and blue random variables to create a strobe effect
					ofSetColor(red, green, blue);
					//Cycle through bin data drawing a rectagle at each height value of the bin data with strobe effect
					ofDrawRectangle(0, 0, 15, (bindata[i] * 1.5));
					rotaterec2 += 30.0f;
					ofSetColor(255, 255, 255);
					ofDrawCircle(0, 0, 80);
				}
				ofPopMatrix();
			}
		}
	}

	//End the 3D camera 
	camera.end();

	//Final pop matrix
	ofPopMatrix();

	ofSetRectMode(OF_RECTMODE_CORNER);
}

//Of key pressed function to call on the debug infomation
void ofApp::keyPressed(int key){

	//If the space key is pressed then toggle bool is true
	if (ofGetKeyPressed(' '))
	{
		if (toggle)
		{
			toggle = false;
		}
		//Else toggle bool is false
		else if (toggle == false)
		{
			toggle = true;
		}
	}

}
