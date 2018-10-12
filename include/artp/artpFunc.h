// -*- C++ -*-
/*!
 * @file  artpFunc.h
 * @brief 
 */

#ifndef ARTPFUNC_H
#define ARTPFUNC_H



#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/idl/ExtendedDataTypesSkel.h>

#include <ARToolKitPlus/TrackerSingleMarker.h>



class artpFunc
{
public:
	artpFunc();
	~artpFunc();
	bool AR_GetPose(RTC::TimedPose3D *pose, RTC::TimedDoubleSeq *trans,IplImage* img ,int pattID = 0);
	void resize(IplImage* img);
	bool init(IplImage* img, std::string cn, std::string markerMode = "SIMPLE", int threshold = 150, float borderWidth = 0.25, float patternWidth = 80, float nNearClip = 1.0, float nFarClip = 1000.0);
	 std::string m_cn;
	std::string m_markerMode;
	 int m_threshold;
	 float m_borderWidth;
	 float m_patternWidth;
	 float m_nNearClip;
	 float m_nFarClip;
	 int width, height;

	 ARToolKitPlus::TrackerSingleMarker *mTracker;

};
#endif
