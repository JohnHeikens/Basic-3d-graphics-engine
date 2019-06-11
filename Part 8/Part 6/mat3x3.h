#include "vec3.h"
#pragma once
struct mat3x3
{
public:
	//standard: input = output
	mat3x3() :
		XtoX(1), YtoX(0), ZtoX(0),
		XtoY(0), YtoY(1), ZtoY(0),
		XtoZ(0), YtoZ(0), ZtoZ(1)
	{}
	//whatever u want
	mat3x3(
		fp xtox, fp ytox, fp ztox,
		fp xtoy, fp ytoy, fp ztoy,
		fp xtoz, fp ytoz, fp ztoz) :
		XtoX(xtox), YtoX(ytox), ZtoX(ztox),
		XtoY(xtoy), YtoY(ytoy), ZtoY(ztoy),
		XtoZ(xtoz), YtoZ(ytoz), ZtoZ(ztoz)
	{}
	//multiply by the vector
	mat3x3(vec3 mult) :
		XtoX(mult.x), YtoX(0), ZtoX(0),
		XtoY(0), YtoY(mult.y), ZtoY(0),
		XtoZ(0), YtoZ(0), ZtoZ(mult.z)
	{}
	mat3x3 Inverse()
	{
		//https://nl.wikipedia.org/wiki/Inverse_matrix
		//https://www.youtube.com/watch?v=pKZyszzmyeQ
		fp multiplier = 1 / Determinant();
		//N
		return
			mat3x3//reflected=>10>01
		{   //each part of the matrix finds himself 5 times
			XtoX = multiplier * (YtoY * ZtoZ - YtoZ * ZtoY),//tl*br-bl*tr
			XtoY = multiplier * (XtoZ * ZtoY - XtoY * ZtoZ),// reverse value(-value) so bl*tr - tl*br
			XtoZ = multiplier * (XtoY * YtoZ - XtoZ * YtoY),
			YtoX = multiplier * (YtoZ * ZtoX - YtoX * ZtoZ),// reverse value(-value)
			YtoY = multiplier * (XtoX * ZtoZ - XtoZ * ZtoX),
			YtoZ = multiplier * (XtoZ * YtoX - XtoX * YtoZ),// reverse value(-value)
			ZtoX = multiplier * (YtoX * ZtoY - YtoY * ZtoX),
			ZtoY = multiplier * (XtoY * ZtoX - XtoX * ZtoY),// reverse value(-value)
			ZtoZ = multiplier * (XtoX * YtoY - XtoY * YtoX)
		};
	}
	fp Determinant()
	{
		//a = the \ diagonal, b = the / diagonal
		return
			//the \ diagonals
			XtoX * YtoY * ZtoZ
			+ YtoX * ZtoY * XtoZ
			+ ZtoX * XtoY * YtoZ
			//the / diagonals
			- ZtoX * YtoY * XtoZ
			- XtoX * ZtoY * YtoZ
			- YtoX * XtoY * ZtoZ;
	}
	mat3x3 Rotate(vec3 axis, mat3x3 matrix, fp Rotation)
	{
		axis.Normalize();
		fp sinr = (fp)sin(Rotation);
		fp cosr = (fp)cos(Rotation);
		fp mincos = 1 - cosr;
		//https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		mat3x3 rot = mat3x3
		{
			XtoX = cosr + axis.x * axis.x * mincos,
			YtoX = axis.x * axis.y * mincos - axis.z * sinr,
			ZtoX = axis.x * axis.z * mincos + axis.y * sinr,
			XtoY = axis.y * axis.x * mincos + axis.z * sinr,
			YtoY = cosr + axis.y * axis.y * mincos,
			ZtoY = axis.y * axis.z * mincos - axis.x * sinr,
			XtoZ = axis.z * axis.x * mincos - axis.y * sinr,
			YtoZ = axis.z * axis.y * mincos + axis.x * sinr,
			ZtoZ = cosr + axis.z * axis.z * mincos
		};
		matrix = ApplyTo(rot);
		//axis.Normalize();
		//Matrix lhs = axis;
		//Matrix cross = Cross(lhs, matrix);
		//return cos * matrix + sin * cross;
		return matrix;
	}
	void MultiplyX(fp value)
	{
		XtoX *= value;
		YtoX *= value;
		ZtoX *= value;
	}
	void MultiplyY(fp value)
	{
		XtoY *= value;
		YtoY *= value;
		ZtoY *= value;
	}
	void MultiplyZ(fp value)
	{
		XtoZ *= value;
		YtoZ *= value;
		ZtoZ *= value;
	}
	vec3 ApplyTo(vec3 vector)
	{
		return vec3
		(
			XtoX * vector.x + YtoX * vector.y + ZtoX * vector.z,
			XtoY * vector.x + YtoY * vector.y + ZtoY * vector.z,
			XtoZ * vector.x + YtoZ * vector.y + ZtoZ * vector.z
		);
	}
	vec3 ApplyTo(fp x, fp y, fp z)
	{
		return vec3
		{
			x = XtoX * x + YtoX * y + ZtoX * z,
			y = XtoY * x + YtoY * y + ZtoY * z,
			z = XtoZ * x + YtoZ * y + ZtoZ * z,
		};
	}
	mat3x3 ApplyTo(mat3x3 other)
	{
		return mat3x3
		{
			XtoX = other.XtoX * XtoX + other.XtoY * YtoX + other.XtoZ * ZtoX,
			YtoX = other.YtoX * XtoX + other.YtoY * YtoX + other.YtoZ * ZtoX,
			ZtoX = other.ZtoX * XtoX + other.ZtoY * YtoX + other.ZtoZ * ZtoX,
			XtoY = other.XtoX * XtoY + other.XtoY * YtoY + other.XtoZ * ZtoY,
			YtoY = other.YtoX * XtoY + other.YtoY * YtoY + other.YtoZ * ZtoY,
			ZtoY = other.ZtoX * XtoY + other.ZtoY * YtoY + other.ZtoZ * ZtoY,
			XtoZ = other.XtoX * XtoZ + other.XtoY * YtoZ + other.XtoZ * ZtoZ,
			YtoZ = other.YtoX * XtoZ + other.YtoY * YtoZ + other.YtoZ * ZtoZ,
			ZtoZ = other.ZtoX * XtoZ + other.ZtoY * YtoZ + other.ZtoZ * ZtoZ,
		};
	}

	//calculate the cross product of two matrices
	static mat3x3 Cross(mat3x3 lhs, mat3x3 rhs)
	{
		return  mat3x3(
			lhs.XtoX * rhs.XtoX,
			lhs.XtoY * rhs.YtoX,
			lhs.XtoZ * rhs.ZtoX,
			lhs.YtoX * rhs.XtoY,
			lhs.YtoY * rhs.YtoY,
			lhs.YtoZ * rhs.ZtoY,
			lhs.ZtoX * rhs.XtoZ,
			lhs.ZtoY * rhs.YtoZ,
			lhs.ZtoZ * rhs.ZtoZ
		);
	}
	fp XtoX;
	fp YtoX;
	fp ZtoX;
	fp XtoY;
	fp YtoY;
	fp ZtoY;
	fp XtoZ;
	fp YtoZ;
	fp ZtoZ;
};
static mat3x3 operator *(fp mult, mat3x3 matrix);
static mat3x3 operator *(mat3x3 matrix, fp mult);