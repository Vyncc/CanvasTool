#pragma once
#include <bakkesmod\wrappers\wrapperstructs.h>

class CanvasItem {
public:
	virtual ~CanvasItem() {}

	int ItemLayer; //canvas layer index
	std::string canvasType;

};


class CanvasDrawString : public CanvasItem
{
public:

	Vector2 pos;
	float xScale;
	float yScale;
	bool dropShadow;
	bool wrapText;
	bool loadCvar;
	float Color[3];
	int Opacity;
	std::string Text;

	Vector2F itemRectMax;
	bool isHovered();

	char nameBuffer[100] = "String";

	CanvasDrawString();

};

class CanvasDrawBox : public CanvasItem
{
public:

	Vector2 pos;
	Vector2 size;
	float Color[3];
	int Opacity;


	bool isHovered();

	char nameBuffer[100] = "Box";

	CanvasDrawBox();

};

class CanvasDrawFillBox : public CanvasItem
{
public:

	Vector2 pos;
	Vector2 size;
	float Color[3];
	int Opacity;

	bool isHovered();

	char nameBuffer[100] = "FillBox";

	CanvasDrawFillBox();

};

class CanvasDrawLine : public CanvasItem
{
public:

	float width;
	Vector2 startPos;
	Vector2 endPos;
	float Color[3];
	int Opacity;

	char nameBuffer[100] = "Line";

	CanvasDrawLine();

};

class CanvasDrawRect : public CanvasItem
{
public:

	Vector2 startPos;
	Vector2 endPos;
	float Color[3];
	int Opacity;
	std::vector<Vector2> GetRectSelectedBoxPos(Vector2 rectpos, Vector2 rectmax);

	bool isHovered();

	char nameBuffer[100] = "Rect";

	CanvasDrawRect();

};

class CanvasDrawFillTriangle : public CanvasItem
{
public:

	Vector2 p1;
	Vector2 p2;
	Vector2 p3;
	float Color[3];
	int Opacity;

	bool isHovered();

	char nameBuffer[100] = "FillTriangle";

	CanvasDrawFillTriangle();

};

class CanvasDrawTexture : public CanvasItem
{
public:

	std::shared_ptr<ImageWrapper> Texture;
	Vector2 pos;
	float scale;
	float Color[3];
	int Opacity;
	bool isTextureLoaded;
	bool SetImageSize;

	ImVec2 ImageSize = {10, 10};
	bool isHovered();

	char texturePathBuff[128];
	char nameBuffer[100] = "Texture";

	CanvasDrawTexture();

};

class CanvasDrawTile : public CanvasItem
{
public:

	std::shared_ptr<ImageWrapper> Texture;
	Vector2 pos;
	float XL;
	float YL;
	float U;
	float V;
	float UL;
	float VL;
	float Color[3];
	unsigned int ClipTile;
	bool isTextureLoaded;

	bool isHovered();

	char texturePathBuff[128] = "";
	char nameBuffer[100] = "Tile";

	CanvasDrawTile();

};

class CanvasDrawRotatedTile : public CanvasItem
{
public:

	std::shared_ptr<ImageWrapper> Texture;
	Vector2 pos;
	Rotator Rotation;
	float XL;
	float YL;
	float U;
	float V;
	float UL;
	float VL;
	float AnchorX;
	float AnchorY;
	bool isTextureLoaded;

	bool isHovered();

	char texturePathBuff[128] = "";
	char nameBuffer[100] = "RotatedTile";

	//Mark variables
	int MarkerSize;
	float MarkerColor[3];
	int MarkerOpacity;
	bool DrawMarker;

	CanvasDrawRotatedTile();

};
