// -*- C++ -*-
/*!
 * @file  ImageDataCom.cpp
 * @brief 画像データ送受信の関数
 * @date  $Date$
 *
 * $Id$
 */

#include "ImageDataCom.h"

IplImage* GetCameraImage(RTC::CameraImage *ci)
{
	IplImage* m_imageBuff;
	std::string format = ci->format;
	if(format == "jpeg" || format == "png")
	{
		std::vector<uchar> buff;
		int len = ci->pixels.length();
		buff.resize(len);
		memcpy(&buff[0], &ci->pixels[0], sizeof(unsigned char)*len);
		cv::Mat jpegimage = cv::imdecode(cv::Mat(buff),CV_LOAD_IMAGE_COLOR);
		//cv::Mat image;
		//cvtColor(jpegimage, image, CV_RGB2BGR);
				
				
		IplImage m_iplImage = jpegimage;

		m_imageBuff = cvCreateImage(cvSize(ci->width, ci->height), IPL_DEPTH_8U, 3);
		cvCopy( &m_iplImage, m_imageBuff);
				

				
				
				
				
		//*m_imageBuff = image;
				
				
				
				
				

				
				
	}
	else
	{
		if(ci->pixels.length() == ci->width * ci->height * 3)
		{
			m_imageBuff = cvCreateImage(cvSize(ci->width, ci->height), IPL_DEPTH_8U, 3);
			memcpy(m_imageBuff->imageData,(void *)&(ci->pixels[0]),ci->pixels.length());
		}
		else if(ci->pixels.length() == ci->width * ci->height)
		{
			m_imageBuff = cvCreateImage(cvSize(ci->width, ci->height), IPL_DEPTH_8U, 1);
			memcpy(m_imageBuff->imageData,(void *)&(ci->pixels[0]),ci->pixels.length());
				
		}
	}
	return m_imageBuff;
	
}

void SetCameraImage(RTC::CameraImage *ci, IplImage* im, std::string string_encode, int int_encode_quality)
{
  ci->width = im->width;
  ci->height = im->height;

  ci->format = string_encode.c_str();

  if(string_encode == "off")
  {
	  int len = im->nChannels * im->width * im->height;
	  ci->pixels.length(len);

	  memcpy((void *)&(ci->pixels[0]),im->imageData,len);
  }
  else if(string_encode == "jpeg")
  {
	  std::vector<int> param = std::vector<int>(2);
	  std::vector<uchar> buff;
	  param[0] = CV_IMWRITE_JPEG_QUALITY;
	  if(0 <= int_encode_quality && int_encode_quality <= 100){
		param[1] = int_encode_quality;
	  }
	  else{
		param[1] = 75;
		
	  }
	  
	  cv::Mat image = cv::cvarrToMat(im);

	  cv::imencode(".jpg", image ,buff, param);

	  int len = buff.size();
	  ci->pixels.length(len);
	  memcpy(&ci->pixels[0],&buff[0], sizeof(unsigned char)*len);
  }
  else if(string_encode == "png")
  {
	  std::vector<int> param = std::vector<int>(2);
	  std::vector<uchar> buff;
	  param[0] = CV_IMWRITE_PNG_COMPRESSION;

	  if(0 <= int_encode_quality && int_encode_quality <= 9){
		param[1] = int_encode_quality;
	  }
	  else{
		param[1] = 3;
		
	  }

	  cv::Mat image = cv::cvarrToMat(im);

	  cv::imencode(".png", image ,buff, param);

	  int len = buff.size();
	  ci->pixels.length(len);
	  memcpy(&ci->pixels[0],&buff[0], sizeof(unsigned char)*len);
  }
}