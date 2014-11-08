#include "artpFunc.h"
#include <math.h>
#include <iostream>
#include "ARToolKitPlus/TrackerSingleMarkerImpl.h"







bool AR_GetPose(float *x, float *y, float *z, float *roll, float *pitch, float *yaw, std::string cn, IplImage* img, std::string markerMode, int threshold, float borderWidth, float patternWidth, float nNearClip, float nFarClip)
{
	ARToolKitPlus::TrackerSingleMarker *mTracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6, 6, 6, 1, 8>(img->width,img->height);
	
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
	IplImage* grayscaleImg = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	grayscaleImg->origin = img->origin;
	cvCvtColor(img, grayscaleImg, CV_RGB2GRAY);

	ARToolKitPlus::ARUint8* data = (ARToolKitPlus::ARUint8*)(grayscaleImg->imageData);
	const int markerId = mTracker->calc(data);

	//std::cout << markerId << std::endl;

	if( markerId != -1 )
	{
		const ARFloat* mv = (const ARFloat*)( mTracker->getModelViewMatrix() );
		*x = mv[12];
		*y = mv[13];
		*z = mv[14];

		*pitch = asin(-mv[8]);
		*yaw = asin(mv[9]/cos(*pitch));
		*roll = asin(-mv[4]/cos(*pitch));

		
		
		
		
	}
	else
	{
		if(mTracker)
			delete mTracker;
		if(grayscaleImg)
			cvReleaseImage(&grayscaleImg);
		return false;
	}

	if(mTracker)
		delete mTracker;
	if(grayscaleImg)
		cvReleaseImage(&grayscaleImg);

	return true;
	
	
}

