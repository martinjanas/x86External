#include "QAngle.h"
#include <math.h>

QAngle::QAngle()
{
	this->x = this->y = this->z = 0.f;
}

QAngle::QAngle(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float QAngle::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

float QAngle::DistTo(const QAngle& vOther) const
{
	QAngle delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.Length();
}
