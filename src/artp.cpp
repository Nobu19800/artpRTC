// -*- C++ -*-
/*!
 * @file  artp.cpp
 * @brief ARToolKitPlus Component
 * @date $Date$
 *
 * $Id$
 */

#include "artp.h"


// Module specification
// <rtc-template block="module_spec">
static const char* artp_spec[] =
  {
    "implementation_id", "artp",
    "type_name",         "artp",
    "description",       "ARToolKitPlus Component",
    "version",           "1.0.0",
    "vendor",            "Miyamoto Nobuhiko",
    "category",          "TEST",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.markerMode", "SIMPLE",
    "conf.default.camerafile", "data/LogitechPro4000.dat",
	"conf.default.threshold", "150",
	"conf.default.borderWidth", "0.25",
	"conf.default.patternWidth", "80",
	"conf.default.nNearClip", "1.0",
	"conf.default.nFarClip", "1000.0",
    // Widget
    "conf.__widget__.markerMode", "radio",
    "conf.__widget__.camerafile", "text",
	"conf.__widget__.threshold", "text",
	"conf.__widget__.borderWidth", "text",
	"conf.__widget__.patternWidth", "text",
	"conf.__widget__.nNearClip", "text",
	"conf.__widget__.nFarClip", "text",
    // Constraints
	"conf.__constraints__.markerMode", "(SIMPLE,BCH)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
artp::artp(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_imageIn("image", m_image),
    m_posOut("pos", m_pos)

    // </rtc-template>
{
	
}

/*!
 * @brief destructor
 */
artp::~artp()
{
}



RTC::ReturnCode_t artp::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("image", m_imageIn);
  
  // Set OutPort buffer
  addOutPort("pos", m_posOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("markerMode", m_markerMode, "SIMPLE");
  bindParameter("camerafile", m_camerafile, "data/LogitechPro4000.dat");
  bindParameter("threshold", m_threshold, "150");
  bindParameter("borderWidth", m_borderWidth, "0.25");
  bindParameter("patternWidth", m_patternWidth, "80");
  bindParameter("nNearClip", m_nNearClip, "1.0");
  bindParameter("nFarClip", m_nFarClip, "1000.0");
  


  
  // </rtc-template>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t artp::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artp::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artp::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t artp::onActivated(RTC::UniqueId ec_id)
{
	

	

  return RTC::RTC_OK;
}


RTC::ReturnCode_t artp::onDeactivated(RTC::UniqueId ec_id)
{
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t artp::onExecute(RTC::UniqueId ec_id)
{
	if(m_imageIn.isNew())
	{
		m_imageIn.read();
			
				
 
				 
		if(m_imageBuff != NULL)
		{
			cvReleaseImage(&m_imageBuff);
         
		}

		m_imageBuff = GetCameraImage(&m_image);

		
		float x,y,z,roll,pitch,yaw;
		if(!AR_GetPose(&x,&y,&z,&roll,&pitch,&yaw,m_camerafile,m_imageBuff,m_markerMode,m_threshold,m_borderWidth,m_patternWidth,m_nNearClip,m_nFarClip))
		{
			return RTC::RTC_OK;
		}
		std::cout << x << "\t" << y << "\t" << z << "\t" << roll << "\t"  << pitch << "\t" << yaw << std::endl;
		m_pos.data.position.x = x;
		m_pos.data.position.y = y;
		m_pos.data.position.z = z;
		m_pos.data.orientation.r = roll;
		m_pos.data.orientation.p = pitch;
		m_pos.data.orientation.y = yaw;
		m_posOut.write();


	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t artp::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artp::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artp::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artp::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t artp::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void artpInit(RTC::Manager* manager)
  {
    coil::Properties profile(artp_spec);
    manager->registerFactory(profile,
                             RTC::Create<artp>,
                             RTC::Delete<artp>);
  }
  
};


