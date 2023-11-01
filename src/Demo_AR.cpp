//--------------------------------------------------------------------------------------------
// Project: AR tracking system based on stereo vision
// Brief:   DEMO_AR.cpp
// Author:  Jorge Cano
// Date:    30/07/2009
// 
//--------------------------------------------------------------------------------------------
// 	Copyright (c) 2009 Jorge Cano
//--------------------------------------------------------------------------------------------

#include "Cing.h"

// Camera capture
#include "PS3Camera.h"

// Flash GUI
#include "Flash/FlashMovieManager.h"
#include "xCalibrationGUI.h"

#include "Flash/FlashVideoPlayer.h"

// Tracking
#include "MarkerDetector.h"
#include "Tracker3d.h"

// Other
#include "AveragedVector.h"

// Ogre
#include "Ogre3d/include/OgreCamera.h"			// To allow creating new ogre camera
#include "Ogre3d/include/OgreRenderWindow.h"	// To allow change active camera

// Open CV
//#include "OpenCV/cv/include/cv.h"

int					camW		= 640;
int					camH		= 480;
Cing::GraphicsType	camFormat	= BGRA;
int					fps			= 30;
float				FOVy		= 47.3;  // PS3Eye vertical FOV

PS3Camera			cam_0;
PS3Camera			cam_1;
Cing::Image			img_0;
Cing::Image			img_1;

// Marker detection
MarkerDetector markerDetector;

// 2d position of markers
std::vector< Cing::Vector2d > markers_cam_0;
std::vector< Cing::Vector2d > markers_cam_1;

// 3d position of markers
std::vector< Cing::Vector > markers_position;
std::vector< AveragedVector* > markers_AvgPosition;
Sphere ball_0,ball_1,ball_2;
Axis trackedModel;
Axis boneModel;

// 3d Tracker
Tracker3d		tracker3d;

// Tracked model
//Axis	trackedModel;
Cing::Quaternion m_orientation;

// Virtual camera
Ogre::Camera*			arCam;

// Flash GUI
FlashMovie				mainMenu;
CalibrationGUI			calibGUI;

//Object3D				paloma;
Ogre::SceneNode*		paloma;
Object3D				mandibula_Paloma;
Object3D				molares_Paloma;
Object3D				dentario_Paloma;

PointLight				light;

boolean					showDebugAxis = true;
boolean					showDebugOutput_Val = true;
// Application states
enum ApplicationState
{
	UNDEFINED,
	CAMERA_CALIBRATION,
	TRACKER_CALIBRATION,
	MODEL_CALIBRATION,
	RUN
};

// Control general del estado de la aplicación
ApplicationState appState = TRACKER_CALIBRATION;

CREATE_APPLICATION( "Cing Demo" );

////////////////////////////////// FUNCIONES GENERALES EXPORTADAS A FLASH

