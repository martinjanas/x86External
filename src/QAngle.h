#pragma once
class QAngle //I have onlyaw slept for like 4 hours lol pitchD ...
{
public:

	QAngle();
	QAngle(float pitch, float yaw, float roll);
	float Length() const;
	float DistTo(const QAngle& vOther) const;
	float GetMaxPitch();
	float GetMinPitch();
	float GetMaxYaw();
	float GetMinYaw();
	bool Clamped();

	QAngle& operator=(const QAngle& vOther)
	{
		pitch = vOther.pitch; yaw = vOther.yaw; roll = vOther.roll;
		return *this;
	}

	QAngle operator-(void) const
	{
		return QAngle(-pitch, -yaw, -roll);
	}
	QAngle operator+(const QAngle& v) const
	{
		return QAngle(pitch + v.pitch, yaw + v.yaw, roll + v.roll);
	}
	QAngle operator+(const float& v) const
	{
		return QAngle(pitch + v, yaw + v, roll + v);
	}
	QAngle operator-(const QAngle& v) const
	{
		return QAngle(pitch - v.pitch, yaw - v.yaw, roll - v.roll);
	}
	QAngle operator-(const float& v) const
	{
		return QAngle(pitch - v, yaw - v, roll - v);
	}
	QAngle operator*(float fl) const
	{
		return QAngle(pitch * fl, yaw * fl, roll * fl);
	}
	QAngle operator*(const QAngle& v) const
	{
		return QAngle(pitch * v.pitch, yaw * v.yaw, roll * v.roll);
	}
	QAngle operator/(float fl) const
	{
		return QAngle(pitch / fl, yaw / fl, roll / fl);
	}
	QAngle operator/(const QAngle& v) const
	{
		return QAngle(pitch / v.pitch, yaw / v.yaw, roll / v.roll);
	}

	float pitch, yaw, roll;
};

