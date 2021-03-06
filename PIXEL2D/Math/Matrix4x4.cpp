#include "Matrix4x4.h"

namespace PIXL { namespace math {

	Matrix4x4::Matrix4x4()
	{
		for (int i = 0; i < 16; i++)
			elements[i] = 0.0f;
	}

	Matrix4x4::Matrix4x4(float diagonal)
	{
		for (int i = 0; i < 16; i++)
			elements[i] = 0.0f;

		elements[0] = diagonal;
		elements[5] = diagonal;
		elements[10] = diagonal;
		elements[15] = diagonal;
	}

	Matrix4x4 Matrix4x4::Identity()
	{
		return Matrix4x4(1.0f);
	}

	Matrix4x4& Matrix4x4::Multiply(const Matrix4x4& other)
	{
		__declspec(align(16)) float data[16];

		__m128 a_line, b_line, r_line;
		int i, j;
		for (i = 0; i < 16; i += 4) {
			// unroll the first step of the loop to avoid having to initialize r_line to zero
			a_line = _mm_loadu_ps(elements);        
			b_line = _mm_set1_ps(other.elements[i]);
			r_line = _mm_mul_ps(a_line, b_line); 
			for (j = 1; j < 4; j++) {
				a_line = _mm_loadu_ps(&elements[j * 4]);
				b_line = _mm_set1_ps(other.elements[i + j]);
				r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
			}
			_mm_store_ps(&data[i], r_line);
		}

		memcpy(elements, data, 16 * sizeof(float));

		return *this;
	}

	Vector2f Matrix4x4::Multiply(const Vector2f& other) const
	{
		return Vector2f(
			elements[0] * other.x + elements[4] * other.y + elements[8],
			elements[1] * other.x + elements[5] * other.y + elements[9]);
	}

	Vector3f Matrix4x4::Multiply(const Vector3f& other) const
	{
		return Vector3f(
			elements[0] * other.x + elements[4] * other.y + elements[8] * other.z + elements[12],
			elements[1] * other.x + elements[5] * other.y + elements[9] * other.z + elements[13],
			elements[2] * other.x + elements[6] * other.y + elements[10] * other.z + elements[14]
			);
	}

	Vector4f Matrix4x4::Multiply(const Vector4f& other) const
	{
		return Vector4f(
			elements[0] * other.x + elements[4] * other.y + elements[8] * other.z + elements[12] * other.w,
			elements[1] * other.x + elements[5] * other.y + elements[9] * other.z + elements[13] * other.w,
			elements[2] * other.x + elements[6] * other.y + elements[10] * other.z + elements[14] * other.w,
			elements[3] * other.x + elements[7] * other.y + elements[11] * other.z + elements[15] * other.w
			);
	}

	Matrix4x4 operator*(Matrix4x4 left, const Matrix4x4& right)
	{
		return left.Multiply(right);
	}

	Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other)
	{
		return Multiply(other);
	}

	Vector2f operator*(const Matrix4x4& left, const Vector2f& right)
	{
		return left.Multiply(right);
	}

	Vector3f operator*(const Matrix4x4& left, const Vector3f& right)
	{
		return left.Multiply(right);
	}

	Vector4f operator*(const Matrix4x4& left, const Vector4f& right)
	{
		return left.Multiply(right);
	}