// Exit application
Hikari::FlashValue exitApp( Hikari::FlashControl* caller, const Hikari::Arguments& args )
{
	exit();
	return FLASH_VOID;
}
// Set threshold 
Hikari::FlashValue setThreshold( Hikari::FlashControl* caller, const Hikari::Arguments& args )
{
	int threshold = (int) (args.at(0).getNumber()*255);
	markerDetector.setThreshold( threshold );
	println( "Threshold: %i", threshold );
	return FLASH_VOID;
}
// Set app state
Hikari::FlashValue setCalibration( Hikari::FlashControl* caller, const Hikari::Arguments& args )
{
	bool calibration = args.at(0).getBool();

	if (calibration) println( "SetCalibration: True" );
	if (!calibration) println( "SetCalibration: False" );

	// Cambiar estado de aplicación
	if ( calibration )
	{
		appState = MODEL_CALIBRATION;
	}
	else 
	{
		appState = RUN;
	}

	return FLASH_VOID;
}
void setup()
{
	size(1024, 768);

	// Load config file
	XMLElement xml;
	xml.load( "configAR.xml" );

	// Get element "models" (the root is configFile)
	XMLElement models = xml.getChild(0);

	// Get models information
	XMLElement model_0 = models.getChild(0);
	XMLElement model_1 = models.getChild(1);
	XMLElement model_2 = models.getChild(2);

	// Writes something
	TiXmlDocument* XMLDoc = new TiXmlDocument();
	XMLDoc->InsertEndChild( TiXmlElement( "windowsSets") );

	// Write data
	TiXmlElement* rootNode = XMLDoc->RootElement();
	if ( !rootNode )
		THROW_EXCEPTION( "Error en root node del xml" );

	// Add set to xml
	TiXmlElement* CVProperties = rootNode->InsertEndChild(TiXmlElement( "CVProperties" ))->ToElement();
	CVProperties->SetAttribute( "numberOfCameras", 2);
	CVProperties->SetAttribute( "cameraGain", 120);
	CVProperties->SetAttribute( "cameraExposure", 120);
	CVProperties->SetAttribute( "threshold", 60);

	// Save file
	XMLDoc->SaveFile( dataFolder + "CVProperties.xml" );

	LOG( "Windows Exported Correctly" );

	// Release
	delete XMLDoc;

	// Init cameras
	cam_0.init(0,camW,camH,fps, camFormat); 
	cam_1.init(1,camW,camH,fps, camFormat);
	img_0.init(camW,camH,COLOR);
	img_1.init(camW,camH,COLOR);

	// Create 3d tracker
	tracker3d.init(2,camW,camH);

	//bool success = tracker3d.loadCameraCalibration("camera.calibration.txt");
	bool success = tracker3d.loadCameraCalibration("camera.calibration_640_noDistort.txt");

	// Init some temporal stuff
	markers_cam_0.push_back(Cing::Vector2d(0,0));	markers_cam_0.push_back(Cing::Vector2d(0,0));	markers_cam_0.push_back(Cing::Vector2d(0,0));
	markers_cam_1.push_back(Cing::Vector2d(0,0));	markers_cam_1.push_back(Cing::Vector2d(0,0));	markers_cam_1.push_back(Cing::Vector2d(0,0));
	markers_position.push_back(Cing::Vector(0,0,0));	markers_position.push_back(Cing::Vector(0,0,0));	markers_position.push_back(Cing::Vector(0,0,0));

	AveragedVector* tempAvgVector;

	tempAvgVector = new AveragedVector();
	tempAvgVector->init(3);
	markers_AvgPosition.push_back( tempAvgVector);
	tempAvgVector = new AveragedVector();
	tempAvgVector->init(3);
	markers_AvgPosition.push_back( tempAvgVector);
	tempAvgVector = new AveragedVector();
	tempAvgVector->init(3);
	markers_AvgPosition.push_back( tempAvgVector);

	ball_0.init(0.05);
	ball_1.init(0.05);
	ball_2.init(0.05);

	trackedModel.init(0.1,0.1,0.1);

	boneModel.init(1,1,1);
	boneModel.getSceneNode()->getParentSceneNode()->removeChild(boneModel.getSceneNode());
	trackedModel.getSceneNode()->addChild( boneModel.getSceneNode() );


	mandibula_Paloma.init( model_0.getStringAttribute( "mesh" ), model_0.getStringAttribute( "materialName" ), boneModel.getSceneNode() );
	molares_Paloma.init(   model_1.getStringAttribute( "mesh" ), model_1.getStringAttribute( "materialName" ), boneModel.getSceneNode() );
	dentario_Paloma.init(  model_2.getStringAttribute( "mesh" ), model_2.getStringAttribute( "materialName" ), boneModel.getSceneNode() );

	//trackedModel.setPosition(9999,9999,9999);
	// Create the virtual camera
	arCam = Cing::ogreSceneManager->createCamera( "arCam" );
	arCam->setPosition(Ogre::Vector3(0,0,0));
	arCam->lookAt(Ogre::Vector3(0,0,0));
	arCam->setFOVy(Ogre::Degree(FOVy));
	arCam->setAspectRatio( (float)width / height ); 
	arCam->setNearClipDistance(0.1f);
	arCam->setFarClipDistance(50.0f);

	// Lights
	light.init( 90, 90, 90, 0, 0, 0 );

	// Add this camera
	GraphicsManager::getSingleton().getMainWindow().getOgreWindow()->removeAllViewports( );
	GraphicsManager::getSingleton().getMainWindow().getOgreWindow()->addViewport( arCam );

	// FLASH GUI
	FlashMovieManager::getSingleton().init();
	mainMenu.load("AbadiaNet_AR_UI.swf", 512, 512, 0, 0 );
	
	// Regisger global functions for the main menu
	mainMenu.registerCFunction("exitApp", Hikari::FlashDelegate(&exitApp) );
	mainMenu.registerCFunction( "setThreshold",		&setThreshold );
	mainMenu.registerCFunction( "setCalibration",	&setCalibration );
	calibGUI.init( boneModel.getSceneNode(), mainMenu );

	// Other
	Cing::useDefault3DCameraControl(true);
	Cing::applyCoordinateSystemTransform(NORMAL);
	showFps(false);
	showDebugOutput( showDebugOutput_Val );

}

