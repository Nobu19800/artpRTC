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
	"conf.default.patternID", "0",
    // Widget
    "conf.__widget__.markerMode", "radio",
    "conf.__widget__.camerafile", "text",
	"conf.__widget__.threshold", "text",
	"conf.__widget__.borderWidth", "text",
	"conf.__widget__.patternWidth", "text",
	"conf.__widget__.nNearClip", "text",
	"conf.__widget__.nFarClip", "text",
	"conf.__widget__.patternID", "text",
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
    m_posOut("pos", m_pos),
	m_transOut("trans", m_trans)

    // </rtc-template>
{
	m_artpFunc = NULL;
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
  addOutPort("trans", m_transOut);
  
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
  bindParameter("patternID", m_patternID, "0");
  


  
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
	if(m_artpFunc)
		delete m_artpFunc;
	m_artpFunc = NULL;
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

		if(m_artpFunc == NULL)
		{
			m_artpFunc = new artpFunc();
			if(!m_artpFunc->init(m_imageBuff,m_camerafile,m_markerMode,m_threshold,m_borderWidth,m_patternWidth,m_nNearClip,m_nFarClip))
			{
				return RTC::RTC_ERROR; 
			}
		}
		
		
		if(!m_artpFunc->AR_GetPose(&m_pos,&m_trans,m_imageBuff,m_patternID))
		{
			return RTC::RTC_OK;
		}
		std::cout << m_pos.data.position.x << "\t" << m_pos.data.position.y << "\t" << m_pos.data.position.z << "\t" << m_pos.data.orientation.r << "\t"  << m_pos.data.orientation.p << "\t" << m_pos.data.orientation.y << std::endl;
		
		m_posOut.write();
		m_transOut.write();


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


