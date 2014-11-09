#include "artpFunc.h"
#include <math.h>
#include <iostream>
#include <ARToolKitPlus/TrackerSingleMarkerImpl.h>




ARToolKitPlus::TrackerSingleMarker *mTracker;

artpFunc::artpFunc()
{
	
	width = 0;
	height = 0;
	
}

artpFunc::~artpFunc()
{
	if(mTracker)
		delete mTracker;
}

bool artpFunc::init(IplImage* img, std::string cn, std::string markerMode, int threshold, float borderWidth, float patternWidth, float nNearClip, float nFarClip)
{
	width = img->width;
	height = img->height;

	m_cn = cn;
	m_markerMode = markerMode;
	m_threshold = threshold;
	m_borderWidth = borderWidth;
	m_patternWidth = patternWidth;
	m_nNearClip = nNearClip;
	m_nFarClip = nFarClip;

	mTracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6, 6, 6, 1, 8>(img->width,img->height);
	
	if(!mTracker->init(cn.c_str(), nNearClip, nFarClip))
	{
		return false;
	}
	
	mTracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);

	mTracker->setPatternWidth(patternWidth);
	mTracker->setBorderWidth(borderWidth);
	//mTracker->activateAutoThreshold(true);
	mTracker->setThreshold(threshold);
	mTracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
	//mTracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);
	//mTracker->setMarkerMode(ARToolKitPlus::MARKER_ID_BCH);

	

	

	//mTracker->addPattern( pn.c_str() );
	if(markerMode == "BCH")
		mTracker->setMarkerMode(ARToolKitPlus::MARKER_ID_BCH);
	else
		mTracker->setMarkerMode(ARToolKitPlus::MARKER_ID_SIMPLE);

	return true;
}


void artpFunc::resize(IplImage* img)
{
	if(width != img->width || height != img->height)
		return;
	
	if(mTracker)
		delete mTracker;

	init(img,m_cn,m_markerMode,m_threshold,m_borderWidth,m_patternWidth,m_nNearClip,m_nFarClip);
	
}

bool artpFunc::AR_GetPose(RTC::TimedPose3D *pose, RTC::TimedDoubleSeq *trans, IplImage* img, int pattID)
{
	resize(img);
	IplImage* grayscaleImg = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	grayscaleImg->origin = img->origin;
	cvCvtColor(img, grayscaleImg, CV_RGB2GRAY);

	ARToolKitPlus::ARUint8* data = (ARToolKitPlus::ARUint8*)(grayscaleImg->imageData);
	const int markerId = mTracker->calc(data);

	//std::cout << markerId << std::endl;

	if( markerId == pattID )
	{
		const ARFloat* mv = (const ARFloat*)( mTracker->getModelViewMatrix() );
		pose->data.position.x = mv[12];
		pose->data.position.y = mv[13];
		pose->data.position.z = mv[14];

		float pitch = asin(-mv[8]);
		float yaw = asin(mv[9]/cos(pitch));
		float roll = asin(-mv[4]/cos(pitch));

		pose->data.orientation.r = roll;
		pose->data.orientation.p = pitch;
		pose->data.orientation.y = yaw;

		trans->data.length(16);
		for(int i=0;i < 16;i++)
			trans->data[i] = mv[i];

		
		
		
		
	}
	else
	{
		
		if(grayscaleImg)
			cvReleaseImage(&grayscaleImg);
		return false;
	}

	
	if(grayscaleImg)
		cvReleaseImage(&grayscaleImg);

	return true;
	
	
}

