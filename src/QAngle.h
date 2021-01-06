#pragma once
class QAngle //I have only slept for like 4 hours lol xD ...
{
public:

	QAngle();
	QAngle(float x, float y, float z);
	float Length() const;
	float DistTo(const QAngle& vOther) const;

	QAngle& operator=(const QAngle& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	QAngle operator-(void) const
	{
		return QAngle(-x, -y, -z);
	}
	QAngle operator+(const QAngle& v) const
	{
		return QAngle(x + v.x, y + v.y, z + v.z);
	}
	QAngle operator+(const float& v) const
	{
		return QAngle(x + v, y + v, z + v);
	}
	QAngle operator-(const QAngle& v) const
	{
		return QAngle(x - v.x, y - v.y, z - v.z);
	}
	QAngle operator-(const float& v) const
	{
		return QAngle(x - v, y - v, z - v);
	}
	QAngle operator*(float fl) const
	{
		return QAngle(x * fl, y * fl, z * fl);
	}
	QAngle operator*(const QAngle& v) const
	{
		return QAngle(x * v.x, y * v.y, z * v.z);
	}
	QAngle operator/(float fl) const
	{
		return QAngle(x / fl, y / fl, z / fl);
	}
	QAngle operator/(const QAngle& v) const
	{
		return QAngle(x / v.x, y / v.y, z / v.z);
	}

	float x, y, z;
};

