#ifndef _ATTITUDE_CONTROL_H_
#define _ATTITUDE_CONTROL_H_

#include "stm32f10x.h"
#include "Remoter.h"
#include "AHRS.h"
#include "ESCMotor.h"
#include "PositionLoop.h"
#include "VelocityLoop.h"
#include "mathtool.h"
#include "Servo.h"

class AttitudeControl
{
public:
	Remoter &_rc;
	AHRS    &_ahrs;
	Servo &_servo;
	PositionLoop _posLoop;
	VelocityLoop _velLoop;

	CraftVector _curAngle;   //current angle
	CraftVector _tagAngle;   //target angle
	CraftVector _curAngVel;  //current angular velocity
	CraftVector _tagAngVel;  //target angular velocity
	CraftVector _velCtrlVal; //control value of velocity loop
public:
	AttitudeControl(Remoter &rc,AHRS &ahrs, Servo &servo);
	
	void UpdateSensor();
	void UpdateServo();
	void PositionLoop();
	void VelocityLoop();

	void SET_PID(u8 kp=0,u8 ki=0,u8 kd=0);

	float CurrentVelocityRoll();
	float CurrentVelocityPitch();
  float CurrentVelocityYaw();
  float CurrentVelocityThrottle();
	float TargetVelocityRoll();
	float TargetVelocityPitch();
  float TargetVelocityYaw();
	float TargetVelocityThrottle();

	float VelCtrlValueRoll();
	float VelCtrlValuePitch();
	float VelCtrlValueYaw();
	float VelCtrlValueThrottle();

	float CurrentAngleRoll();
	float CurrentAnglePitch();
	float CurrentAngleYaw();
	float TargetAngleRoll();
	float TargetAnglePitch();
	float TargetAngleYaw();
};



#endif


