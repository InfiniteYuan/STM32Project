#include "VelocityLoop.h"


VelocityLoop::VelocityLoop(const CraftVector &curAngVel, const CraftVector &tagAngVel, CraftVector &velCtrlVal)
	:_curAngVel(curAngVel),_tagAngVel(tagAngVel),_velCtrlVal(velCtrlVal)
{
	//       Kp    Ki   Kd    dt     
	pidPitch(4,    0.0,  0.1, 0.002,  15,  10);
}

void VelocityLoop::Roll()
{
//	_velCtrlVal.roll = _tagAngVel.roll - _curAngVel.roll;
}

void VelocityLoop::Pitch()
{
	_velCtrlVal.pitch = pidPitch.ComputePID(_tagAngVel.pitch, _curAngVel.pitch);
}

void VelocityLoop::Yaw()
{
//	_velCtrlVal.yaw = _tagAngVel.yaw - _curAngVel.yaw;
}

void VelocityLoop::Throttle()
{
	
}

void VelocityLoop::Set_PIT_PID(float kp, float ki, float kd)
{
	pidPitch.Set_PID(kp, ki, kd);
}


