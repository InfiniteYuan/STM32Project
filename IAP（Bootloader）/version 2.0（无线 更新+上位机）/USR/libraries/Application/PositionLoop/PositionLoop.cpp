
#include "PositionLoop.h"

PositionLoop::PositionLoop(const CraftVector &curAngle, const CraftVector &tagAngle, CraftVector &tagAngVel)
	:_curAngle(curAngle),_tagAngle(tagAngle),_tagAngVel(tagAngVel)
{
	//       Kp    Ki   Kd    dt     
	pidPitch(3.5,    0.0,  0.1, 0.002,  15,  10);
}

void PositionLoop::Roll()
{
//	_tagAngVel.roll = pidRoll.ComputePID(_tagAngle.roll - _curAngle.roll);
}

void PositionLoop::Pitch()
{
//	_tagAngVel.pitch = pidPitch.ComputePID(_tagAngle.pitch , _curAngle.pitch);
}

void PositionLoop::Yaw()
{
//	_tagAngVel.yaw = pidYaw.ComputePID(_tagAngle.yaw - _curAngle.yaw);
}

void PositionLoop::Throttle()
{
	
}

