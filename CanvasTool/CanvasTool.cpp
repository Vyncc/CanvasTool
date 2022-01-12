#include "pch.h"
#include "CanvasTool.h"




BAKKESMOD_PLUGIN(CanvasTool, "A tool to render and visualize canvas", "1.0.1", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void CanvasTool::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->registerCvar("canvastool_carposx", "0", "car pos x");
	cvarManager->registerCvar("canvastool_carposy", "0", "car pos z");
	cvarManager->registerCvar("canvastool_carposz", "0", "car pos y");

	gameWrapper->RegisterDrawable(std::bind(&CanvasTool::RenderCanvas, this, std::placeholders::_1));
}



void CanvasTool::addItem(std::string itemName)
{
	int Layer;
	if (CanvasItems.size() == 0)
	{
		Layer = 0;
	}
	else
	{
		Layer = CanvasItems.back()->ItemLayer + 1;
	}


	if (itemName == "String")
	{
		auto Str = CanvasDrawString();
		Str.canvasType = "String";
		Str.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawString>(Str));

		cvarManager->log("Added a string (Layer : " + std::to_string(Layer) + ")");
		cvarManager->log(std::to_string(CanvasItems.size()));
	}

	if (itemName == "Box")
	{
		auto box = CanvasDrawBox();
		box.canvasType = "Box";
		box.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawBox>(box));

		cvarManager->log("Added a box (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Fill Box")
	{
		auto FillBox = CanvasDrawFillBox();
		FillBox.canvasType = "Fill Box";
		FillBox.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawFillBox>(FillBox));

		cvarManager->log("Added a fill box (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Line")
	{
		auto line = CanvasDrawLine();
		line.canvasType = "Line";
		line.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawLine>(line));

		cvarManager->log("Added a line (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Rect")
	{
		auto rect = CanvasDrawRect();
		rect.canvasType = "Rect";
		rect.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawRect>(rect));

		cvarManager->log("Added a rect (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Fill triangle")
	{
		auto fillTriangle = CanvasDrawFillTriangle();
		fillTriangle.canvasType = "Fill triangle";
		fillTriangle.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawFillTriangle>(fillTriangle));

		cvarManager->log("Added a fill triangle (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Texture")
	{
		auto texture = CanvasDrawTexture();
		texture.canvasType = "Texture";
		texture.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawTexture>(texture));

		cvarManager->log("Added a texture (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Tile")
	{
		auto tile = CanvasDrawTile();
		tile.canvasType = "Tile";
		tile.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawTile>(tile));

		cvarManager->log("Added a tile (Layer : " + std::to_string(Layer) + ")");
	}

	if (itemName == "Rotated Tile")
	{
		auto rotatedTile = CanvasDrawRotatedTile();
		rotatedTile.canvasType = "Rotated Tile";
		rotatedTile.ItemLayer = Layer;
		CanvasItems.push_back(std::make_shared<CanvasDrawRotatedTile>(rotatedTile));

		cvarManager->log("Added a rotatedTile (Layer : " + std::to_string(Layer) + ")");
	}

	SelectedIndex = CanvasItems.size() - 1;
}



