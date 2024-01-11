#pragma once

#include <cassert>
#include <iostream>

namespace framework
{
	struct Vector2D
	{
		/// properties
		float x;
		float y;


		/// constructors / destructor

		Vector2D(float x = 0.f, float y = 0.f) : x(x), y(y) { }

		Vector2D(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) { }

		Vector2D(const Vector2D& vec) = default;


		/// operator functions

		Vector2D& operator = (const Vector2D& vec) = default;

		bool operator == (const Vector2D& vec) const { return (this->x == vec.x && this->y == vec.y); }
		bool operator != (const Vector2D& vec) const { return (this->x != vec.x || this->y != vec.y); }

		Vector2D operator + (float value) const { return { this->x + value, this->y + value }; }
		Vector2D operator - (float value) const { return { this->x - value, this->y - value }; }
		Vector2D operator * (float value) const { return { this->x * value, this->y * value }; }
		Vector2D operator / (float value) const
		{
			assert(value != 0.f);
			return { this->x / value, this->y / value };
		}

		Vector2D operator + (const Vector2D& vec) const { return {this->x + vec.x, this->y + vec.y}; }
		Vector2D operator - (const Vector2D& vec) const { return {this->x - vec.x, this->y - vec.y}; }
		Vector2D operator * (const Vector2D& vec) const { return {this->x * vec.x, this->y * vec.y}; }
		Vector2D operator / (const Vector2D& vec) const
		{
			assert(vec.x != 0.f && vec.y != 0.f);
			return {this->x / vec.x, this->y / vec.y};
		}

		Vector2D operator += (const Vector2D& vec)
		{
			this->x += vec.x;
			this->y += vec.y;

			return *this;
		}

		Vector2D operator += (const float value)
		{
			this->x += value;
			this->y += value;

			return *this;
		}

		Vector2D& operator -= (const Vector2D& vec)
		{
			this->x -= vec.x;
			this->y -= vec.y;

			return *this;
		}

		Vector2D operator -= (const float value)
		{
			this->x -= value;
			this->y -= value;

			return *this;
		}

		Vector2D& operator *= (const Vector2D& vec)
		{
			this->x *= vec.x;
			this->y *= vec.y;

			return *this;
		}

		Vector2D& operator *= (const float value)
		{
			this->x *= value;
			this->y *= value;

			return *this;
		}

		Vector2D& operator /= (const Vector2D& vec)
		{
			this->x /= vec.x;
			this->y /= vec.y;

			return *this;
		}

		Vector2D& operator /= (const float value)
		{
			this->x /= value;
			this->y /= value;

			return *this;
		}

		/// member functions

		float Length() const { return static_cast<float>(sqrt(x * x + y * y)); }
		float LengthSqrt() const { return x * x + y * y; }

		Vector2D& Normalize()
		{
			const float length = Length();
			if (length == 0)
				return *this;

			this->x /= length; // this->x == rawX
			this->y /= length; // this->y == rawY

			return *this;		 // 원본이 그대로 반환
		}

		Vector2D GetNormalize() const
		{
			const float length = Length();

			if (length == 0)
				return *this;

			return { this->x / length, this->y / length };
		}

		Vector2D& LimitX(float value)
		{
			if (this->x >= value)
				this->x = value;
			else if (this->x < -value)
				this->x = -value;

			return *this;
		}
		Vector2D& LimitY(float value)
		{
			if (this->y >= value)
				this->y = value;
			else if (this->y < -value)
				this->y = -value;

			return *this;
		}
		Vector2D& Limit(float value)
		{
			const float length = this->Length();

			if(length >= value)
			{
				// 벡터의 길이가 value를 넘지 못하도록 한다
				*this = (*this / length) * value;
			}

			return *this;
		}

		/// static functions

		static Vector2D Zero() { return { 0.f, 0.f }; }
		static Vector2D One() { return { 1.f, 1.f }; }
		static Vector2D Up() { return { 0.f, -1.f }; }
		static Vector2D Down() { return { 0.f, 1.f }; }
		static Vector2D Left() { return { -1.f, 0.f }; }
		static Vector2D Right() { return { 1.f, 0.f }; }

		// TODO : 일반적으로 곱셈은 값의 제곱근을 취하는 것보다 훨씬 저렴한 연산이므로 수정하자
		static float Distance(const Vector2D& vec1, const Vector2D& vec2)
		{
			return static_cast<float>(sqrt
			(
				static_cast<float>(pow(vec1.x - vec2.x, 2))
				+ static_cast<float>(pow(vec1.y - vec2.y, 2))
			));
		}
		static float DotProduct(const Vector2D& vec1, const Vector2D& vec2) { return vec1.x * vec2.x + vec1.y * vec2.y; }
	};

	inline std::ostream& operator << (std::ostream& os, const Vector2D& vec)
	{
		os << "X : " << vec.x << " Y : " << vec.y << std::endl;
		return os;
	}

	/*---------------------------------------
	애니메이션 프레임 정보
	---------------------------------------*/

	struct FRAME_INFO
	{
		D2D1_RECT_F Source;		// 이미지 파일에서 어느 영역 Rect가 실제 프레임인지 저장한다.
		float		RenderTime; // 그리는 시간

		FRAME_INFO(int x, int y, int width, int height, float time)
		{
			Source.left = (float)x;
			Source.top = (float)y;
			Source.right = (float)x + (float)width;
			Source.bottom = (float)y + (float)height;
			RenderTime = time;
		}
		FRAME_INFO(D2D1_RECT_F rect, float time)
		{
			Source = rect;
			RenderTime = time;
		}
	};


	/*----------------------------------------------------
	EventManager에 사용되는 구조체
	----------------------------------------------------*/
	struct EVENT_ANIMATION_INFO
	{
		std::wstring animationName;
		bool flip;
	};

	struct EVENT_MOVEMENT_INFO
	{
		Vector2D moveDirection;
		Vector2D lookDirection;
	};
}