	Matrix4x4& Matrix4x4::Inverse()
	{
		double temp[16];

		temp[0] = elements[5] * elements[10] * elements[15] -
			elements[5] * elements[11] * elements[14] -
			elements[9] * elements[6] * elements[15] +
			elements[9] * elements[7] * elements[14] +
			elements[13] * elements[6] * elements[11] -
			elements[13] * elements[7] * elements[10];

		temp[4] = -elements[4] * elements[10] * elements[15] +
			elements[4] * elements[11] * elements[14] +
			elements[8] * elements[6] * elements[15] -
			elements[8] * elements[7] * elements[14] -
			elements[12] * elements[6] * elements[11] +
			elements[12] * elements[7] * elements[10];

		temp[8] = elements[4] * elements[9] * elements[15] -
			elements[4] * elements[11] * elements[13] -
			elements[8] * elements[5] * elements[15] +
			elements[8] * elements[7] * elements[13] +
			elements[12] * elements[5] * elements[11] -
			elements[12] * elements[7] * elements[9];

		temp[12] = -elements[4] * elements[9] * elements[14] +
			elements[4] * elements[10] * elements[13] +
			elements[8] * elements[5] * elements[14] -
			elements[8] * elements[6] * elements[13] -
			elements[12] * elements[5] * elements[10] +
			elements[12] * elements[6] * elements[9];

		temp[1] = -elements[1] * elements[10] * elements[15] +
			elements[1] * elements[11] * elements[14] +
			elements[9] * elements[2] * elements[15] -
			elements[9] * elements[3] * elements[14] -
			elements[13] * elements[2] * elements[11] +
			elements[13] * elements[3] * elements[10];

		temp[5] = elements[0] * elements[10] * elements[15] -
			elements[0] * elements[11] * elements[14] -
			elements[8] * elements[2] * elements[15] +
			elements[8] * elements[3] * elements[14] +
			elements[12] * elements[2] * elements[11] -
			elements[12] * elements[3] * elements[10];

		temp[9] = -elements[0] * elements[9] * elements[15] +
			elements[0] * elements[11] * elements[13] +
			elements[8] * elements[1] * elements[15] -
			elements[8] * elements[3] * elements[13] -
			elements[12] * elements[1] * elements[11] +
			elements[12] * elements[3] * elements[9];

		temp[13] = elements[0] * elements[9] * elements[14] -
			elements[0] * elements[10] * elements[13] -
			elements[8] * elements[1] * elements[14] +
			elements[8] * elements[2] * elements[13] +
			elements[12] * elements[1] * elements[10] -
			elements[12] * elements[2] * elements[9];

		temp[2] = elements[1] * elements[6] * elements[15] -
			elements[1] * elements[7] * elements[14] -
			elements[5] * elements[2] * elements[15] +
			elements[5] * elements[3] * elements[14] +
			elements[13] * elements[2] * elements[7] -
			elements[13] * elements[3] * elements[6];

		temp[6] = -elements[0] * elements[6] * elements[15] +
			elements[0] * elements[7] * elements[14] +
			elements[4] * elements[2] * elements[15] -
			elements[4] * elements[3] * elements[14] -
			elements[12] * elements[2] * elements[7] +
			elements[12] * elements[3] * elements[6];

		temp[10] = elements[0] * elements[5] * elements[15] -
			elements[0] * elements[7] * elements[13] -
			elements[4] * elements[1] * elements[15] +
			elements[4] * elements[3] * elements[13] +
			elements[12] * elements[1] * elements[7] -
			elements[12] * elements[3] * elements[5];

		temp[14] = -elements[0] * elements[5] * elements[14] +
			elements[0] * elements[6] * elements[13] +
			elements[4] * elements[1] * elements[14] -
			elements[4] * elements[2] * elements[13] -
			elements[12] * elements[1] * elements[6] +
			elements[12] * elements[2] * elements[5];

		temp[3] = -elements[1] * elements[6] * elements[11] +
			elements[1] * elements[7] * elements[10] +
			elements[5] * elements[2] * elements[11] -
			elements[5] * elements[3] * elements[10] -
			elements[9] * elements[2] * elements[7] +
			elements[9] * elements[3] * elements[6];

		temp[7] = elements[0] * elements[6] * elements[11] -
			elements[0] * elements[7] * elements[10] -
			elements[4] * elements[2] * elements[11] +
			elements[4] * elements[3] * elements[10] +
			elements[8] * elements[2] * elements[7] -
			elements[8] * elements[3] * elements[6];

		temp[11] = -elements[0] * elements[5] * elements[11] +
			elements[0] * elements[7] * elements[9] +
			elements[4] * elements[1] * elements[11] -
			elements[4] * elements[3] * elements[9] -
			elements[8] * elements[1] * elements[7] +
			elements[8] * elements[3] * elements[5];

		temp[15] = elements[0] * elements[5] * elements[10] -
			elements[0] * elements[6] * elements[9] -
			elements[4] * elements[1] * elements[10] +
			elements[4] * elements[2] * elements[9] +
			elements[8] * elements[1] * elements[6] -
			elements[8] * elements[2] * elements[5];

		double determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
		determinant = 1.0 / determinant;

		for (int i = 0; i < 16; i++)
			elements[i] = (float)(temp[i] * determinant);

		return *this;
	}

