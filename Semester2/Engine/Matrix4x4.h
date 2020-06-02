#pragma once
#include "Point4D.h"
#include <math.h>
#include <assert.h>

#define Pi 3.14159265359

class Matrix4x4
{
public:
	Matrix4x4() 
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrix[i][j] = 0.0f;			
			}
		}
	}

	~Matrix4x4()
	{

	}

	Matrix4x4(Point4D row1, Point4D row2, Point4D row3, Point4D row4)
	{
		matrix[0] = row1;
		matrix[1] = row2;
		matrix[2] = row3;
		matrix[3] = row4;
	}

	inline const Matrix4x4 operator* (Matrix4x4 & other)
	{
		Matrix4x4 tempMatrix;
		float tempVal;
		for (int k = 0; k < 4; k++)
		{
			for (int i = 0; i < 4; i++)
			{
				tempVal = 0.0f;
				for (int j = 0; j < 4; j++)
				{
					tempVal += matrix[i][j] * other[j][k];
				}
				tempMatrix[i][k] = tempVal;
			}
		}
		return tempMatrix;
	}

	inline const Matrix4x4 operator+ (Matrix4x4 & other)
	{
		Matrix4x4 tempMatrix;
		for (int i = 0; i < 4; i++)
		{
				for (int j = 0; j < 4; j++)
				{
					tempMatrix[i][j] =  matrix[i][j] + other[i][j];
				}
		}
		return tempMatrix;
	}

	inline const Matrix4x4 operator- (Matrix4x4 & other)
	{
		Matrix4x4 tempMatrix;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				tempMatrix[i][j] = matrix[i][j] - other[i][j];
			}
		}
		return tempMatrix;
	}