void CanvasTool::RenderCanvas(CanvasWrapper canvas)
{

	static bool RButtonPressed = false;
	static bool LButtonPressed = false;

	//Check the mouse left button is pressed or not
	if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
	{
		cvarManager->log("LButton pressed");
		LButtonPressed = true;
	}
	else if ((GetKeyState(VK_LBUTTON) & 0x80) == 0 && LButtonPressed)
	{

		cvarManager->log("LButton realeased");
		LButtonPressed = false;
	}

	//Check the mouse right button is pressed or not
	if ((GetKeyState(VK_RBUTTON) & 0x80) != 0)
	{
		cvarManager->log("RButton pressed");
	}

	POINT point;
	GetCursorPos(&point);
	//cvarManager->log("x : " + std::to_string(point.x));
	//cvarManager->log("y : " + std::to_string(point.y));

	for (int i = 0; i < CanvasItems.size(); i++)
	{
		//cvarManager->log(std::to_string(i));
		std::shared_ptr<CanvasDrawString> StringPtr = std::dynamic_pointer_cast<CanvasDrawString>(CanvasItems.at(i));									//Render String
		
		/*if (StringPtr)
		{
			cvarManager->log(StringPtr->Text + " exists, itemLayer : " + std::to_string(StringPtr->ItemLayer));
		}*/
		if (StringPtr && StringPtr->ItemLayer == i)
		{
			canvas.SetColor((int)(StringPtr->Color[0] * 255), (int)(StringPtr->Color[1] * 255), (int)(StringPtr->Color[2] * 255), StringPtr->Opacity);
			canvas.SetPosition(StringPtr->pos);
			if (StringPtr->loadCvar)
			{
				if (!cvarManager->getCvar(std::string(StringPtr->nameBuffer)).IsNull())
				{
					StringPtr->Text = cvarManager->getCvar(std::string(StringPtr->nameBuffer)).getStringValue();
				}
				else
				{
					StringPtr->Text = "N/A";
				}
			}
			else
			{
				StringPtr->Text = std::string(StringPtr->nameBuffer);
			}
			canvas.DrawString(StringPtr->Text, StringPtr->xScale, StringPtr->yScale, StringPtr->dropShadow, StringPtr->wrapText);
			//cvarManager->log(StringPtr->Text + " rendered at layer : " + std::to_string(StringPtr->ItemLayer));



			StringPtr->itemRectMax = canvas.GetStringSize(StringPtr->Text, StringPtr->xScale, StringPtr->yScale);
			if (SelectedIndex == StringPtr->ItemLayer)
			{
				//if (!CanEditWhileWindowClosed && !isWindowOpen_) { break; }
				if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
				{
					//Green box
					canvas.SetColor(0, 255, 0, 255);
					canvas.SetPosition(Vector2{ StringPtr->pos.X - 1, StringPtr->pos.Y - 1 });
					canvas.DrawBox(Vector2{ int(StringPtr->itemRectMax.X + 1), int(StringPtr->itemRectMax.Y + 1) });

					if (StringPtr->isHovered())
					{
						static bool wasPressingL = false;
						if (LButtonPressed)
						{
							static int oldPosX = point.x;
							static int oldPosY = point.y;


							if (!wasPressingL)
							{
								oldPosX = point.x;
								oldPosY = point.y;
								wasPressingL = true;
							}

							StringPtr->pos.X += point.x - oldPosX;
							StringPtr->pos.Y += point.y - oldPosY;


							oldPosX = point.x;
							oldPosY = point.y;
						}
						else
						{
							wasPressingL = false;
						}
					}
				}
			}
		}




		std::shared_ptr<CanvasDrawBox> BoxPtr = std::dynamic_pointer_cast<CanvasDrawBox>(CanvasItems.at(i));											//Render Box
		if (BoxPtr && BoxPtr->ItemLayer == i)
		{
			canvas.SetColor((int)(BoxPtr->Color[0] * 255), (int)(BoxPtr->Color[1] * 255), (int)(BoxPtr->Color[2] * 255), BoxPtr->Opacity);
			canvas.SetPosition(Vector2{ BoxPtr->pos.X, BoxPtr->pos.Y });
			canvas.DrawBox(BoxPtr->size);


			if (SelectedIndex == BoxPtr->ItemLayer)
			{
				//if (!CanEditWhileWindowClosed && !isWindowOpen_) { break; }
				if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
				{
					//Green box
					canvas.SetColor(0, 255, 0, 255);
					canvas.SetPosition(Vector2{ BoxPtr->pos.X - 2, BoxPtr->pos.Y - 2 });
					canvas.DrawBox(Vector2{ int(BoxPtr->size.X + 4), int(BoxPtr->size.Y + 4) });

					if (BoxPtr->isHovered())
					{
						static bool wasPressingL = false;
						if (LButtonPressed)
						{
							static int oldPosX = point.x;
							static int oldPosY = point.y;


							if (!wasPressingL)
							{
								oldPosX = point.x;
								oldPosY = point.y;
								wasPressingL = true;
							}

							BoxPtr->pos.X += point.x - oldPosX;
							BoxPtr->pos.Y += point.y - oldPosY;


							oldPosX = point.x;
							oldPosY = point.y;
						}
						else
						{
							wasPressingL = false;
						}
					}
				}

				
			}
		}

		std::shared_ptr<CanvasDrawFillBox> FillBoxPtr = std::dynamic_pointer_cast<CanvasDrawFillBox>(CanvasItems.at(i));								//Render FillBox
		if (FillBoxPtr && FillBoxPtr->ItemLayer == i)
		{
			canvas.SetColor((int)(FillBoxPtr->Color[0] * 255), (int)(FillBoxPtr->Color[1] * 255), (int)(FillBoxPtr->Color[2] * 255), FillBoxPtr->Opacity);
			canvas.SetPosition(Vector2{ FillBoxPtr->pos.X, FillBoxPtr->pos.Y });
			canvas.FillBox(FillBoxPtr->size);



			if (SelectedIndex == FillBoxPtr->ItemLayer)
			{
				if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
				{
					//Green box
					canvas.SetColor(0, 255, 0, 255);
					canvas.SetPosition(Vector2{ FillBoxPtr->pos.X - 2, FillBoxPtr->pos.Y - 2 });
					canvas.DrawBox(Vector2{ int(FillBoxPtr->size.X + 4), int(FillBoxPtr->size.Y + 4) });

					if (FillBoxPtr->isHovered())
					{
						static bool wasPressingL = false;
						if (LButtonPressed)
						{
							static int oldPosX = point.x;
							static int oldPosY = point.y;


							if (!wasPressingL)
							{
								oldPosX = point.x;
								oldPosY = point.y;
								wasPressingL = true;
							}

							FillBoxPtr->pos.X += point.x - oldPosX;
							FillBoxPtr->pos.Y += point.y - oldPosY;


							oldPosX = point.x;
							oldPosY = point.y;
						}
						else
						{
							wasPressingL = false;
						}
					}
				}

				
			}
		}

		std::shared_ptr<CanvasDrawLine> LinePtr = std::dynamic_pointer_cast<CanvasDrawLine>(CanvasItems.at(i));											//Render Line
		if (LinePtr && LinePtr->ItemLayer == i)
		{
			canvas.SetColor((int)(LinePtr->Color[0] * 255), (int)(LinePtr->Color[1] * 255), (int)(LinePtr->Color[2] * 255), LinePtr->Opacity);
			canvas.DrawLine(LinePtr->startPos, LinePtr->endPos, LinePtr->width);
		}

		std::shared_ptr<CanvasDrawRect> RectPtr = std::dynamic_pointer_cast<CanvasDrawRect>(CanvasItems.at(i));											//Render Rect
		if (RectPtr && RectPtr->ItemLayer == i)
		{
			canvas.SetColor((int)(RectPtr->Color[0] * 255), (int)(RectPtr->Color[1] * 255), (int)(RectPtr->Color[2] * 255), RectPtr->Opacity);
			canvas.DrawRect(RectPtr->startPos, RectPtr->endPos);



			if (SelectedIndex == RectPtr->ItemLayer)
			{
				//if (!CanEditWhileWindowClosed && !isWindowOpen_) { break; }
				if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
				{
					//Green box
					canvas.SetColor(0, 255, 0, 255);
					std::vector<Vector2> RectPos = GetRectSelectedBoxPos(Vector2{ RectPtr->startPos.X , RectPtr->startPos.Y }, Vector2{ RectPtr->endPos.X , RectPtr->endPos.Y });
					canvas.SetPosition(RectPos.at(0));
					canvas.DrawBox(Vector2{ RectPos.at(1).X - RectPos.at(0).X, RectPos.at(1).Y - RectPos.at(0).Y });

					if (RectPtr->isHovered())
					{
						static bool wasPressingL = false;
						if (LButtonPressed)
						{
							static int oldPosX = point.x;
							static int oldPosY = point.y;


							if (!wasPressingL)
							{
								oldPosX = point.x;
								oldPosY = point.y;
								wasPressingL = true;
							}

							RectPtr->startPos.X += point.x - oldPosX;
							RectPtr->endPos.X += point.x - oldPosX;
							RectPtr->startPos.Y += point.y - oldPosY;
							RectPtr->endPos.Y += point.y - oldPosY;


							oldPosX = point.x;
							oldPosY = point.y;
						}
						else
						{
							wasPressingL = false;
						}
					}
				}
				
			}
		}

		std::shared_ptr<CanvasDrawFillTriangle> FillTrianglePtr = std::dynamic_pointer_cast<CanvasDrawFillTriangle>(CanvasItems.at(i));					//Render Fill Triangle
		if (FillTrianglePtr && FillTrianglePtr->ItemLayer == i)
		{
			canvas.SetColor((int)(FillTrianglePtr->Color[0] * 255), (int)(FillTrianglePtr->Color[1] * 255), (int)(FillTrianglePtr->Color[2] * 255), FillTrianglePtr->Opacity);
			canvas.FillTriangle(FillTrianglePtr->p1, FillTrianglePtr->p2, FillTrianglePtr->p3);

			if (SelectedIndex == FillTrianglePtr->ItemLayer)
			{
				//if (!CanEditWhileWindowClosed && !isWindowOpen_) { break; }
				if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
				{
					//Green box
					canvas.SetColor(0, 255, 0, 255);
					std::vector<Vector2> RectPos = GetTriangleSelectedBoxPos(FillTrianglePtr->p1, FillTrianglePtr->p2, FillTrianglePtr->p3);
					canvas.SetPosition(RectPos.at(0));
					canvas.DrawBox(Vector2{ RectPos.at(1).X - RectPos.at(0).X, RectPos.at(1).Y - RectPos.at(0).Y });

					if (FillTrianglePtr->isHovered())
					{
						static bool wasPressingL = false;
						if (LButtonPressed)
						{
							static int oldPosX = point.x;
							static int oldPosY = point.y;


							if (!wasPressingL)
							{
								oldPosX = point.x;
								oldPosY = point.y;
								wasPressingL = true;
							}

							FillTrianglePtr->p1.X += point.x - oldPosX;
							FillTrianglePtr->p2.X += point.x - oldPosX;
							FillTrianglePtr->p3.X += point.x - oldPosX;
							FillTrianglePtr->p1.Y += point.y - oldPosY;
							FillTrianglePtr->p2.Y += point.y - oldPosY;
							FillTrianglePtr->p3.Y += point.y - oldPosY;


							oldPosX = point.x;
							oldPosY = point.y;
						}
						else
						{
							wasPressingL = false;
						}
					}
				}
				
			}
		}

		std::shared_ptr<CanvasDrawTexture> TexturePtr = std::dynamic_pointer_cast<CanvasDrawTexture>(CanvasItems.at(i));								//Render Texture
		if (TexturePtr && TexturePtr->ItemLayer == i)
		{
			if (TexturePtr->isTextureLoaded)
			{
				canvas.SetColor((int)(TexturePtr->Color[0] * 255), (int)(TexturePtr->Color[1] * 255), (int)(TexturePtr->Color[2] * 255), TexturePtr->Opacity);
				canvas.SetPosition(Vector2{ TexturePtr->pos.X, TexturePtr->pos.Y });
				canvas.DrawTexture(TexturePtr->Texture.get(), TexturePtr->scale);

				if (SelectedIndex == TexturePtr->ItemLayer)
				{
					if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
					{
						//Green box
						canvas.SetColor(0, 255, 0, 255);
						canvas.SetPosition(TexturePtr->pos);
						canvas.DrawBox(Vector2{ int(TexturePtr->ImageSize.x * TexturePtr->scale), int(TexturePtr->ImageSize.y * TexturePtr->scale) });

						if (TexturePtr->isHovered())
						{
							static bool wasPressingL = false;
							if (LButtonPressed)
							{
								static int oldPosX = point.x;
								static int oldPosY = point.y;


								if (!wasPressingL)
								{
									oldPosX = point.x;
									oldPosY = point.y;
									wasPressingL = true;
								}

								TexturePtr->pos.X += point.x - oldPosX;
								TexturePtr->pos.Y += point.y - oldPosY;


								oldPosX = point.x;
								oldPosY = point.y;
							}
							else
							{
								wasPressingL = false;
							}
						}
					}

				}
			}
		}

		std::shared_ptr<CanvasDrawTile> TilePtr = std::dynamic_pointer_cast<CanvasDrawTile>(CanvasItems.at(i));								//Render Texture
		if (TilePtr && TilePtr->ItemLayer == i)
		{
			if (TilePtr->isTextureLoaded)
			{
				canvas.SetPosition(Vector2{ TilePtr->pos.X, TilePtr->pos.Y });
				canvas.DrawTile(TilePtr->Texture.get(), TilePtr->XL, TilePtr->YL, TilePtr->U, TilePtr->V, TilePtr->UL, TilePtr->VL, LinearColor{ (TilePtr->Color[0] * 255), (TilePtr->Color[1] * 255), (TilePtr->Color[2] * 255), 255 }, TilePtr->ClipTile, 1);

				if (SelectedIndex == TilePtr->ItemLayer)
				{
					if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
					{
						//Green box
						canvas.SetColor(0, 255, 0, 255);
						canvas.SetPosition(Vector2{ TilePtr->pos.X - 2, TilePtr->pos.Y - 2 });
						canvas.DrawBox(Vector2{ int(TilePtr->XL + 2), int(TilePtr->YL + 2) });

						if (TilePtr->isHovered())
						{
							static bool wasPressingL = false;
							if (LButtonPressed)
							{
								static int oldPosX = point.x;
								static int oldPosY = point.y;


								if (!wasPressingL)
								{
									oldPosX = point.x;
									oldPosY = point.y;
									wasPressingL = true;
								}

								TilePtr->pos.X += point.x - oldPosX;
								TilePtr->pos.Y += point.y - oldPosY;


								oldPosX = point.x;
								oldPosY = point.y;
							}
							else
							{
								wasPressingL = false;
							}
						}
					}
				}
			}
		}

		std::shared_ptr<CanvasDrawRotatedTile> RotatedTilePtr = std::dynamic_pointer_cast<CanvasDrawRotatedTile>(CanvasItems.at(i));								//Render Texture
		if (RotatedTilePtr && RotatedTilePtr->ItemLayer == i)
		{
			if (RotatedTilePtr->isTextureLoaded)
			{
				canvas.SetColor(255, 255, 255, 255);
				canvas.SetPosition(Vector2{ RotatedTilePtr->pos.X, RotatedTilePtr->pos.Y });
				canvas.DrawRotatedTile(RotatedTilePtr->Texture.get(), RotatedTilePtr->Rotation, RotatedTilePtr->XL, RotatedTilePtr->YL, RotatedTilePtr->U, RotatedTilePtr->V, RotatedTilePtr->UL, RotatedTilePtr->VL, RotatedTilePtr->AnchorX, RotatedTilePtr->AnchorY);
				
				if (SelectedIndex == RotatedTilePtr->ItemLayer)
				{
					if (CanEditWhileWindowClosed && !isWindowOpen_ || isWindowOpen_)
					{
						//Green box
						canvas.SetColor(0, 255, 0, 255);
						canvas.SetPosition(Vector2{ RotatedTilePtr->pos.X - 2, RotatedTilePtr->pos.Y - 2 });
						canvas.DrawBox(Vector2{ int(RotatedTilePtr->XL + 2), int(RotatedTilePtr->YL + 2) });

						if (RotatedTilePtr->isHovered())
						{
							static bool wasPressingL = false;
							if (LButtonPressed)
							{
								static int oldPosX = point.x;
								static int oldPosY = point.y;


								if (!wasPressingL)
								{
									oldPosX = point.x;
									oldPosY = point.y;
									wasPressingL = true;
								}

								RotatedTilePtr->pos.X += point.x - oldPosX;
								RotatedTilePtr->pos.Y += point.y - oldPosY;


								oldPosX = point.x;
								oldPosY = point.y;
							}
							else
							{
								wasPressingL = false;
							}
						}
					}
				}
			}


			if (RotatedTilePtr->DrawMarker)
			{
				int posX = RotatedTilePtr->pos.X + (RotatedTilePtr->XL * RotatedTilePtr->AnchorX);
				int posY = RotatedTilePtr->pos.Y + (RotatedTilePtr->YL * RotatedTilePtr->AnchorY);
				canvas.SetColor((int)(RotatedTilePtr->MarkerColor[0] * 255), (int)(RotatedTilePtr->MarkerColor[1] * 255), (int)(RotatedTilePtr->MarkerColor[2] * 255), RotatedTilePtr->MarkerOpacity);
				canvas.SetPosition(Vector2{ posX,  posY });
				canvas.FillBox(Vector2{ RotatedTilePtr->MarkerSize,  RotatedTilePtr->MarkerSize });

			}
		}

	}

	if (gameWrapper->IsInFreeplay() || gameWrapper->IsInCustomTraining())
	{
		CarWrapper car = gameWrapper->GetLocalCar();
		if (car.IsNull()) { return; }

		cvarManager->getCvar("canvastool_carposx").setValue(car.GetLocation().X);
		cvarManager->getCvar("canvastool_carposy").setValue(car.GetLocation().Y);
		cvarManager->getCvar("canvastool_carposz").setValue(car.GetLocation().Z);
	}
}

