#include "CanvasItems.h"
#include "pch.h"


CanvasDrawString::CanvasDrawString()
{
	pos = { 0, 0 };
	xScale = 8.f;
	yScale = 8.f;
	dropShadow = false;
	wrapText = false;
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
	Text = "String";

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

CanvasDrawFillBox::CanvasDrawFillBox()
{
	pos = { 0, 0 };
	size = { 40, 40 };
	Color[0] = 1;
	Color[1] = 1;
	Color[2] = 1;
	Opacity = 255;
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