Point4D operator* (Point4D & other)
	{
		Point4D tempPoint4D;
		float tempVal;

			for (int i = 0; i < 4; i++)
			{
				tempVal = 0.0f;
				for (int j = 0; j < 4; j++)
				{
					tempVal += matrix[i][j] * other[j];
				}
				tempPoint4D[i] = tempVal;
			}
		return tempPoint4D;
	}

	inline Point4D& operator[](int i_index)
	{
		assert(i_index < 4);
		assert(i_index >= 0);
		return matrix[i_index];
	}

	inline const Matrix4x4 Transpose()
	{
		Matrix4x4 tempMatrix;
			for (int i = 0; i < 4; i++)
			{				
				for (int j = 0; j < 4; j++)
				{
					tempMatrix[j][i] = matrix[i][j];
				}			
			}
		return tempMatrix;
	}

	inline const float Determinant()
	{
		return matrix[0][0] * ((matrix[1][1] * matrix[2][2] * matrix[3][3]) + (matrix[1][2] * matrix[2][3] * matrix[3][1]) + (matrix[1][3] * matrix[2][1] * matrix[3][2]) - (matrix[1][3] * matrix[2][2] * matrix[3][1]) - (matrix[1][2] * matrix[2][1] * matrix[3][3]) - (matrix[1][1] * matrix[2][3] * matrix[3][2]))
			- matrix[1][0] * ((matrix[0][1] * matrix[2][2] * matrix[3][3]) + (matrix[0][2] * matrix[2][3] * matrix[3][1]) + (matrix[0][3] * matrix[2][1] * matrix[3][2]) - (matrix[0][3] * matrix[2][2] * matrix[3][1]) - (matrix[0][2] * matrix[2][1] * matrix[3][3]) - (matrix[0][1] * matrix[2][3] * matrix[3][2]))
			+ matrix[2][0] * ((matrix[0][1] * matrix[1][2] * matrix[3][3]) + (matrix[0][2] * matrix[1][3] * matrix[3][1]) + (matrix[0][3] * matrix[1][1] * matrix[3][2]) - (matrix[0][3] * matrix[1][2] * matrix[3][1]) - (matrix[0][2] * matrix[1][1] * matrix[3][3]) - (matrix[0][1] * matrix[1][3] * matrix[3][2]))
			- matrix[3][0] * ((matrix[0][1] * matrix[1][2] * matrix[2][3]) + (matrix[0][2] * matrix[1][3] * matrix[2][1]) + (matrix[0][3] * matrix[1][1] * matrix[2][2]) - (matrix[0][3] * matrix[1][2] * matrix[2][1]) - (matrix[0][2] * matrix[1][1] * matrix[2][3]) - (matrix[0][1] * matrix[1][3] * matrix[2][2]));
	}

	inline const Matrix4x4 Adjugate()
	{
		Matrix4x4 tempMatrix;

		tempMatrix[0][0] = ((matrix[1][1] * matrix[2][2] * matrix[3][3]) + (matrix[1][2] * matrix[2][3] * matrix[3][1]) + (matrix[1][3] * matrix[2][1] * matrix[3][2])
			- (matrix[1][3] * matrix[2][2] * matrix[3][1]) - (matrix[1][2] * matrix[2][1] * matrix[3][3]) - (matrix[1][1] * matrix[2][3] * matrix[3][2]));

		tempMatrix[0][1] = (-(matrix[0][1] * matrix[2][2] * matrix[3][3]) - (matrix[0][2] * matrix[2][3] * matrix[3][1]) - (matrix[0][3] * matrix[2][1] * matrix[3][2])
			+ (matrix[0][3] * matrix[2][2] * matrix[3][1]) + (matrix[0][2] * matrix[2][1] * matrix[3][3]) + (matrix[0][1] * matrix[2][3] * matrix[3][2]));

		tempMatrix[0][2] = ((matrix[0][1] * matrix[1][2] * matrix[3][3]) + (matrix[0][2] * matrix[1][3] * matrix[3][1]) + (matrix[0][3] * matrix[1][1] * matrix[3][2])
			- (matrix[0][3] * matrix[1][2] * matrix[3][1]) - (matrix[0][2] * matrix[1][1] * matrix[3][3]) - (matrix[0][1] * matrix[1][3] * matrix[3][2]));

		tempMatrix[0][3] = (-(matrix[0][1] * matrix[1][2] * matrix[2][3]) - (matrix[0][2] * matrix[1][3] * matrix[2][1]) - (matrix[0][3] * matrix[1][1] * matrix[2][2])
			+ (matrix[0][3] * matrix[1][2] * matrix[2][1]) + (matrix[0][2] * matrix[1][1] * matrix[2][3]) + (matrix[0][1] * matrix[1][3] * matrix[2][2]));



		tempMatrix[1][0] = (-(matrix[1][0] * matrix[2][2] * matrix[3][3]) - (matrix[1][2] * matrix[2][3] * matrix[3][0]) - (matrix[1][3] * matrix[2][0] * matrix[3][2])
			+ (matrix[1][3] * matrix[2][2] * matrix[3][0]) + (matrix[1][2] * matrix[2][0] * matrix[3][3]) + (matrix[1][0] * matrix[2][3] * matrix[3][2]));

		tempMatrix[1][1] = ((matrix[0][0] * matrix[2][2] * matrix[3][3]) + (matrix[0][2] * matrix[2][3] * matrix[3][0]) + (matrix[0][3] * matrix[2][0] * matrix[3][2])
			- (matrix[0][3] * matrix[2][2] * matrix[3][0]) - (matrix[0][2] * matrix[2][0] * matrix[3][3]) - (matrix[0][0] * matrix[2][3] * matrix[3][2]));

		tempMatrix[1][2] = (-(matrix[0][0] * matrix[1][2] * matrix[3][3]) - (matrix[0][2] * matrix[1][3] * matrix[3][0]) - (matrix[0][3] * matrix[1][0] * matrix[3][2])
			+ (matrix[0][3] * matrix[1][2] * matrix[3][0]) + (matrix[0][2] * matrix[1][0] * matrix[3][3]) + (matrix[0][0] * matrix[1][3] * matrix[3][2]));

		tempMatrix[1][3] = ((matrix[0][0] * matrix[1][2] * matrix[2][3]) + (matrix[0][2] * matrix[1][3] * matrix[2][0]) + (matrix[0][3] * matrix[1][0] * matrix[2][2])
			- (matrix[0][3] * matrix[1][2] * matrix[2][0]) - (matrix[0][2] * matrix[1][0] * matrix[2][3]) - (matrix[0][0] * matrix[1][3] * matrix[2][2]));


		tempMatrix[2][0] = ((matrix[1][0] * matrix[2][1] * matrix[3][3]) + (matrix[1][1] * matrix[2][3] * matrix[3][0]) + (matrix[1][3] * matrix[2][0] * matrix[3][1])
			- (matrix[1][3] * matrix[2][2] * matrix[3][1]) - (matrix[1][2] * matrix[2][1] * matrix[3][3]) - (matrix[1][1] * matrix[2][3] * matrix[3][1]));

		tempMatrix[2][1] = (-(matrix[0][0] * matrix[2][1] * matrix[3][3]) - (matrix[0][1] * matrix[2][3] * matrix[3][0]) - (matrix[0][3] * matrix[2][0] * matrix[3][1])
			+ (matrix[0][3] * matrix[2][1] * matrix[3][0]) + (matrix[0][1] * matrix[2][0] * matrix[3][3]) + (matrix[0][0] * matrix[2][3] * matrix[3][1]));

		tempMatrix[2][2] = ((matrix[0][0] * matrix[1][1] * matrix[3][3]) + (matrix[0][1] * matrix[1][3] * matrix[3][0]) + (matrix[0][3] * matrix[1][0] * matrix[3][1])
			- (matrix[0][3] * matrix[1][1] * matrix[3][0]) - (matrix[0][1] * matrix[1][0] * matrix[3][3]) - (matrix[0][0] * matrix[1][3] * matrix[3][1]));

		tempMatrix[2][3] = (-(matrix[0][0] * matrix[1][1] * matrix[2][3]) - (matrix[0][1] * matrix[1][3] * matrix[2][0]) - (matrix[0][3] * matrix[1][0] * matrix[2][1])
			+ (matrix[0][3] * matrix[1][1] * matrix[2][0]) + (matrix[0][1] * matrix[1][0] * matrix[2][3]) + (matrix[0][0] * matrix[1][3] * matrix[2][1]));


		tempMatrix[3][0] = (-(matrix[1][0] * matrix[2][1] * matrix[3][2]) - (matrix[1][1] * matrix[2][2] * matrix[3][0]) - (matrix[1][2] * matrix[2][0] * matrix[3][1])
			+ (matrix[1][2] * matrix[2][1] * matrix[3][0]) + (matrix[1][1] * matrix[2][0] * matrix[3][2]) + (matrix[1][0] * matrix[2][2] * matrix[3][1]));

		tempMatrix[3][1] = ((matrix[0][0] * matrix[2][1] * matrix[3][2]) + (matrix[0][1] * matrix[2][2] * matrix[3][0]) + (matrix[0][2] * matrix[2][0] * matrix[3][1])
			- (matrix[0][2] * matrix[2][1] * matrix[3][0]) - (matrix[0][1] * matrix[2][0] * matrix[3][2]) - (matrix[0][0] * matrix[2][2] * matrix[3][1]));

		tempMatrix[3][2] = (-(matrix[0][0] * matrix[1][1] * matrix[3][2]) - (matrix[0][1] * matrix[1][2] * matrix[3][0]) - (matrix[0][2] * matrix[1][0] * matrix[3][1])
			+ (matrix[0][2] * matrix[1][1] * matrix[3][0]) + (matrix[0][1] * matrix[1][0] * matrix[3][2]) + (matrix[0][0] * matrix[1][2] * matrix[3][1]));

		tempMatrix[3][3] = ((matrix[0][0] * matrix[1][1] * matrix[2][2]) + (matrix[0][1] * matrix[1][2] * matrix[2][0]) + (matrix[0][2] * matrix[1][0] * matrix[2][1])
			- (matrix[0][2] * matrix[1][1] * matrix[2][0]) - (matrix[0][1] * matrix[1][0] * matrix[2][2]) - (matrix[0][0] * matrix[1][2] * matrix[2][1]));

		return tempMatrix;
	}

	inline const Matrix4x4 Inverse()
	{
		Matrix4x4 temp = Matrix4x4(matrix[0], matrix[1], matrix[2], matrix[3]);

		float tempfloat = 0.0f;

		assert(temp.Determinant() != 0.0f);

		tempfloat = 1.0f / temp.Determinant();
		

		temp = temp.Adjugate();

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				temp[i][j] = temp[i][j]*tempfloat;
			}
		}	
		return temp;
	}

	static Matrix4x4 ZAxisRotationTransform(float angle)
	{
		return Matrix4x4(Point4D(cos(angle / Pi), -sin(angle / Pi), 0.0f, 0.0f), Point4D(sin(angle / Pi), cos(angle / Pi), 0.0f, 0.0f), Point4D(0.0f, 0.0f, 1.0f, 0.0f), Point4D(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix4x4 YAxisRotationTransform(float angle)
	{
		
		return Matrix4x4(Point4D(cos(angle / Pi), 0.0f, sin(angle / Pi), 0.0f), Point4D(0.0f, 1.0f, 0.0f, 0.0f), Point4D(-sin(angle / Pi), 0.0f, cos(angle / Pi), 0.0f), Point4D(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix4x4 XAxisRotationTransform(float angle)
	{
		return Matrix4x4(Point4D(1.0f, 0.0f, 0.0f, 0.0f), Point4D(0.0f, cos(angle / Pi), -sin(angle / Pi), 0.0f), Point4D(0.0f, sin(angle / Pi), cos(angle / Pi), 0.0f), Point4D(0.0f, 0.0f, 0.0f, 1.0f));
	}

	static Matrix4x4 TranslationTransform(float x, float y, float z)
	{
		return Matrix4x4(Point4D(1.0f,0.0f,0.0f,x), Point4D(0.0f,1.0f,0.0f,y), Point4D(0.0f,0.0f,1,z), Point4D(0.0f,0.0f,0.0f,1.0f));
	}

	static Matrix4x4 ScalingTransform(float x, float y, float z)
	{
		return Matrix4x4(Point4D(x, 0.0f, 0.0f, 0.0f), Point4D(0.0f, y, 0.0f, 0.0f), Point4D(0.0f, 0.0f, z, 0.0f), Point4D(0.0f, 0.0f, 0.0f, 1.0f));
	}

private:
	Point4D matrix[4];
};