void draw()
{
	background(120);

	// Update cameras 
	cam_0.update();
	cam_1.update();

	cvCvtColor( &cam_0.getCVImage(), &img_0.getCVImage(), CV_BGRA2BGR );
	cvCvtColor( &cam_1.getCVImage(), &img_1.getCVImage(), CV_BGRA2BGR );

	img_0.setUpdateTexture(true);
	img_1.setUpdateTexture(true);


	switch (appState)
	{
	case UNDEFINED:
		// Draw video
		img_0.drawBackground(0,0,width/2,height/2);
		img_1.drawBackground(width/2,0,width/2,height/2);
		break;
	case CAMERA_CALIBRATION:
		tracker3d.calibrateCameras( &img_0.getCVImage(), &img_1.getCVImage() );
		img_0.drawBackground(0,0,width/2,height/2);
		img_1.drawBackground(width/2,0,width/2,height/2);
		break;
	case TRACKER_CALIBRATION:
		if ( tracker3d.isCamerasCalibrated() )
			tracker3d.calibrateTracker( &img_0.getCVImage(), &img_1.getCVImage() );
		img_0.drawBackground(0,0,width/2,height/2);
		img_1.drawBackground(width/2,0,width/2,height/2);
		break;
	case RUN:

		//if ( tracker3d.isCamerasCalibrated() && tracker3d.isTrackerCalibrated())

		break;
	}

	arCam->setPosition(tracker3d.m_cam_0_axis.getPosition());
	arCam->setOrientation(tracker3d.m_cam_0_axis.getOrientation());
	arCam->pitch(Ogre::Degree(180));

	light.setPosition( tracker3d.m_cam_0_axis.getPosition() );

	// Distance form object to camera
	float objectDistance = (arCam->getPosition() - trackedModel.getPosition()).length();
	//println( "dist: %f --- threshold: %i", objectDistance, markerDetector.getThreshold() );

	bool isLocatedInCamera_0 = false;
	bool isLocatedInCamera_1 = false;

	markerDetector.update( img_0 );
	if ( showDebugAxis )
	{
		//Cing::rectMode(Cing::CENTER);
		Cing::smooth();
		for (int i=0;i<markerDetector.m_bFinder.getNumBlobs();i++)
		{
			float x = markerDetector.m_bFinder.getBlobN(i).center.x;
			float y = markerDetector.m_bFinder.getBlobN(i).center.y;

			Cing::stroke( 255, 0, 0);
			Cing::noFill();		
			img_0.ellipse(x, y,  markerDetector.m_bFinder.getBlobN(i).bbox.width, markerDetector.m_bFinder.getBlobN(i).bbox.width);
		}
	}

	if (markerDetector.m_bFinder.getNumBlobs() == 3)
	{
		// Update 2d position for each camera
		for (int i = 0; i < 3; i++)
		{			
			markers_cam_0[i].x = markerDetector.m_bFinder.getBlobN(i).bbox.x;
			markers_cam_0[i].y = markerDetector.m_bFinder.getBlobN(i).bbox.y;
		}
		isLocatedInCamera_0= true;
	}

	markerDetector.update( img_1 );
	if ( showDebugAxis )
	{
		for (int i=0;i<markerDetector.m_bFinder.getNumBlobs();i++)
		{
			float x = markerDetector.m_bFinder.getBlobN(i).center.x;
			float y = markerDetector.m_bFinder.getBlobN(i).center.y;

			Cing::stroke( 0, 0, 255);
			Cing::noFill();		
			img_0.ellipse(x, y,  markerDetector.m_bFinder.getBlobN(i).bbox.width, markerDetector.m_bFinder.getBlobN(i).bbox.width);

		}
	}
	if (markerDetector.m_bFinder.getNumBlobs() == 3)
	{
		// Update 2d position for each camera
		for (int i = 0; i < 3; i++)
		{			
			markers_cam_1[i].x = markerDetector.m_bFinder.getBlobN(i).bbox.x;
			markers_cam_1[i].y = markerDetector.m_bFinder.getBlobN(i).bbox.y;
		}
		isLocatedInCamera_1 = true;
	}

	if ( isLocatedInCamera_0 && isLocatedInCamera_1 )
	{
		// Identify markers by heuristics

		// search first point (x minimum) in both images
		float minX_cam0 = 99999;
		float x0,y0,x1,y1 = 0;
		for (int i = 0; i < 3 ; i++)
		{
			if ( minX_cam0>markers_cam_0[i].x )
			{
				minX_cam0 = markers_cam_0[i].x;
				x0 = markers_cam_0[i].x;
				y0 = markers_cam_0[i].y;
			}
		}

		float minX_cam1 = 99999;

		for (int i = 0; i < 3 ; i++)
		{
			if ( minX_cam1>markers_cam_1[i].x )
			{
				minX_cam1 = markers_cam_1[i].x;
				x1 = markers_cam_1[i].x;
				y1 = markers_cam_1[i].y;
			}
		}

		// Calculate 3d position from 2d coordinates
		markers_position[0] = tracker3d.locatePoint(x0,y0,x1,y1);

		// search second point (x maximum) in both images
		float maxX_cam0 = 0;		
		for (int i = 0; i < 3 ; i++)
		{
			if ( maxX_cam0<markers_cam_0[i].x )
			{
				maxX_cam0 = markers_cam_0[i].x;
				x0 = markers_cam_0[i].x;
				y0 = markers_cam_0[i].y;
			}
		}
		float maxX_cam1 = 0;
		for (int i = 0; i < 3 ; i++)
		{
			if ( maxX_cam1<markers_cam_1[i].x )
			{
				maxX_cam1 = markers_cam_1[i].x;
				x1 = markers_cam_1[i].x;
				y1 = markers_cam_1[i].y;
			}
		}

		// Calculate 3d position from 2d coordinates
		markers_position[2] = tracker3d.locatePoint(x0,y0,x1,y1);


		// search last point (medium) in both images
		for (int i = 0; i < 3 ; i++)
		{
			if ( minX_cam0<markers_cam_0[i].x && maxX_cam0>markers_cam_0[i].x)
			{
				x0 = markers_cam_0[i].x;
				y0 = markers_cam_0[i].y;
			}
		}

		for (int i = 0; i < 3 ; i++)
		{
			if ( minX_cam1<markers_cam_1[i].x && maxX_cam1>markers_cam_1[i].x)
			{
				x1 = markers_cam_1[i].x;
				y1 = markers_cam_1[i].y;
			}
		}

		// Calculate 3d position from 2d coordinates
		markers_position[1] = tracker3d.locatePoint(x0,y0,x1,y1);

	}

	// Average position
	for (int i = 0; i < 3 ; i++)
		markers_AvgPosition[i]->addValue( markers_position[i].x, markers_position[i].y, markers_position[i].z  );
	
	// Update models position
	ball_0.setPosition( *(markers_AvgPosition[0]->getValue()) );
	ball_1.setPosition( *(markers_AvgPosition[1]->getValue()) );
	ball_2.setPosition( *(markers_AvgPosition[2]->getValue()) );

	Cing::Vector position;

	Cing::Vector front;
	Cing::Vector up;
	Cing::Vector side;

	position = (ball_0.getPosition() + ball_1.getPosition() + ball_2.getPosition())/3.0f;

	trackedModel.setPosition(position);

	front = ball_0.getPosition() - ball_2.getPosition();
	front.normalise();
	side =  ball_0.getPosition() - ball_1.getPosition();
	side.normalise();
	up = side.crossProduct( front);
	side = up.crossProduct( front);

	Cing::Quaternion tempRot;
	tempRot.FromAxes(front,side,up);

	// Update and filter orientation
	m_orientation = Ogre::Quaternion::Slerp( 0.7, tempRot, m_orientation, true);
	trackedModel.setOrientation( m_orientation );

	// Debug axis
	if ( showDebugAxis )
	{
		tracker3d.m_cam_0_axis.setVisible( true );
		tracker3d.m_cam_1_axis.setVisible( true );
		tracker3d.m_world_axis.setVisible( true );
		trackedModel.getSceneNode()->setVisible( true, false  );
		boneModel.getSceneNode()->setVisible( true, false  );
		
		ball_0.setVisible( true );
		ball_1.setVisible( true );
		ball_2.setVisible( true );

		stroke(40);
		line(	ball_0.getPosition().x,ball_0.getPosition().y,ball_0.getPosition().z,ball_1.getPosition().x,ball_1.getPosition().y,ball_1.getPosition().z);
		line(	ball_1.getPosition().x,ball_1.getPosition().y,ball_1.getPosition().z,ball_2.getPosition().x,ball_2.getPosition().y,ball_2.getPosition().z);
		line(	ball_2.getPosition().x,ball_2.getPosition().y,ball_2.getPosition().z,ball_0.getPosition().x,ball_0.getPosition().y,ball_0.getPosition().z);
	}else{
		tracker3d.m_cam_0_axis.setVisible( false );
		tracker3d.m_cam_1_axis.setVisible( false );
		tracker3d.m_world_axis.setVisible( false );
		trackedModel.getSceneNode()->setVisible( false, false  );
		boneModel.getSceneNode()->setVisible( false, false  );

		ball_0.setVisible( false );
		ball_1.setVisible( false );
		ball_2.setVisible( false );
	}

	// Draw image
	img_0.setUpdateTexture(true);
	img_0.drawBackground(0,0,width,height);

	// Flash
	FlashMovieManager::getSingleton().update();
}

