// -*- C++ -*-
/*!
 * @file  BumperController.cpp
 * @brief Bumper Controller
 * @date $Date$
 *
 * $Id$
 */

#include "BumperController.h"

// Module specification
// <rtc-template block="module_spec">
static const char* bumpercontroller_spec[] =
  {
    "implementation_id", "BumperController",
    "type_name",         "BumperController",
    "description",       "Bumper Controller",
    "version",           "1.0.0",
    "vendor",            "yumin05cim",
    "category",          "Test",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.initialVelocityX", "0.0",
    "conf.default.avoidVelocityX", "-0.1",
    // Widget
    "conf.__widget__.initialVelocityX", "text",
    "conf.__widget__.avoidVelocityX", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
BumperController::BumperController(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_bumperIn("bumper", m_bumper),
    m_targetVelocityOut("targetVelocity", m_targetVelocity)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
BumperController::~BumperController()
{
}



RTC::ReturnCode_t BumperController::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("bumper", m_bumperIn);
  
  // Set OutPort buffer
  addOutPort("targetVelocity", m_targetVelocityOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("initialVelocityX", m_initialVelocityX, "0.0");
  bindParameter("avoidVelocityX", m_avoidVelocityX, "-0.1");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BumperController::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BumperController::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BumperController::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t BumperController::onActivated(RTC::UniqueId ec_id)
{

	m_Mode = SEARCH_MODE;

  return RTC::RTC_OK;
}


RTC::ReturnCode_t BumperController::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t BumperController::onExecute(RTC::UniqueId ec_id)
{

  	if(this->m_bumperIn.isNew()) {
		m_bumperIn.read();

		//std::cout << m_bumper.data.length();

		int bumper_r = m_bumper.data[0];
		int bumper_f = m_bumper.data[1];
		int bumper_l = m_bumper.data[2];
	
		std::cout <<  "right = " << bumper_r << ", ";
		std::cout <<  "front = " << bumper_f << ", ";
		std::cout <<   "left = " << bumper_l << std::endl;
		
		if(m_Mode == SEARCH_MODE){
			m_targetVelocity.data.vx = m_initialVelocityX;
			m_targetVelocity.data.vy = 0;
			m_targetVelocity.data.va = 0;

			if(bumper_f){
				m_Mode = FRONT_AVOID_MODE;
				m_StartTime = coil::gettimeofday();
				//m_Timer.tick();
			}
			if(bumper_r){
				m_Mode = RIGHT_AVOID_MODE;
				m_StartTime = coil::gettimeofday();
				//m_Timer.tick();
			}
			if(bumper_l){
				m_Mode = LEFT_AVOID_MODE;
				m_StartTime = coil::gettimeofday();
				//m_Timer.tick();
			}
		}

		if(m_Mode == FRONT_AVOID_MODE){
			coil::TimeValue now = coil::gettimeofday();
			double durationSec = now - m_StartTime;

			//m_Timer.tack();
			//coil::TimeValue duration = m_Timer.interval();
			//double durationSec = duration.sec() + duration.usec() / 1000000.0;

			m_targetVelocity.data.vx = m_avoidVelocityX;
			m_targetVelocity.data.vy = 0;
			m_targetVelocity.data.va = 0.5;

			if(durationSec > 2.0){
				m_Mode = SEARCH_MODE;
			}
		}
		if(m_Mode == RIGHT_AVOID_MODE){
			coil::TimeValue now = coil::gettimeofday();
			double durationSec = now - m_StartTime;

			m_targetVelocity.data.vx = m_avoidVelocityX;
			m_targetVelocity.data.vy = 0;
			m_targetVelocity.data.va = 0.5;

			if(durationSec > 2.0){
				m_Mode = SEARCH_MODE;
			}
		}
		if(m_Mode == LEFT_AVOID_MODE){
			coil::TimeValue now = coil::gettimeofday();
			double durationSec = now - m_StartTime;

			m_targetVelocity.data.vx = m_avoidVelocityX;
			m_targetVelocity.data.vy = 0;
			m_targetVelocity.data.va = -0.5;

			if(durationSec > 2.0){
				m_Mode = SEARCH_MODE;
			}
		}

		std::cout << "Mode = " << m_Mode << std::endl;

		std::cout <<  "vx = " << m_targetVelocity.data.vx << ", ";
		std::cout <<  "vy = " << m_targetVelocity.data.vy << ", ";
		std::cout <<  "va = " << m_targetVelocity.data.va << std::endl;

		m_targetVelocityOut.write();
	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t BumperController::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BumperController::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BumperController::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BumperController::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t BumperController::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void BumperControllerInit(RTC::Manager* manager)
  {
    coil::Properties profile(bumpercontroller_spec);
    manager->registerFactory(profile,
                             RTC::Create<BumperController>,
                             RTC::Delete<BumperController>);
  }
  
};


