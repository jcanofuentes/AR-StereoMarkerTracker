//--------------------------------------------------------------------------------------------
// Project: AR tracking system based on stereo vision
// Brief:   Tracker3d.cpp
// Author:  Jorge Cano
// Date:    29/07/2009
// 
//--------------------------------------------------------------------------------------------
// 	Copyright (c) 2009 Jorge Cano
//--------------------------------------------------------------------------------------------

#include "Tracker3d.h"
#include "Graphics/ImageResourceManager.h"
#include "Graphics/GraphicsUserApi.h"
#include "Common/CommonTypes.h"

#define BOARD_WIDTH  5
#define BOARD_HEIGHT 4
#define PATTERN_SIZE cvSize(BOARD_WIDTH - 1, BOARD_HEIGHT - 1)

Tracker3d::Tracker3d() :	m_bIsValid( false ),
m_debug(true),
m_isCamerasCalibrated(false),
m_frameskip(0),
m_calibFilter(NULL),
m_isTrackerCalibrated(false)
{

};

Tracker3d::~Tracker3d()
{
	// Release resources
	end();
};

void Tracker3d::init  (int nCameras, int cameraWidth, int cameraHeight)
{
	m_images	= new IplImage*[nCameras];	
	m_res		= new IplImage*[nCameras];

	// Init image resources ( images and more... )
	CvSize imgSize = cvSize(cameraWidth,cameraHeight);

	/*
	for(int i =0;i<nCameras;i++)
	{
		m_images[i]	= cvCreateImage(imgSize, IPL_DEPTH_8U, 3); // Real resolution and nChannels of input images
		m_res[i]	= cvCreateImage(imgSize, IPL_DEPTH_8U, 3); // Real resolution and nChannels of input images
	}
	*/

	// Init calibration filter and etalon params
	m_calibFilter = new CvCalibFilter();

	m_calibFilter->SetCameraCount(nCameras);

	m_world_axis.init(.1,.1,.1);	
	m_cam_0_axis.init(.000000005,.000000005,.000000005);
	m_cam_1_axis.init(.000000005,.000000005,.000000005);
/*
	m_world_axis.setPosition(9999,9999,9999);	
	m_cam_0_axis.setPosition(9999,9999,9999);	
	m_cam_1_axis.setPosition(9999,9999,9999);	
*/
	// Init class
	m_bIsValid = true;
};

bool Tracker3d::loadCameraCalibration(const char* fileName)
{

	// Load calibration file
	bool calibSuccess = m_calibFilter->LoadCameraParams(fileName);

	if (calibSuccess)
	{
		m_isCamerasCalibrated = true;

		for (int i=0; i<m_numCameras; i++)
		{

			const CvCamera *cvcam = m_calibFilter->GetCameraParams(i);
			m_camIntrinsics[i].principal_point.x = cvcam->matrix[2];
			m_camIntrinsics[i].principal_point.y = cvcam->matrix[5];
			m_camIntrinsics[i].focal_length[0]   = cvcam->matrix[0];
			m_camIntrinsics[i].focal_length[1]   = cvcam->matrix[4];
			m_camIntrinsics[i].distortion[0]     = cvcam->distortion[0];
			m_camIntrinsics[i].distortion[1]     = cvcam->distortion[1];
			m_camIntrinsics[i].distortion[2]     = cvcam->distortion[2];
			m_camIntrinsics[i].distortion[3]     = cvcam->distortion[3];
			println("Intrinsic parameters for camera %i", i);
			println("principal_point: %f, %f", cvcam->matrix[2],cvcam->matrix[5]);
			println("focal_length: %f, %f", cvcam->matrix[0],cvcam->matrix[4]);
		}
	} else {
		m_isCamerasCalibrated = false;
		// We have a 8x6 checkerboard of unit squares
		double	etalonParams[3];
		etalonParams[0] = 8; // 8;
		etalonParams[1] = 6; // 6;
		etalonParams[2] = 1;
		m_calibFilter->SetEtalon(CV_CALIB_ETALON_CHECKERBOARD, etalonParams);
		println("Calibration file %s not found", fileName);

	}

	return calibSuccess;
};

