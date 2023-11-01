//--------------------------------------------------------------------------------------------
// Project: AR tracking system based on stereo vision
// Brief:   MarkerDetector.cpp
// Author:  Jorge Cano
// Date:    30/07/2009
// 
//--------------------------------------------------------------------------------------------
// 	Copyright (c) 2009 Jorge Cano
//--------------------------------------------------------------------------------------------

#include "MarkerDetector.h"


/**
* @internal
* @brief Constructor. Initializes class attributes.
*/
MarkerDetector::MarkerDetector():
m_bIsValid        ( false ),
m_threshold			(180)
{
}

/**
* @internal
* @brief Destructor. Class release.
*/
MarkerDetector::~MarkerDetector()
{
	// Release resources
	end();
}

/**
* @internal
* @brief Releases the class resources. 
* After this method is called the class is not valid anymore.
*/
void MarkerDetector::end()
{
	// Check if the class is already released
	if ( !isValid() )
		return;

	m_bIsValid = false;
}

/**
* @internal
* @brief 
*/

void MarkerDetector::update(  Cing::Image& inImage )
{

	// Check intialization
	if ( !isValid() )
	{
		// Init resources
		m_tempInputImage = cvCreateImage( cvSize(inImage.getWidth(), inImage.getHeight()), 8, 1);
		m_tempImage.init( inImage.getWidth(), inImage.getHeight(), Cing::GRAYSCALE );

		// The class is now initialized
		m_bIsValid = true;
	}

	// If they have different number of channels -> convert them
	if ( inImage.getNChannels() == 3 )
		cvCvtColor( &inImage.getCVImage(), &m_tempImage.getCVImage(), CV_BGR2GRAY );
	else 
		cvCopy( &inImage.getCVImage(), &m_tempImage.getCVImage() );

	// Filter image
	cvThreshold( &m_tempImage.getCVImage(), &m_tempImage.getCVImage(), m_threshold, 255, CV_THRESH_BINARY );
	//cvDilate( &m_tempImage.getCVImage(), &m_tempImage.getCVImage()) ;
	
	// Find contours
	m_bFinder.setMinBlobArea( 4 );
	m_bFinder.setMaxBlobArea( 900 );
	m_bFinder.update( m_tempImage );

}
