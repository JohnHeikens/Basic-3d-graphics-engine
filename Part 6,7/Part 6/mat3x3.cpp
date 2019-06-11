#include "mat3x3.h"
mat3x3 operator *(fp mult, mat3x3 matrix)
{
	return mat3x3
	(
		matrix.XtoX * mult,
		matrix.YtoX * mult,
		matrix.ZtoX * mult,
		matrix.XtoY * mult,
		matrix.YtoY * mult,
		matrix.ZtoY * mult,
		matrix.XtoZ * mult,
		matrix.YtoZ * mult,
		matrix.ZtoZ * mult
	);
}
mat3x3 operator *(mat3x3 matrix, fp mult)
{
	return mat3x3
	(
		matrix.XtoX * mult,
		matrix.YtoX * mult,
		matrix.ZtoX * mult,
		matrix.XtoY * mult,
		matrix.YtoY * mult,
		matrix.ZtoY * mult,
		matrix.XtoZ * mult,
		matrix.YtoZ * mult,
		matrix.ZtoZ * mult
	);
}