void end()
{
	cam_0.end();
	cam_1.end();
}

void mousePressed()
{
}

void mouseMoved()
{
}

void mouseReleased()
{
}

void keyPressed()
{

	// Tracker3D params
	if (key == 'c') {
		tracker3d.needCalibration();
	}

	// Camera params
	if (key == 'p') {
		cam_0.setGain( cam_0.getGain() + 1 );
		cam_1.setGain( cam_1.getGain() + 1 );
	}
	if (key == 'ñ') {
		cam_0.setGain( cam_0.getGain() - 1 );
		cam_1.setGain( cam_1.getGain() - 1 );
	}
	if (key == 'o') {
		cam_0.setExposure( cam_0.getExposure() + 1 );
		cam_1.setExposure( cam_1.getExposure() + 1 );
	}
	if (key == 'l') {
		cam_0.setExposure( cam_0.getExposure() - 1 );
		cam_1.setExposure( cam_1.getExposure() - 1 );
	}

	switch (key)
	{
	case '1':
		appState = UNDEFINED;
		break;
	case '2':
		//appState = CAMERA_CALIBRATION;
		break;
	case '3':
		appState = TRACKER_CALIBRATION;
		break;
	case '4':
		appState = RUN;
		break;
	case 's':
		showDebugAxis = !showDebugAxis;
		showDebugOutput_Val = !showDebugOutput_Val;
		showDebugOutput( showDebugOutput_Val );
		break;
	}
}