void Tracker3d::calibrateCameras( IplImage* cam_0, IplImage* cam_1 )
{
	println("Performing camera calibration");
	m_images[0] = cam_0;
	m_images[1] = cam_1;

	if (!m_calibFilter->IsCalibrated()) {
		if (m_calibFilter->FindEtalon((IplImage**)m_images))
		{
			// Only use every 30th frame, to give the operator a chance to move around the checkerboard 
			if (m_frameskip)
			{
				m_frameskip--;
			}else{
				// We found our checkerboard points in this image, use it to calibrate 
				CvPoint2D32f *points[2];
				int pointCount;
				bool found;
				m_calibFilter->GetLatestPoints(0, &points[0], &pointCount, &found);
				m_calibFilter->GetLatestPoints(1, &points[1], &pointCount, &found);
				m_calibFilter->Push((const CvPoint2D32f**) points);
				m_frameskip = 30;
				println("Accepted frame %d", m_calibFilter->GetFrameCount()+1);
			}
		}
		// Draw points even if we haven't found the complete checkerboard yet 
		m_calibFilter->DrawPoints(m_images);

	}else{
		// Calibration done! Save params to file
		m_calibFilter->SaveCameraParams("camera.calibration.txt");
		m_isCamerasCalibrated = true;
		println("Calibration done");
	}
};

void Tracker3d::update()
{
	if ( !isValid() ) return;
};


// Once the cameras is calibrated, this function recalibrate for new pos/rot of cameras?
void Tracker3d::calibrateTracker( IplImage* cam_0, IplImage* cam_1 )
{
	//println("Performing tracking calibration");

	// Undistort images
	m_images[0] = cam_0;
	m_images[1] = cam_1;
	m_calibFilter->Undistort(m_images, m_images);

	// Try to calibrate. This updates cam_info, and returns true if we're done 
	memset(&m_camInfo, 0, sizeof(m_camInfo));

	// Etalon parameters are different
	if (cv3dTrackerCalibrateCameras( m_numCameras, m_camIntrinsics, PATTERN_SIZE, 1, m_images, m_camInfo )) 
	{
		//println("Finished 3dtracker calibration");

		// Debug world and camera axis

		Cing::Vector		tempPos;
		Cing::Vector		tempX;
		Cing::Vector		tempY;
		Cing::Vector		tempZ;
		Cing::Quaternion	tempRot;

		// extract rotation axis and update debug model orientation
		
		tempX.x = m_camInfo[0].mat[0][0];
		tempX.y = m_camInfo[0].mat[0][1];
		tempX.z = m_camInfo[0].mat[0][2];

		tempY.x = m_camInfo[0].mat[1][0];
		tempY.y = m_camInfo[0].mat[1][1];
		tempY.z = m_camInfo[0].mat[1][2];

		tempZ.x = m_camInfo[0].mat[2][0];
		tempZ.y = m_camInfo[0].mat[2][1];
		tempZ.z = m_camInfo[0].mat[2][2];
		
		tempRot.FromAxes(-tempX*m_camIntrinsics[0].focal_length[0], -tempY*m_camIntrinsics[0].focal_length[1], -tempZ);
		m_cam_0_axis.setOrientation( tempRot );

		// extract position vector and update debug model position
		tempPos.x = m_camInfo[0].mat[3][0];
		tempPos.y = m_camInfo[0].mat[3][1];
		tempPos.z = m_camInfo[0].mat[3][2];
		m_cam_0_axis.setPosition( tempPos );


		// extract rotation axis and update debug model orientation
		tempX.x = m_camInfo[1].mat[0][0];
		tempX.y = m_camInfo[1].mat[0][1];
		tempX.z = m_camInfo[1].mat[0][2];

		tempY.x = m_camInfo[1].mat[1][0];
		tempY.y = m_camInfo[1].mat[1][1];
		tempY.z = m_camInfo[1].mat[1][2];

		tempZ.x = m_camInfo[1].mat[2][0];
		tempZ.y = m_camInfo[1].mat[2][1];
		tempZ.z = m_camInfo[1].mat[2][2];

		tempRot.FromAxes(-tempX*m_camIntrinsics[1].focal_length[0], -tempY*m_camIntrinsics[1].focal_length[1], -tempZ);
		m_cam_1_axis.setOrientation( tempRot );

		// extract position vector and update debug model position
		tempPos.x = m_camInfo[1].mat[3][0];
		tempPos.y = m_camInfo[1].mat[3][1];
		tempPos.z = m_camInfo[1].mat[3][2];
		m_cam_1_axis.setPosition( tempPos );

		m_isTrackerCalibrated		= true;	
	}

	// Draw an X or a check on each camera to indicate current calibration status 
	for (int i=0; i<m_numCameras; i++) {
		IplImage *image = m_images[i];
		if (m_camInfo[i].valid) {
			//Green check 
			cvLineAA(image, cvPoint(10, 64), cvPoint(50, 128), 140);
			cvLineAA(image, cvPoint(50, 128), cvPoint(100, 10), 140);
		}
		else {
			//Red X 
			cvLineAA(image, cvPoint(10, 10), cvPoint(128, 128), 255);
			cvLineAA(image, cvPoint(10, 128), cvPoint(128, 10), 255);
		}
	}

}


