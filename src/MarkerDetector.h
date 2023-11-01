//--------------------------------------------------------------------------------------------
// Project: AR tracking system based on stereo vision
// Brief:   MarkerDetector.h
// Author:  Jorge Cano
// Date:    30/07/2009
// 
//--------------------------------------------------------------------------------------------
// 	Copyright (c) 2009 Jorge Cano
//--------------------------------------------------------------------------------------------

#ifndef _MarkerDetector_H_
#define _MarkerDetector_H_

// Computer vision
#include "computervision/BlobFinder.h"

/**
 * @internal
 * Class to find squares in images
 */
class MarkerDetector
{
public:
	// Constructor / Destructor
	MarkerDetector();
	~MarkerDetector();

	// BaseCameraInput like interface
	void  end			();
	void  update		(  Cing::Image& inImage );
	void  setThreshold	(  int threshold ) { m_threshold = threshold; };
	int	  getThreshold(){	return m_threshold; };

	// Query methods
	bool  isValid() const	{ return m_bIsValid; }

	Cing::BlobFinder		m_bFinder;

private:

	int						m_threshold;
	Cing::Image				m_tempImage;
	IplImage*				m_tempInputImage;
	bool					m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
};


#endif // _MarkerDetector_H_
