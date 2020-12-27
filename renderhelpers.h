#pragma once
#include <d2d1helper.h>
#include "Vector.h"



//Renders a black line with alpha gradient
void GradientLine(Vector2D start, Vector2D end, float startalpha, float endalpha, float thickness = 1.0f);
void PreviewLine();