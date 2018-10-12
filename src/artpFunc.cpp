#include "artpFunc.h"
#include <math.h>
#include <iostream>


#include <ARToolKitPlus/TrackerSingleMarker.h>




ARToolKitPlus::TrackerSingleMarker *mTracker;

artpFunc::artpFunc()
{
	
	width = 0;
	height = 0;

	std::min(1, 2);
	
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

	

	mTracker = new ARToolKitPlus::TrackerSingleMarker(img->width, img->height, 8, 6, 6, 6, 0);
	
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

	unsigned char * data = (unsigned char *)(grayscaleImg->imageData);
	std::vector<int> markerId = mTracker->calc(data);
	mTracker->selectBestMarkerByCf();
	float conf = mTracker->getConfidence();

	/*std::cout << markerId.size() << "\t" << conf << std::endl;
	for (int i = 0; i < 4; i++)
	{

		std::cout << mTracker->getModelViewMatrix()[i * 4] << "\t" << mTracker->getModelViewMatrix()[i * 4 + 1] << "\t" << mTracker->getModelViewMatrix()[i * 4 + 2] << "\t" << mTracker->getModelViewMatrix()[i * 4 + 3] << std::endl;

	}
	*/

	if (markerId.size() > 0)
	{
		//std::cout << "ID: " << markerId[0] << std::endl;
		if (markerId[0] == pattID)
		{
			const ARFloat* mv = (const ARFloat*)(mTracker->getModelViewMatrix());
			pose->data.position.x = mv[14] / 1000.;
			pose->data.position.y = -mv[12] / 1000.;
			pose->data.position.z = -mv[13] / 1000.;

			float yaw = asin(-mv[8]);
			float pitch = asin(mv[9] / cos(yaw));
			float roll = asin(-mv[4] / cos(yaw));

			pose->data.orientation.r = roll;
			pose->data.orientation.p = pitch;
			pose->data.orientation.y = yaw;

			trans->data.length(16);
			for (int i = 0; i < 16; i++)
				trans->data[i] = mv[i];





		}
		else
		{

			if (grayscaleImg)
				cvReleaseImage(&grayscaleImg);
			return false;
		}
	}
	
	if(grayscaleImg)
		cvReleaseImage(&grayscaleImg);

	return true;
	
	
}

