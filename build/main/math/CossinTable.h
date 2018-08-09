#ifndef COSSIN_TABLE_H
#define COSSIN_TABLE_H

#include <cassert>
#include <math.h>

#define MAX_CIRCLE_ANGLE      512
#define HALF_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/2)
#define QUARTER_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE/4)
#define MASK_MAX_CIRCLE_ANGLE (MAX_CIRCLE_ANGLE - 1)
#define PI 3.14159265358979323846

class CossinTable
{
	private:
		static CossinTable* mTableInstance;
		float fast_cossin_table[MAX_CIRCLE_ANGLE]; // Declare table of fast cosinus and sinus
	public:
		
										   //Static class functions
		static CossinTable* getInstance()
		{
			assert(mTableInstance != NULL);
			return mTableInstance;
		}


		static void initInstance()
		{
			mTableInstance = new CossinTable;
		}


		static void cleanupInstance()
		{
			if (mTableInstance != NULL)
			{
				delete mTableInstance;
				mTableInstance = NULL;
			}
		}

		CossinTable();
		~CossinTable();

		inline void FloatToInt(int *int_pointer, float f);
		float fastcos(float n);
		float fastsin(float n);
};
													
#endif // !COSSIN_TABLE_H