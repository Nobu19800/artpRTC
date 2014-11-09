// -*- C++ -*-
/*!
 * @file  ImageDataCom.h
 * @brief 画像データ送受信の関数
 * @date  $Date$
 *
 * $Id$
 */

#ifndef IMAGEDATACOM_H
#define IMAGEDATACOM_H

#include <string>
#include <rtm/idl/InterfaceDataTypesSkel.h>
#include "InterfaceDataTypesStub.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>


/**
*@brief CameraImage型からIplImageの画像データを取得
* @param ci CameraImage型の画像データ
* @return IplImageの画像データ
*/
IplImage* GetCameraImage(RTC::CameraImage *ci);

/**
*@brief IplImageの画像データをCameraImage型のデータに設定
* @param ci CameraImage型のデータ
* @param im IplImageの画像データ
* @param string_encode 圧縮の形式(offは圧縮なし、png、jpegを選択可能)
* @param int_encode_quality 圧縮率(pngは0～10、jpegは0～100)
*/
void SetCameraImage(RTC::CameraImage *ci, IplImage* im, std::string string_encode = "off", int int_encode_quality = 75);


#endif