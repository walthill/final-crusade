#include "CossinTable.h"

// http://www.flipcode.com/archives/Fast_Trigonometry_Functions_Using_Lookup_Tables.shtml

CossinTable* CossinTable::mTableInstance = NULL;

CossinTable::CossinTable()
{
	// Build cossin table
	for (int i = 0; i < MAX_CIRCLE_ANGLE; i++)
	{
		fast_cossin_table[i] = (float)sin((double)i * PI / HALF_MAX_CIRCLE_ANGLE);
	}
}


CossinTable::~CossinTable()
{
}

// Copied from NVidia web site
void CossinTable::FloatToInt(int *int_pointer, float f)
{
	__asm  fld  f
	__asm  mov  edx, int_pointer
	__asm  FRNDINT
	__asm  fistp dword ptr[edx];

}

float CossinTable::fastcos(float n)
{
	float f = n * HALF_MAX_CIRCLE_ANGLE / PI;
	int i;
	FloatToInt(&i, f);
	if (i < 0)
	{
		return fast_cossin_table[((-i) + QUARTER_MAX_CIRCLE_ANGLE)&MASK_MAX_CIRCLE_ANGLE];
	}
	else
	{
		return fast_cossin_table[(i + QUARTER_MAX_CIRCLE_ANGLE)&MASK_MAX_CIRCLE_ANGLE];
	}

	assert(0);
}

float CossinTable::fastsin(float n)
{
	float f = n * HALF_MAX_CIRCLE_ANGLE / PI;
	int i;
	FloatToInt(&i, f);
	if (i < 0)
	{
		return fast_cossin_table[(-((-i)&MASK_MAX_CIRCLE_ANGLE)) + MAX_CIRCLE_ANGLE];
	}
	else
	{
		return fast_cossin_table[i&MASK_MAX_CIRCLE_ANGLE];
	}

	assert(0);
}