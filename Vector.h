#pragma once

#include <cmath>

struct Vector2D
{
	constexpr Vector2D& operator-=(const Vector2D& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	constexpr auto operator+(const Vector2D& v) const noexcept
	{
		return Vector2D{ x + v.x, y + v.y };
	}

	constexpr auto operator-(const Vector2D& v) const noexcept
	{
		return Vector2D{ x - v.x, y - v.y};
	}

	constexpr auto operator*(float mul) const noexcept
	{
		return Vector2D{ x * mul, y * mul};
	}

	constexpr auto operator/(float mul) const noexcept
	{
		return Vector2D{ x / mul, y / mul };
	}

	auto length() const noexcept
	{
		return std::sqrt(x * x + y * y);
	}

	float x, y;
};

struct Vector3D
{
	constexpr operator bool() const noexcept
	{
		return x || y || z;
	}

	constexpr Vector3D& operator=(float array[3]) noexcept
	{
		x = array[0];
		y = array[1];
		z = array[2];
		return *this;
	}

	constexpr Vector3D& operator+=(const Vector3D& v) noexcept
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	constexpr Vector3D& operator-=(const Vector3D& v) noexcept
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	constexpr auto operator-(const Vector3D& v) const noexcept
	{
		return Vector3D{ x - v.x, y - v.y, z - v.z };
	}

	constexpr auto operator+(const Vector3D& v) const noexcept
	{
		return Vector3D{ x + v.x, y + v.y, z + v.z };
	}

	constexpr Vector3D& operator/=(float div) noexcept
	{
		x /= div;
		y /= div;
		z /= div;
		return *this;
	}

	constexpr auto operator*(float mul) const noexcept
	{
		return Vector3D{ x * mul, y * mul, z * mul };
	}

	constexpr void normalize() noexcept
	{
		x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
		y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
		z = 0.0f;
	}

	auto length() const noexcept
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	auto length2D() const noexcept
	{
		return std::sqrt(x * x + y * y);
	}

	auto length3D() const noexcept
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	constexpr auto squareLength() const noexcept
	{
		return x * x + y * y + z * z;
	}

	constexpr auto dotProduct(const Vector3D& v) const noexcept
	{
		return x * v.x + y * v.y + z * v.z;
	}

	float x, y, z;
};