Cing::Vector Tracker3d::locatePoint(float x1, float y1, float x2, float y2 )
{
	Cing::Vector tempVector;

		// Initialize objects info (-1 --> slot not used)
		for (int i = 0; i < m_numCameras; i++) {
			for (int j= 0; j < m_numObjects; j++) {
				m_object2d[i * m_numObjects + j].id = -1;
			}
		}

		m_object2d[0].id = 0;
		m_object2d[0].p.x = x1; 
		m_object2d[0].p.y = y1; 

		m_object2d[1].id = 0;
		m_object2d[1].p.x = x2; 
		m_object2d[1].p.y = y2; 

		int result =  cv3dTrackerLocateObjects( m_numCameras, m_numObjects, m_camInfo,	m_object2d,	m_object );

		if (result==1)
		{
			tempVector.x = m_object[0].p.x;
			tempVector.y = m_object[0].p.y;
			tempVector.z = m_object[0].p.z;
		}

	return tempVector;
}

void Tracker3d::undistort( IplImage* cam_0, IplImage* cam_1 )
{
	//println("Performing undistortion ");

	m_images[0] = cam_0;
	m_images[1] = cam_1;

	m_calibFilter->Undistort(m_images, m_images);

}

void Tracker3d::undistort( CvMat* inPoints, CvMat* outPoints )
{
	//println("Performing undistortion 1");

	const CvCamera *cvcam = m_calibFilter->GetCameraParams(0);

	CvMat* intrinsic_matrix = cvCreateMat(3,3,CV_32F);
	for(int i=0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			CV_MAT_ELEM(*intrinsic_matrix, float, i, j) = cvcam->matrix[i+j*3];
		}
	}
	CvMat* distortion_coeffs = cvCreateMat(4,1,CV_32F);
	for(int i=0; i < 4; i++) {
		CV_MAT_ELEM(*distortion_coeffs, float, i, 0) = cvcam->distortion[i];
	}
    cvUndistortPoints(inPoints, outPoints, intrinsic_matrix, distortion_coeffs);
}

Cing::Vector2d Tracker3d::undistort( float _x, float _y)
{
	//println("Performing undistortion 2");
	const CvCamera *cvcam = m_calibFilter->GetCameraParams(0);

	float u0 = cvcam->matrix[2],
		  v0 = cvcam->matrix[5],
		  fx = cvcam->matrix[0],
		  fy = cvcam->matrix[4];
	float _fx = 1.0/fx,
		  _fy = 1.0/fy;
	float k1 = cvcam->distortion[0],
		k2 = cvcam->distortion[1],
		p1 = cvcam->distortion[2],
		p2 = cvcam->distortion[3];

	float y         = (_y - v0)*_fy;
	float y2      = y*y;
	float ky      = 1 + (k1 + k2*y2)*y2;
	float k2y      = 2*k2*y2;
	float _2p1y      = 2*p1*y;
	float _3p1y2   = 3*p1*y2;
	float p2y2      = p2*y2;

	float x      = (_x - u0)*_fx;
	float x2   = x*x;
	float kx   = (k1 + k2*x2)*x2;
	float d      = kx + ky + k2y*x2;
	float _u   = fx*(x*(d + _2p1y) + p2y2 + (3*p2)*x2) + u0;
	float _v   = fy*(y*(d + (2*p2)*x) + _3p1y2 + p1*x2) + v0;

	return Cing::Vector2d( _u,_v);  
}

void Tracker3d::end()
{
	// Check if the class is already released
	if ( !isValid() )
		return;

	// TODO: Release resources
	// The class is not valid anymore
	m_bIsValid = false;
};