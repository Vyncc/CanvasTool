#include "CanvasItems.h"
#include "pch.h"


CanvasDrawString::CanvasDrawString()
{
	pos = { 0, 0 };
	xScale = 8.f;
	yScale = 8.f;
	dropShadow = false;
	wrapText = false;
	loadCvar = false;
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
	Text = "String";

}
bool CanvasDrawString::isHovered()
{
	POINT point;
	GetCursorPos(&point);

	if (point.x >= pos.X && point.x <= pos.X + itemRectMax.X && point.y >= pos.Y && point.y <= pos.Y + itemRectMax.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CanvasDrawBox::CanvasDrawBox()
{
	pos = { 0, 0 };
	size = { 40, 40 };
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
}
bool CanvasDrawBox::isHovered()
{
	POINT point;
	GetCursorPos(&point);

	if (point.x >= pos.X && point.x <= pos.X + size.X && point.y >= pos.Y && point.y <= pos.Y + size.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CanvasDrawFillBox::CanvasDrawFillBox()
{
	pos = { 0, 0 };
	size = { 40, 40 };
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
}
bool CanvasDrawFillBox::isHovered()
{
	POINT point;
	GetCursorPos(&point);

	if (point.x >= pos.X && point.x <= pos.X + size.X && point.y >= pos.Y && point.y <= pos.Y + size.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CanvasDrawLine::CanvasDrawLine()
{
	width = 1.f;
	startPos = { 0, 0 };
	endPos = { 40, 40 };
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
}

CanvasDrawRect::CanvasDrawRect()
{
	startPos = { 0, 0 };
	endPos = { 40, 40 };
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
}
bool CanvasDrawRect::isHovered()
{
	POINT point;
	GetCursorPos(&point);

	if (point.x >= startPos.X && point.x <= endPos.X && point.y >= startPos.Y && point.y <= endPos.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CanvasDrawFillTriangle::CanvasDrawFillTriangle()
{
	p1 = { 100, 0 };
	p2 = { 0, 175 };
	p3 = { 200, 175 };
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
}
//https://webdevdesigner.com/q/how-to-determine-if-a-point-is-in-a-2d-triangle-36120/
bool CanvasDrawFillTriangle::isHovered()
{
	POINT point;
	GetCursorPos(&point);

	Vector2 s = { point.x, point.y };
	Vector2 a = p1;
	Vector2 b = p2;
	Vector2 c = p3;

	int as_x = s.X - a.X;
	int as_y = s.Y - a.Y;

	bool s_ab = (b.X - a.X) * as_y - (b.Y - a.Y) * as_x > 0;

	if ((c.X - a.X) * as_y - (c.Y - a.Y) * as_x > 0 == s_ab) return false;

	if ((c.X - b.X) * (s.Y - b.Y) - (c.Y - b.Y) * (s.X - b.X) > 0 != s_ab) return false;

	return true;
}

CanvasDrawTexture::CanvasDrawTexture()
{
	pos = { 0, 0 };
	scale = 1.f;
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
	isTextureLoaded = false;
}

CanvasDrawTile::CanvasDrawTile()
{
	pos = { 0, 0 };
	XL = 200.f;
	YL = 150.f;
	U = 0.f;
	V = 0.f;
	UL = 300.f;
	VL = 200.f;
	Color[0] = 0.0039;
	Color[1] = 0.0039;
	Color[2] = 0.0039;
	ClipTile = 1;
	isTextureLoaded = false;
}

CanvasDrawRotatedTile::CanvasDrawRotatedTile()
{
	pos = { 0, 0 };
	Rotation = { 0, 0, 0 };
	XL = 200.f;
	YL = 150.f;
	U = 0.f;
	V = 0.f;
	UL = 300.f;
	VL = 200.f;
	AnchorX = 0.f;
	AnchorY = 0.f;
	isTextureLoaded = false;

	//Mark variables
	MarkerSize = 10;
	MarkerColor[0] = 1;
	MarkerColor[1] = 1;
	MarkerColor[2] = 1;
	MarkerOpacity = 255;
	DrawMarker = true;
}