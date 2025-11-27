#pragma once

struct Size
{
	unsigned int width = 0;
	unsigned int height = 0;
};

struct Point
{
	int x = 0;
	int y = 0;

	// Точка передаётся в локальных координатах.
	bool IsInSize(Size size) const noexcept
	{
		return x >= 0 && y >= 0 && x < size.width && y < size.height;
	}
};
