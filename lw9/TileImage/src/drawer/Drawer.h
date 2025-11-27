#pragma once

#include "../image/Image.h"

namespace Drawer
{
/**
 * Рисует отрезок прямой линии между точками from и to цветом color на изображении Image.
 *
 * Для рисования используется алгоритм Брезенхэма.
 * (https://ru.wikipedia.org/wiki/Алгоритм_Брезенхэма)
 */
void DrawLine(Image& image, Point from, Point to, char color);

/**
 * Рисует окружность с центром в точке center и радиусом radius цветом color на изображении Image.
 *
 * Для рисования используется алгоритм Брезенхэма для окружностей.
 */
void DrawCircle(Image& image, Point center, int radius, char color);
} // namespace Drawer