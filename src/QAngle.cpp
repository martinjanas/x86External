#include "QAngle.h"
#include <math.h>

QAngle::QAngle()
{
	this->pitch = this->yaw = this->roll = 0.f;
}

QAngle::QAngle(float x, float y, float z)
{
	this->pitch = x;
	this->yaw = y;
	this->roll = z;
}

float QAngle::Length() const
{
	return sqrt(pitch * pitch + yaw * yaw + roll * roll);
}

float QAngle::DistTo(const QAngle& vOther) const
{
	QAngle delta;

	delta.pitch = pitch - vOther.pitch;
	delta.yaw = yaw - vOther.yaw;
	delta.roll = roll - vOther.roll;

	return delta.Length();
}

float QAngle::GetMaxPitch()
{
    return 89.0f;
}

float QAngle::GetMinPitch()
{
    return -89.0f;
}

float QAngle::GetMaxYaw()
{
    return 180.0f;
}

float QAngle::GetMinYaw()
{
    return -180.0f;
}

bool QAngle::Clamped()
{
    QAngle angle = *this;

    if (angle.pitch > GetMaxPitch())
        angle.pitch = 89.0f;
    else if (angle.pitch < GetMinPitch())
        angle.pitch = -89.0f;

    if (angle.yaw > GetMaxYaw())
        angle.yaw = 180.0f;
    else if (angle.yaw < GetMinYaw())
        angle.yaw = -180.0f;

    angle.roll = 0.f;

    *this = angle;

    return pitch >= -89.f && pitch <= 89.f && yaw >= -180.f && yaw <= 180.f && roll == 0.f;
}