	Matrix4x4 Matrix4x4::Orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		Matrix4x4 result(1.0f);

		result.elements[0] = 2.0f / (right - left);
		result.elements[5] = 2.0f / (top - bottom);
		result.elements[10] = 2.0f / (near - far);
		result.elements[12] = (left + right) / (left - right);
		result.elements[13] = (bottom + top) / (bottom - top);
		result.elements[14] = (far + near) / (near - far);

		return result;
	}

	Matrix4x4 Matrix4x4::Perspective(float fov, float aspectRatio, float near, float far)
	{
		Matrix4x4 result(1.0f);

		float q = 1.0f / tan(ToRadians(0.5f * fov));
		float a = q / aspectRatio;

		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);

		result.elements[0] = a;
		result.elements[5] = q;
		result.elements[10] = b;
		result.elements[11] = -1.0f;
		result.elements[14] = c;

		return result;
	}

	Matrix4x4 Matrix4x4::Translation(const Vector2f& translation)
	{
		Matrix4x4 result(1.0f);

		result.elements[12] = translation.x;
		result.elements[13] = translation.y;

		return result;
	}

	void Matrix4x4::Translate(const Vector2f& translation)
	{
		elements[12] = translation.x;
		elements[13] = translation.y;
	}

	PIXL::math::Matrix4x4 Matrix4x4::Rotation(float angle)
	{
		Matrix4x4 result(1.0f);

		float r = ToRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = 0;
		float y = 0;
		float z = 1.0f;

		result.elements[0] = x * omc + c;
		result.elements[1] = y * x * omc + z * s;
		result.elements[24] = x * z * omc - y * s;

		result.elements[4] = x * y * omc - z * s;
		result.elements[5] = y * omc + c;
		result.elements[6] = y * z * omc + x * s;

		result.elements[8] = x * z * omc + y * s;
		result.elements[9] = y * z * omc - x * s;
		result.elements[10] = z * omc + c;

		return result;
	}

	void Matrix4x4::Rotate(float angle)
	{
		float r = ToRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = 0;
		float y = 0;
		float z = 1.0f;

		elements[0] = x * omc + c;
		elements[1] = y * x * omc + z * s;
		elements[2] = x * z * omc - y * s;

		elements[4] = x * y * omc - z * s;
		elements[5] = y * omc + c;
		elements[6] = y * z * omc + x * s;

		elements[8] = x * z * omc + y * s;
		elements[9] = y * z * omc - x * s;
		elements[10] = z * omc + c;
	}

	Matrix4x4 Matrix4x4::Scaled(const Vector2f& scale)
	{
		Matrix4x4 result(1.0f);

		result.elements[0] = scale.x;
		result.elements[5] = scale.y;

		return result;
	}

	void Matrix4x4::Scale(const Vector2f& scale)
	{
		elements[0] = scale.x;
		elements[5] = scale.y;
	}

	Matrix4x4& Matrix4x4::Transpose()
	{
		for (int i = 0; i < 16; i++) {
			for (int j = i + 1; j < 16; j++) {
				std::swap(elements[i + j], elements[j + i]);
			}
		}

		return *this;
	}
} }