std::vector<Vector2> CanvasTool::GetTriangleSelectedBoxPos(Vector2 a, Vector2 b, Vector2 c)
{
	int mostSignificantX = 0;
	int mostSignificantY = 0;
	int lowestX = 1920;
	int lowestY = 1080;
	std::vector<int> Xposes = { a.X, b.X, c.X };
	std::vector<int> Yposes = { a.Y, b.Y, c.Y };

	for (int i = 0; i < Xposes.size(); i++)
	{
		if (Xposes.at(i) > mostSignificantX)
			mostSignificantX = Xposes.at(i);
		if (Xposes.at(i) < lowestX)
			lowestX = Xposes.at(i);
	}

	for (int i = 0; i < Yposes.size(); i++)
	{
		if (Yposes.at(i) > mostSignificantY)
			mostSignificantY = Yposes.at(i);
		if (Yposes.at(i) < lowestY)
			lowestY = Yposes.at(i);
	}
	Vector2 pos = { lowestX, lowestY };
	Vector2 rectMax = { mostSignificantX, mostSignificantY };
	return std::vector<Vector2> {pos, rectMax};
}

std::vector<Vector2> CanvasTool::GetRectSelectedBoxPos(Vector2 rectpos, Vector2 rectmax)
{
	int mostSignificantX = 0;
	int mostSignificantY = 0;
	int lowestX = 1920;
	int lowestY = 1080;
	std::vector<int> Xposes = { rectpos.X, rectmax.X };
	std::vector<int> Yposes = { rectpos.Y, rectmax.Y };

	for (int i = 0; i < Xposes.size(); i++)
	{
		if (Xposes.at(i) > mostSignificantX)
			mostSignificantX = Xposes.at(i);
		if (Xposes.at(i) < lowestX)
			lowestX = Xposes.at(i);
	}

	for (int i = 0; i < Yposes.size(); i++)
	{
		if (Yposes.at(i) > mostSignificantY)
			mostSignificantY = Yposes.at(i);
		if (Yposes.at(i) < lowestY)
			lowestY = Yposes.at(i);
	}
	Vector2 pos = { lowestX, lowestY };
	Vector2 rectMax = { mostSignificantX, mostSignificantY };
	return std::vector<Vector2> {pos, rectMax};
}

