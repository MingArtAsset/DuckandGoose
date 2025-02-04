/*!*************************************************************************
****
\file			Vec4.cpp
\author			Lau Yong Hui
\par DP email:	l.yonghui@digipen.edu
\par Course:	Gam200
\section		A
\date			18-10-2022
\brief			This file contain the Vector3 liabry
				and its corresponding functions for creation and computation

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************/
#include "empch.h"
#include "Vec4.h"

namespace EM
{
	Vec4::Vec4()
	{

	}

	Vec4::Vec4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;

	}

	Vec4 Vec4::operator += (const Vec4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}
	Vec4 Vec4::operator -= (const Vec4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}
	Vec4 Vec4::operator *= (const Vec4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;

		return *this;
	}
	Vec4 Vec4::operator /= (const Vec4& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;

		return *this;
	}
	Vec4 Vec4::operator = (const Vec4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}
	Vec4& Vec4::operator += (float& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		w += rhs;

		return *this;
	}
	Vec4& Vec4::operator -= (float& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		w -= rhs;


		return *this;
	}

	Vec4 Vec4::operator -() const
	{
		return Vec4(-x, -y, -z, -w);
	}

	void Vec4::Vec4Set(Vec4& curr, float x_, float y_, float z_, float w_)
	{
		(void)curr;
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}



	Vec4 operator - (const Vec4& lhs, const Vec4& rhs)
	{

		return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}
	Vec4 operator + (const Vec4& lhs, const Vec4& rhs)
	{
		return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}
	Vec4 operator * (const Vec4& lhs, float rhs)
	{
		return Vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}
	Vec4 operator * (float lhs, const Vec4& rhs)
	{
		return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}

	Vec4 operator / (const Vec4& lhs, float rhs)
	{
		return Vec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
	}
}