//--------------------------------------------------------------------------------------------
// Project: AR tracking system based on stereo vision
// Brief:   Tracker3d.h
// Author:  Jorge Cano
// Date:    29/07/2009
// 
//--------------------------------------------------------------------------------------------
// 	Copyright (c) 2009 Jorge Cano
//--------------------------------------------------------------------------------------------

#ifndef _Tracker3d_H
#define _Tracker3d_H

#include "Cing.h"
#include "OpenCV/cvaux/include/cvaux.h"
#include "Common/CommonTypes.h"
#include "Axis.h"

class Tracker3d
{
public:
	 
	// Constructor / Destructor
	Tracker3d();
	~Tracker3d();

	// Init / Release
	void init  (int nCameras, int cameraWidth, int cameraHeight);
	void update();
	void end   ();

	// Query methods
	bool isValid () const { return m_bIsValid; }

	//Load cameras params
	bool loadCameraCalibration(const char* fileName);

	// Camera calibration and tracking methods
	void calibrateCameras( IplImage* cam_0, IplImage* cam_1 );	// TODO: Add comment
	void calibrateTracker( IplImage* cam_0, IplImage* cam_1 );						// TODO: Add comment
	void performTracking();										// TODO: Add comment
    void undistort(IplImage* cam_0, IplImage* cam_1);
	void undistort( CvMat* inPoints, CvMat* outPoints );
	Cing::Vector2d undistort( float _x, float _y );

	// Start tracker calibration
	//void startTrackerCalibration() { m_isTrackerCalibrated = false; };

	Cing::Vector locatePoint(float x1, float y1, float x2, float y2 );
	

	bool isCamerasCalibrated() { return m_isCamerasCalibrated; };
	bool isTrackerCalibrated() { return m_isTrackerCalibrated; };
	void needCalibration() { m_isTrackerCalibrated = false; };


	bool fromFile; 
	bool debug; 

	CvMat* fundamental_matrix;

	static const int m_numObjects = 1;	///< Number of tracked objects

	Cv3dTrackerTrackedObject		m_object[m_numObjects];						///< 3dTracking
	Axis m_world_axis;
	Axis m_cam_0_axis;
	Axis m_cam_1_axis;

private:
	static const int m_numCameras = 2;	///< Number of cameras. Only two cameras supported
	IplImage**	m_images;				///< Image inputs     ( 320x240 GrayScale )
	IplImage**	m_res;					///< Rectified images ( 320x240 GrayScale )
	
	int				m_frameskip;		///< Counter used in camera calibration mode
	CvCalibFilter*	m_calibFilter;		///< OCV Camera calibration filter object

	Cv3dTrackerCameraInfo			m_camInfo[m_numCameras];		///< 3dTracking
	Cv3dTrackerCameraIntrinsics		m_camIntrinsics[m_numCameras];	///< 3dTracking

	Cv3dTracker2dTrackedObject		m_object2d[m_numCameras * m_numObjects];		///< Tracking



	bool m_bIsValid;				///< Indicates whether the class is valid or not
	bool m_debug;					///< Debug flag
	bool m_isCamerasCalibrated;		///< Indicates whether each camera is calibrated
	bool m_isTrackerCalibrated;		///< Indicates whether the tracker3d is calibrated
};

#endif // _Tracker3d_H