//http://www.cplusplus.com/forum/beginner/45217/
bool CanvasTool::GetImageSize(const char* fn, int* x, int* y)
{
	FILE* f = fopen(fn, "rb");
	if (f == 0) return false;
	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (len < 24) {
		fclose(f);
		return false;
	}
	// Strategy:
	// reading GIF dimensions requires the first 10 bytes of the file
	// reading PNG dimensions requires the first 24 bytes of the file
	// reading JPEG dimensions requires scanning through jpeg chunks
	// In all formats, the file is at least 24 bytes big, so we'll read that always
	unsigned char buf[24]; fread(buf, 1, 24, f);

	// For JPEGs, we need to read the first 12 bytes of each chunk.
	// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
	{
		long pos = 2;
		while (buf[2] == 0xFF)
		{
			if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB) break;
			pos += 2 + (buf[4] << 8) + buf[5];
			if (pos + 12 > len) break;
			fseek(f, pos, SEEK_SET); fread(buf + 2, 1, 12, f);
		}
	}

	fclose(f);

	// JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
	if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
	{
		*y = (buf[7] << 8) + buf[8];
		*x = (buf[9] << 8) + buf[10];
		//cout << *x << endl;
		return true;
	}

	// GIF: first three bytes say "GIF", next three give version number. Then dimensions
	if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
	{
		*x = buf[6] + (buf[7] << 8);
		*y = buf[8] + (buf[9] << 8);
		return true;
	}

	// PNG: the first frame is by definition an IHDR frame, which gives dimensions
	if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A
		&& buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
	{
		*x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
		*y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
		return true;
	}

	return false;
}

