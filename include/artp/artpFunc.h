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




	

bool AR_GetPose(float *x, float *y, float *z, float *roll, float *pitch, float *yaw, std::string cn, IplImage* img, std::string markerMode = "SIMPLE", int threshold = 150, float borderWidth = 0.25, float patternWidth = 80, float nNearClip = 1.0, float nFarClip = 1000.0);

#endif