bool CanvasTool::FileExists(char pathBuff[100])
{
	bool FileExists;
	std::string path = pathBuff;
	std::fstream myFile(path);
	if (myFile.fail()) //check if the file exists
	{
		FileExists = false;
		return FileExists;
	}
	FileExists = true;

	return FileExists;
}

bool CanvasTool::FileIsImage(char pathBuff[100])
{
	bool IsImage;
	std::string extension = std::string(pathBuff).substr(std::string(pathBuff).length() - 4, 4);
	//cvarManager->log(extension);

	if (extension != ".png" && extension != ".jpg" && extension != "jpeg")
	{
		IsImage = false;
		return IsImage;
	}
	IsImage = true;

	return IsImage;
}

std::vector<std::string> CanvasTool::GetDrives()
{
	std::vector<std::string> Drives;
	int iCounter = 0;
	int iASCIILetter = (int)'a';

	DWORD dwDrivesMask = GetLogicalDrives();

	if (dwDrivesMask == 0) {
		printf("Failed to acquire mask of drives.\n");
		exit(EXIT_FAILURE);
	}

	printf("Drives available:\n");
	//extract the drives from the mask using 
	//logical and-ing with & and bitshift operator <<
	while (iCounter < 24) {
		if (dwDrivesMask & (1 << iCounter)) {
			printf("%c:\\ \n", iASCIILetter + iCounter);
			char driveLetter = iASCIILetter + iCounter;
			std::string str(1, driveLetter);
			Drives.push_back(str);
		}
		iCounter++;
	}
	return Drives;
}


void CanvasTool::onUnload()
{
}