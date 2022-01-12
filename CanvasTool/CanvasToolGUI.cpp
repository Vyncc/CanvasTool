#include "pch.h"
#include "CanvasTool.h"
#include "imgui/imgui_internal.h"
#include <filesystem>



void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


// Do ImGui rendering here
void CanvasTool::Render()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(820, 607), ImVec2(FLT_MAX, FLT_MAX));
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_MenuBar))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::Checkbox("Edit While Window Is Closed", &CanEditWhileWindowClosed);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}



	if (ImGui::BeginChild("##ItemsList", ImVec2(250, 0), true)) {

		const char* items[] = { "String", "Line", "Box", "Fill Box", "Rect", "Fill triangle" ,"Texture", "Tile", "Rotated Tile" };
		static int item_current = 0;
		ImGui::Combo("Items", &item_current, items, IM_ARRAYSIZE(items));

		if (ImGui::Button("Add Item"))
		{
			addItem(items[item_current]);
		}
		ImGui::Text("Canvas items : %d", CanvasItems.size());

		ImGui::Separator();

		RenderCanvasItems();
	}
	ImGui::EndChild();

	ImGui::SameLine();

	if (ImGui::BeginChild("##ItemDetails", ImVec2(0, 0), true)) {

		if (CanvasItems.size() > 0 && SelectedIndex <= CanvasItems.size() - 1) //second condition : to avoid "out of range" in CanvasItems
		{
			RenderCanvasItemDetails();

			ImGui::Separator();

			ImVec2 left1 = ImGui::GetCursorScreenPos();
			float windowWitdh = ImGui::GetWindowWidth();
			if (ImGui::Button("Remove"))
			{
				removeCanvasItem(SelectedIndex);
			}
			ImVec2 left2 = ImGui::GetCursorScreenPos();
			if (ImGui::Button("Copy"))
			{
				Copy(SelectedIndex);
			}

			//bof marche pas
			ImGui::SetCursorScreenPos(ImVec2(left1.x + (windowWitdh - 107.f), left1.y));
			if (ImGui::Button("Move Layer Up"))
			{
				MoveLayerUp(SelectedIndex);
			}
			ImGui::SetCursorScreenPos(ImVec2(left2.x + (windowWitdh - 123.f), left2.y));
			if (ImGui::Button("Move Layer Down"))
			{
				MoveLayerDown(SelectedIndex);
			}
		}
	}
	ImGui::EndChild();

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

void CanvasTool::RenderCanvasItemDetails()
{
	std::shared_ptr<CanvasDrawString> StringPtr = std::dynamic_pointer_cast<CanvasDrawString>(CanvasItems.at(SelectedIndex));									//Render String
	if (StringPtr)
	{
		RenderStringDetails(StringPtr);
	}

	std::shared_ptr<CanvasDrawBox> BoxPtr = std::dynamic_pointer_cast<CanvasDrawBox>(CanvasItems.at(SelectedIndex));											//Render Box
	if (BoxPtr)
	{
		RenderBoxDetails(BoxPtr);
	}

	std::shared_ptr<CanvasDrawFillBox> FillBoxPtr = std::dynamic_pointer_cast<CanvasDrawFillBox>(CanvasItems.at(SelectedIndex));								//Render FillBox
	if (FillBoxPtr)
	{
		RenderFillBoxDetails(FillBoxPtr);
	}

	std::shared_ptr<CanvasDrawLine> LinePtr = std::dynamic_pointer_cast<CanvasDrawLine>(CanvasItems.at(SelectedIndex));											//Render Line
	if (LinePtr)
	{
		RenderLineDetails(LinePtr);
	}

	std::shared_ptr<CanvasDrawRect> RectPtr = std::dynamic_pointer_cast<CanvasDrawRect>(CanvasItems.at(SelectedIndex));											//Render Rect
	if (RectPtr)
	{
		RenderRectDetails(RectPtr);
	}

	std::shared_ptr<CanvasDrawFillTriangle> FillTrianglePtr = std::dynamic_pointer_cast<CanvasDrawFillTriangle>(CanvasItems.at(SelectedIndex));					//Render Fill Triangle
	if (FillTrianglePtr)
	{
		RenderFillTriangleDetails(FillTrianglePtr);
	}

	std::shared_ptr<CanvasDrawTexture> TexturePtr = std::dynamic_pointer_cast<CanvasDrawTexture>(CanvasItems.at(SelectedIndex));								//Render Texture
	if (TexturePtr)
	{
		RenderTextureDetails(TexturePtr);
	}

	std::shared_ptr<CanvasDrawTile> TilePtr = std::dynamic_pointer_cast<CanvasDrawTile>(CanvasItems.at(SelectedIndex));														//Render Texture
	if (TilePtr)
	{
		RenderTileDetails(TilePtr);
	}

	std::shared_ptr<CanvasDrawRotatedTile> RotatedTilePtr = std::dynamic_pointer_cast<CanvasDrawRotatedTile>(CanvasItems.at(SelectedIndex));
	if (RotatedTilePtr)
	{
		RenderRotatedTileDetails(RotatedTilePtr);
	}
}


void CanvasTool::RenderCanvasItems()
{
	for (int i = 0; i < CanvasItems.size(); i++)
	{
		std::shared_ptr<CanvasDrawString> StringPtr = std::dynamic_pointer_cast<CanvasDrawString>(CanvasItems.at(i));
		if (StringPtr)
		{
			std::string itemName = "[" + std::to_string(StringPtr->ItemLayer) + "]" + std::string(StringPtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : String    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawBox> BoxPtr = std::dynamic_pointer_cast<CanvasDrawBox>(CanvasItems.at(i));
		if (BoxPtr)
		{
			std::string itemName = "[" + std::to_string(BoxPtr->ItemLayer) + "]" + std::string(BoxPtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Box    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawFillBox> FillBoxPtr = std::dynamic_pointer_cast<CanvasDrawFillBox>(CanvasItems.at(i));
		if (FillBoxPtr)
		{
			std::string itemName = "[" + std::to_string(FillBoxPtr->ItemLayer) + "]" + std::string(FillBoxPtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Fill Box    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawLine> LinePtr = std::dynamic_pointer_cast<CanvasDrawLine>(CanvasItems.at(i));
		if (LinePtr)
		{
			std::string itemName = "[" + std::to_string(LinePtr->ItemLayer) + "]" + std::string(LinePtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Line    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawRect> RectPtr = std::dynamic_pointer_cast<CanvasDrawRect>(CanvasItems.at(i));
		if (RectPtr)
		{
			std::string itemName = "[" + std::to_string(RectPtr->ItemLayer) + "]" + std::string(RectPtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Rect    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawFillTriangle> FillTrianglePtr = std::dynamic_pointer_cast<CanvasDrawFillTriangle>(CanvasItems.at(i));
		if (FillTrianglePtr)
		{
			std::string itemName = "[" + std::to_string(FillTrianglePtr->ItemLayer) + "]" + std::string(FillTrianglePtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Fill Triangle    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawTexture> TexturePtr = std::dynamic_pointer_cast<CanvasDrawTexture>(CanvasItems.at(i));
		if (TexturePtr)
		{
			std::string itemName = "[" + std::to_string(TexturePtr->ItemLayer) + "]" + std::string(TexturePtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Texture    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawTile> TilePtr = std::dynamic_pointer_cast<CanvasDrawTile>(CanvasItems.at(i));
		if (TilePtr)
		{
			std::string itemName = "[" + std::to_string(TilePtr->ItemLayer) + "]" + std::string(TilePtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Tile    Index : " + std::to_string(i));
			}
		}

		std::shared_ptr<CanvasDrawRotatedTile> RotatedTilePtr = std::dynamic_pointer_cast<CanvasDrawRotatedTile>(CanvasItems.at(i));
		if (RotatedTilePtr)
		{
			std::string itemName = "[" + std::to_string(RotatedTilePtr->ItemLayer) + "]" + std::string(RotatedTilePtr->nameBuffer) + "##" + std::to_string(i);
			if (ImGui::Selectable(itemName.c_str(), SelectedIndex == i))
			{
				SelectedIndex = i;
				cvarManager->log("Selected :   Type : Rotated Tile    Index : " + std::to_string(i));
			}
		}
	}
}

void CanvasTool::RenderStringDetails(std::shared_ptr<CanvasDrawString> drawString)
{
	if (drawString->loadCvar)
	{
		ImGui::Text("Type a cvar : ");
	}
	else
	{
		ImGui::Text("Text : ");
	}
	ImGui::InputTextEx("##name", "", drawString->nameBuffer, sizeof(drawString->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::SameLine();

	ImGui::Checkbox("Load a cvar", &drawString->loadCvar);

	ImGui::NewLine();

	ImGui::SliderInt("Position X", &drawString->pos.X, -600, 1920);
	ImGui::SliderInt("Position Y", &drawString->pos.Y, -600, 1080);
	ImGui::NewLine();
	ImGui::SliderFloat("xScale", &drawString->xScale, 0.0f, 100.f);
	ImGui::SliderFloat("yScale", &drawString->yScale, 0.0f, 100.f);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity", &drawString->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawString->Color);
	ImGui::Text("Float values : Red : %f", drawString->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawString->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawString->Color[2]);
	ImGui::NewLine();
	ImGui::Checkbox("Drop Shadow", &drawString->dropShadow);
	ImGui::Checkbox("Wrap Text", &drawString->wrapText);
}

void CanvasTool::RenderBoxDetails(std::shared_ptr<CanvasDrawBox> drawBox)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawBox->nameBuffer, sizeof(drawBox->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::SliderInt("Position X", &drawBox->pos.X, -600, 1920);
	ImGui::SliderInt("Position Y", &drawBox->pos.Y, -600, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("SizeX", &drawBox->size.X, 0.0, 1920);
	ImGui::SliderInt("SizeY", &drawBox->size.Y, 0.0, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity", &drawBox->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawBox->Color);
	ImGui::Text("Float values : Red : %f", drawBox->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawBox->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawBox->Color[2]);

}

void CanvasTool::RenderFillBoxDetails(std::shared_ptr<CanvasDrawFillBox> drawFillBox)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawFillBox->nameBuffer, sizeof(drawFillBox->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::SliderInt("Position X", &drawFillBox->pos.X, -600, 1920);
	ImGui::SliderInt("Position Y", &drawFillBox->pos.Y, -600, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("SizeX", &drawFillBox->size.X, 0.0, 1920);
	ImGui::SliderInt("SizeY", &drawFillBox->size.Y, 0.0, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity", &drawFillBox->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawFillBox->Color);
	ImGui::Text("Float values : Red : %f", drawFillBox->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawFillBox->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawFillBox->Color[2]);

}


void CanvasTool::RenderLineDetails(std::shared_ptr<CanvasDrawLine> drawLine)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawLine->nameBuffer, sizeof(drawLine->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::SliderInt("Start Pos X", &drawLine->startPos.X, 0.0, 1920);
	ImGui::SliderInt("Start Pos Y", &drawLine->startPos.Y, 0.0, 1080);
	ImGui::SliderInt("End Pos X", &drawLine->endPos.X, 0.0, 1920);
	ImGui::SliderInt("End Pos Y", &drawLine->endPos.Y, 0.0, 1080);
	ImGui::NewLine();
	ImGui::SliderFloat("width", &drawLine->width, 0.f, 400);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity", &drawLine->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawLine->Color);
	ImGui::Text("Float values : Red : %f", drawLine->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawLine->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawLine->Color[2]);

}

void CanvasTool::RenderRectDetails(std::shared_ptr<CanvasDrawRect> drawRect)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawRect->nameBuffer, sizeof(drawRect->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::SliderInt("Start Pos X", &drawRect->startPos.X, 0.0, 1920);
	ImGui::SliderInt("Start Pos Y", &drawRect->startPos.Y, 0.0, 1080);
	ImGui::SliderInt("End Pos X", &drawRect->endPos.X, 0.0, 1920);
	ImGui::SliderInt("End Pos Y", &drawRect->endPos.Y, 0.0, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity", &drawRect->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawRect->Color);
	ImGui::Text("Float values : Red : %f", drawRect->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawRect->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawRect->Color[2]);

}

void CanvasTool::RenderFillTriangleDetails(std::shared_ptr<CanvasDrawFillTriangle> drawFilTriangle)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawFilTriangle->nameBuffer, sizeof(drawFilTriangle->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::SliderInt("p1 X", &drawFilTriangle->p1.X, 0.0, 1920);
	ImGui::SliderInt("p1 Y", &drawFilTriangle->p1.Y, 0.0, 1080);
	ImGui::SliderInt("p2 X", &drawFilTriangle->p2.X, 0.0, 1920);
	ImGui::SliderInt("p2 Y", &drawFilTriangle->p2.Y, 0.0, 1080);
	ImGui::SliderInt("p3 X", &drawFilTriangle->p3.X, 0.0, 1920);
	ImGui::SliderInt("p3 Y", &drawFilTriangle->p3.Y, 0.0, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity(doesn't work idk why)", &drawFilTriangle->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawFilTriangle->Color);
	ImGui::Text("Float values : Red : %f", drawFilTriangle->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawFilTriangle->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawFilTriangle->Color[2]);

}

void CanvasTool::RenderTextureDetails(std::shared_ptr<CanvasDrawTexture> drawTexture)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawTexture->nameBuffer, sizeof(drawTexture->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::Text("Texture Path : ");
	ImGui::InputTextEx("##texturepath", "", drawTexture->texturePathBuff, sizeof(drawTexture->texturePathBuff), ImVec2(0, 0), ImGuiTextFlags_None);
	ImGui::SameLine();
	if (!FileExists(drawTexture->texturePathBuff))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "File doesn't exists !");
	}
	else if (!FileIsImage(drawTexture->texturePathBuff))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "Need png, jpg or jpeg !");
	}
	else
	{
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "You can apply ;)");
	}

	if (ImGui::Button("Open file explorer"))
	{
		ImGui::OpenPopup("Select an image");
	}
	renderFileExplorer(drawTexture);

	ImGui::SameLine();

	if (ImGui::Button("Apply Texture"))
	{
		const char* theFile = drawTexture->texturePathBuff;
		int the_x = 0;
		int the_y = 0;
		bool didRun = false;
		drawTexture->SetImageSize = false;

		didRun = GetImageSize(theFile, &the_x, &the_y);
		if (didRun)
			drawTexture->ImageSize = { float(the_x) , float(the_y) };
		else
			drawTexture->SetImageSize = true;

		updateTexture(drawTexture);
	}

	ImGui::NewLine();

	ImGui::SliderInt("Position X", &drawTexture->pos.X, -1920, 1920);
	ImGui::SliderInt("Position Y", &drawTexture->pos.Y, -1080, 1080);
	ImGui::NewLine();
	if (drawTexture->SetImageSize)
	{
		ImGui::Text("Set Dimensions");
		ImGui::SliderFloat("X", &drawTexture->ImageSize.x, 0, 1920);
		ImGui::SliderFloat("Y", &drawTexture->ImageSize.y, 0, 1080);
		ImGui::NewLine();
	}
	ImGui::SliderFloat("Scale", &drawTexture->scale, 0.f, 10.f);
	ImGui::NewLine();
	ImGui::SliderInt("Opacity", &drawTexture->Opacity, 0, 255);
	ImGui::ColorEdit3("Color", drawTexture->Color);
	ImGui::Text("Float values : Red : %f", drawTexture->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawTexture->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawTexture->Color[2]);
	ImGui::NewLine();
}

void CanvasTool::RenderTileDetails(std::shared_ptr<CanvasDrawTile> drawTile)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawTile->nameBuffer, sizeof(drawTile->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::Text("Texture Path : ");
	ImGui::InputTextEx("##texturepath", "", drawTile->texturePathBuff, sizeof(drawTile->texturePathBuff), ImVec2(0, 0), ImGuiTextFlags_None);
	ImGui::SameLine();
	if (!FileExists(drawTile->texturePathBuff))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "File doesn't exists !");
	}
	else if (!FileIsImage(drawTile->texturePathBuff))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "Need png, jpg or jpeg !");
	}
	else
	{
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "You can apply ;)");
	}

	if (ImGui::Button("Open file explorer"))
	{
		ImGui::OpenPopup("Select an image");
	}
	renderFileExplorer(drawTile);

	ImGui::SameLine();

	if (ImGui::Button("Apply Texture"))
	{
		updateTexture(drawTile);
	}

	ImGui::NewLine();

	ImGui::SliderInt("Position X", &drawTile->pos.X, -1920, 1920);
	ImGui::SliderInt("Position Y", &drawTile->pos.Y, -1080, 1080);
	ImGui::NewLine();
	ImGui::SliderFloat("XL", &drawTile->XL, 0.f, 1920.f); ImGui::SameLine(); HelpMarker("The width of the tile");
	ImGui::SliderFloat("YL", &drawTile->YL, 0.f, 1080.f); ImGui::SameLine(); HelpMarker("The height of the tile");
	ImGui::SliderFloat("U", &drawTile->U, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The horizontal position of the upper left corner of the portion of the texture to be drawn. This value is in texels");
	ImGui::SliderFloat("V", &drawTile->V, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The vertical position of the upper left corner of the portion of the texture to be drawn. This value is in texels");
	ImGui::SliderFloat("UL", &drawTile->UL, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The width of the portion of the texture to be drawn. This value is in texels.");
	ImGui::SliderFloat("VL", &drawTile->VL, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The height of the portion of the texture to be drawn. This value is in texels.");

	ImGui::ColorEdit3("Color", drawTile->Color);

	ImGui::Text("Float values : Red : %f", drawTile->Color[0]);
	ImGui::SameLine();
	ImGui::Text("Green : %f", drawTile->Color[1]);
	ImGui::SameLine();
	ImGui::Text("Blue : %f", drawTile->Color[2]);
	ImGui::NewLine();
}

void CanvasTool::RenderRotatedTileDetails(std::shared_ptr<CanvasDrawRotatedTile> drawRotatedTile)
{
	ImGui::Text("Item Name : ");
	ImGui::InputTextEx("##name", "", drawRotatedTile->nameBuffer, sizeof(drawRotatedTile->nameBuffer), ImVec2(0, 0), ImGuiTextFlags_None);

	ImGui::NewLine();

	ImGui::Text("Texture Path : ");
	ImGui::InputTextEx("##texturepath", "", drawRotatedTile->texturePathBuff, sizeof(drawRotatedTile->texturePathBuff), ImVec2(0, 0), ImGuiTextFlags_None);
	ImGui::SameLine();
	if (!FileExists(drawRotatedTile->texturePathBuff))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "File doesn't exists !");
	}
	else if (!FileIsImage(drawRotatedTile->texturePathBuff))
	{
		ImGui::TextColored(ImVec4(255, 0, 0, 1), "Need png, jpg or jpeg !");
	}
	else
	{
		ImGui::TextColored(ImVec4(0, 255, 0, 1), "You can apply ;)");
	}

	if (ImGui::Button("Open file explorer"))
	{
		ImGui::OpenPopup("Select an image");
	}
	renderFileExplorer(drawRotatedTile);

	ImGui::SameLine();

	if (ImGui::Button("Apply Texture"))
	{
		updateTexture(drawRotatedTile);
	}

	ImGui::NewLine();

	ImGui::SliderInt("Position X", &drawRotatedTile->pos.X, -600, 1920);
	ImGui::SliderInt("Position Y", &drawRotatedTile->pos.Y, -600, 1080);
	ImGui::NewLine();
	ImGui::SliderInt("Pitch", &drawRotatedTile->Rotation.Pitch, -600, 600);
	ImGui::SliderInt("Roll", &drawRotatedTile->Rotation.Roll, -600, 600);
	ImGui::SliderInt("Yaw", &drawRotatedTile->Rotation.Yaw, -100000, 100000);
	ImGui::NewLine();
	ImGui::SliderFloat("XL", &drawRotatedTile->XL, 0.f, 1920.f); ImGui::SameLine(); HelpMarker("The width of the tile");
	ImGui::SliderFloat("YL", &drawRotatedTile->YL, 0.f, 1080.f); ImGui::SameLine(); HelpMarker("The height of the tile");
	ImGui::SliderFloat("U", &drawRotatedTile->U, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The horizontal position of the upper left corner of the portion of the texture to be drawn. This value is in texels");
	ImGui::SliderFloat("V", &drawRotatedTile->V, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The vertical position of the upper left corner of the portion of the texture to be drawn. This value is in texels");
	ImGui::SliderFloat("UL", &drawRotatedTile->UL, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The width of the portion of the texture to be drawn. This value is in texels");
	ImGui::SliderFloat("VL", &drawRotatedTile->VL, 0.f, 10000.f); ImGui::SameLine(); HelpMarker("The height of the portion of the texture to be drawn. This value is in texels");
	ImGui::NewLine();
	ImGui::SliderFloat("AnchorX", &drawRotatedTile->AnchorX, 0.f, 1.f); ImGui::SameLine(); HelpMarker("The horizontal position in pixels relative to the top-left corner of the tile to use as the pivot point for the rotation");
	ImGui::SliderFloat("AnchorY", &drawRotatedTile->AnchorY, 0.f, 1.f); ImGui::SameLine(); HelpMarker("The vertical position in pixels relative to the top-left corner of the tile to use as the pivot point for the rotation.");
	ImGui::Checkbox("Draw Marker At Anchor", &drawRotatedTile->DrawMarker);
	if (drawRotatedTile->DrawMarker)
	{
		ImGui::SameLine();
		ImGui::ColorEdit3("Color", drawRotatedTile->MarkerColor, ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		ImGui::PushItemWidth(50.f);
		ImGui::SliderInt("Size", &drawRotatedTile->MarkerSize, 0, 20);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushItemWidth(100.f);
		ImGui::SliderInt("Opacity", &drawRotatedTile->MarkerOpacity, 0, 255);
		ImGui::PopItemWidth();
	}
}

void CanvasTool::updateTexture(std::shared_ptr<CanvasDrawTexture> drawTexture)
{
	if (drawTexture->texturePathBuff == "")
	{
		cvarManager->log("Texture : path is empty !");
		return;
	}

	std::fstream myFile(std::string(drawTexture->texturePathBuff));
	if (myFile.fail()) //check if the file exists
	{
		cvarManager->log("Texture : File doesn't exists !");
		return;
	}

	std::string extension = std::string(drawTexture->texturePathBuff).substr(std::string(drawTexture->texturePathBuff).length() - 4, 4);
	cvarManager->log(extension);

	if (extension != ".png" && extension != ".jpg" && extension != "jpeg")
	{
		cvarManager->log("Texture : Need png, jpg or jpeg");
		return;
	}

	drawTexture->Texture = std::make_shared<ImageWrapper>(std::string(drawTexture->texturePathBuff), true, false);
	drawTexture->isTextureLoaded = true;
}

void CanvasTool::updateTexture(std::shared_ptr<CanvasDrawTile> drawTile)
{
	if (drawTile->texturePathBuff == "")
	{
		cvarManager->log("Texture : path is empty !");
		return;
	}

	std::fstream myFile(std::string(drawTile->texturePathBuff));
	if (myFile.fail()) //check if the file exists
	{
		cvarManager->log("Texture : File doesn't exists !");
		return;
	}

	std::string extension = std::string(drawTile->texturePathBuff).substr(std::string(drawTile->texturePathBuff).length() - 4, 4);
	cvarManager->log(extension);

	if (extension != ".png" && extension != ".jpg" && extension != "jpeg")
	{
		cvarManager->log("Texture : Need png, jpg or jpeg");
		return;
	}

	drawTile->Texture = std::make_shared<ImageWrapper>(std::string(drawTile->texturePathBuff), true, false);
	drawTile->isTextureLoaded = true;
}

void CanvasTool::updateTexture(std::shared_ptr<CanvasDrawRotatedTile> drawRotatedTile)
{
	if (drawRotatedTile->texturePathBuff == "")
	{
		cvarManager->log("Texture : path is empty !");
		return;
	}

	std::fstream myFile(std::string(drawRotatedTile->texturePathBuff));
	if (myFile.fail()) //check if the file exists
	{
		cvarManager->log("Texture : File doesn't exists !");
		return;
	}

	std::string extension = std::string(drawRotatedTile->texturePathBuff).substr(std::string(drawRotatedTile->texturePathBuff).length() - 4, 4);
	cvarManager->log(extension);

	if (extension != ".png" && extension != ".jpg" && extension != "jpeg")
	{
		cvarManager->log("Texture : Need png, jpg or jpeg");
		return;
	}

	drawRotatedTile->Texture = std::make_shared<ImageWrapper>(std::string(drawRotatedTile->texturePathBuff), true, false);
	drawRotatedTile->isTextureLoaded = true;
}



void CanvasTool::removeCanvasItem(int canvasItemindex)
{
	std::vector<std::shared_ptr<CanvasItem>>::iterator begin = CanvasItems.begin();
	std::advance(begin, canvasItemindex);
	CanvasItems.erase(begin);


	for (int i = 0; i < CanvasItems.size(); i++)  //Reseting all the ItemLayers (need for renderCanvas to avoid a gap)
	{
		CanvasItems.at(i)->ItemLayer = i;
	}

	if (SelectedIndex > CanvasItems.size() - 1);
	{
		SelectedIndex = CanvasItems.size() - 1;
	}
}


void CanvasTool::MoveLayerUp(int canvasItemindex)
{
	int index = canvasItemindex + 1;
	if (index > CanvasItems.size() - 1) { return; }

	std::shared_ptr<CanvasItem> itemMoveUp = CanvasItems.at(canvasItemindex);
	std::shared_ptr<CanvasItem> itemMoveDown = CanvasItems.at(index);

	CanvasItems.at(index) = itemMoveUp;
	CanvasItems.at(canvasItemindex) = itemMoveDown;

	for (int i = 0; i < CanvasItems.size(); i++)  //Reseting all the ItemLayers
	{
		CanvasItems.at(i)->ItemLayer = i;
	}

	SelectedIndex = index;
}

void CanvasTool::MoveLayerDown(int canvasItemindex)
{
	int index = canvasItemindex - 1;
	if (index < 0) { return; }

	std::shared_ptr<CanvasItem> itemMoveDown = CanvasItems.at(canvasItemindex);
	std::shared_ptr<CanvasItem> itemMoveUp = CanvasItems.at(index);

	CanvasItems.at(index) = itemMoveDown;
	CanvasItems.at(canvasItemindex) = itemMoveUp;

	for (int i = 0; i < CanvasItems.size(); i++)  //Reseting all the ItemLayers
	{
		CanvasItems.at(i)->ItemLayer = i;
	}

	SelectedIndex = index;
}

void CanvasTool::Copy(int canvasItemindex)
{
	auto selectedItem = CanvasItems.at(canvasItemindex);
	addItem(selectedItem->canvasType); //add new CanvasItem, the same canvasType as the selected item

	auto lastItem = CanvasItems.back();

	//then replace the values of the copy by the values of the item we wanted to copy
	std::shared_ptr<CanvasDrawString> copyStringPtr = std::dynamic_pointer_cast<CanvasDrawString>(lastItem);
	if (copyStringPtr)
	{
		std::shared_ptr<CanvasDrawString> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawString>(selectedItem);

		//http://www.cplusplus.com/forum/beginner/111242/
		//makes copyStringPtr->nameBuffer = selectedItemPtr->nameBuffer
		int stringLength = strlen(selectedItemPtr->nameBuffer);
		for (int i = 0; i < stringLength; i++) {
			copyStringPtr->nameBuffer[i] = selectedItemPtr->nameBuffer[i];
		}

		copyStringPtr->pos = selectedItemPtr->pos;
		copyStringPtr->xScale = selectedItemPtr->xScale;
		copyStringPtr->yScale = selectedItemPtr->yScale;
		copyStringPtr->dropShadow = selectedItemPtr->dropShadow;
		copyStringPtr->wrapText = selectedItemPtr->wrapText;
		copyStringPtr->loadCvar = selectedItemPtr->loadCvar;
		copyStringPtr->Color[0] = selectedItemPtr->Color[0];
		copyStringPtr->Color[1] = selectedItemPtr->Color[1];
		copyStringPtr->Color[2] = selectedItemPtr->Color[2];
		copyStringPtr->Opacity = selectedItemPtr->Opacity;
	}

	std::shared_ptr<CanvasDrawBox> copyBoxPtr = std::dynamic_pointer_cast<CanvasDrawBox>(lastItem);
	if (copyBoxPtr)
	{
		std::shared_ptr<CanvasDrawBox> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawBox>(selectedItem);
		copyBoxPtr->pos = selectedItemPtr->pos;
		copyBoxPtr->size = selectedItemPtr->size;
		copyBoxPtr->Color[0] = selectedItemPtr->Color[0];
		copyBoxPtr->Color[1] = selectedItemPtr->Color[1];
		copyBoxPtr->Color[2] = selectedItemPtr->Color[2];
		copyBoxPtr->Opacity = selectedItemPtr->Opacity;
	}

	std::shared_ptr<CanvasDrawFillBox> copyFillBoxPtr = std::dynamic_pointer_cast<CanvasDrawFillBox>(lastItem);
	if (copyFillBoxPtr)
	{
		std::shared_ptr<CanvasDrawFillBox> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawFillBox>(selectedItem);
		copyFillBoxPtr->pos = selectedItemPtr->pos;
		copyFillBoxPtr->size = selectedItemPtr->size;
		copyFillBoxPtr->Color[0] = selectedItemPtr->Color[0];
		copyFillBoxPtr->Color[1] = selectedItemPtr->Color[1];
		copyFillBoxPtr->Color[2] = selectedItemPtr->Color[2];
		copyFillBoxPtr->Opacity = selectedItemPtr->Opacity;
	}

	std::shared_ptr<CanvasDrawLine> copyLinePtr = std::dynamic_pointer_cast<CanvasDrawLine>(lastItem);
	if (copyLinePtr)
	{
		std::shared_ptr<CanvasDrawLine> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawLine>(selectedItem);
		copyLinePtr->width = selectedItemPtr->width;
		copyLinePtr->startPos = selectedItemPtr->startPos;
		copyLinePtr->endPos = selectedItemPtr->endPos;
		copyLinePtr->Color[0] = selectedItemPtr->Color[0];
		copyLinePtr->Color[1] = selectedItemPtr->Color[1];
		copyLinePtr->Color[2] = selectedItemPtr->Color[2];
		copyLinePtr->Opacity = selectedItemPtr->Opacity;
	}

	std::shared_ptr<CanvasDrawRect> copyRectPtr = std::dynamic_pointer_cast<CanvasDrawRect>(lastItem);
	if (copyRectPtr)
	{
		std::shared_ptr<CanvasDrawRect> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawRect>(selectedItem);
		copyRectPtr->startPos = selectedItemPtr->startPos;
		copyRectPtr->endPos = selectedItemPtr->endPos;
		copyRectPtr->Color[0] = selectedItemPtr->Color[0];
		copyRectPtr->Color[1] = selectedItemPtr->Color[1];
		copyRectPtr->Color[2] = selectedItemPtr->Color[2];
		copyRectPtr->Opacity = selectedItemPtr->Opacity;
	}

	std::shared_ptr<CanvasDrawFillTriangle> copyFillTrianglePtr = std::dynamic_pointer_cast<CanvasDrawFillTriangle>(lastItem);
	if (copyFillTrianglePtr)
	{
		std::shared_ptr<CanvasDrawFillTriangle> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawFillTriangle>(selectedItem);
		copyFillTrianglePtr->p1 = selectedItemPtr->p1;
		copyFillTrianglePtr->p2 = selectedItemPtr->p2;
		copyFillTrianglePtr->p3 = selectedItemPtr->p3;
		copyFillTrianglePtr->Color[0] = selectedItemPtr->Color[0];
		copyFillTrianglePtr->Color[1] = selectedItemPtr->Color[1];
		copyFillTrianglePtr->Color[2] = selectedItemPtr->Color[2];
		copyFillTrianglePtr->Opacity = selectedItemPtr->Opacity;
	}

	std::shared_ptr<CanvasDrawTexture> copyTexturePtr = std::dynamic_pointer_cast<CanvasDrawTexture>(lastItem);
	if (copyTexturePtr)
	{
		std::shared_ptr<CanvasDrawTexture> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawTexture>(selectedItem);
		copyTexturePtr->pos = selectedItemPtr->pos;
		copyTexturePtr->scale = selectedItemPtr->scale;
		copyTexturePtr->Color[0] = selectedItemPtr->Color[0];
		copyTexturePtr->Color[1] = selectedItemPtr->Color[1];
		copyTexturePtr->Color[2] = selectedItemPtr->Color[2];
		copyTexturePtr->Opacity = selectedItemPtr->Opacity;
		copyTexturePtr->Texture = selectedItemPtr->Texture;
		copyTexturePtr->isTextureLoaded = selectedItemPtr->isTextureLoaded;
		//copyTexturePtr->texturePathBuff = selectedItemPtr->texturePathBuff; //need to find a solution
	}

	std::shared_ptr<CanvasDrawTile> copyTilePtr = std::dynamic_pointer_cast<CanvasDrawTile>(lastItem);
	if (copyTilePtr)
	{
		std::shared_ptr<CanvasDrawTile> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawTile>(selectedItem);
		copyTilePtr->pos = selectedItemPtr->pos;
		copyTilePtr->XL = selectedItemPtr->XL;
		copyTilePtr->YL = selectedItemPtr->YL;
		copyTilePtr->U = selectedItemPtr->U;
		copyTilePtr->V = selectedItemPtr->V;
		copyTilePtr->UL = selectedItemPtr->UL;
		copyTilePtr->VL = selectedItemPtr->VL;
		copyTilePtr->Color[0] = selectedItemPtr->Color[0];
		copyTilePtr->Color[1] = selectedItemPtr->Color[1];
		copyTilePtr->Color[2] = selectedItemPtr->Color[2];
		copyTilePtr->ClipTile = selectedItemPtr->ClipTile;
		copyTilePtr->Texture = selectedItemPtr->Texture;
		copyTilePtr->isTextureLoaded = selectedItemPtr->isTextureLoaded;
	}

	std::shared_ptr<CanvasDrawRotatedTile> copyRotatedTilePtr = std::dynamic_pointer_cast<CanvasDrawRotatedTile>(lastItem);
	if (copyRotatedTilePtr)
	{
		std::shared_ptr<CanvasDrawRotatedTile> selectedItemPtr = std::dynamic_pointer_cast<CanvasDrawRotatedTile>(selectedItem);
		copyRotatedTilePtr->pos = selectedItemPtr->pos;
		copyRotatedTilePtr->Rotation = selectedItemPtr->Rotation;
		copyRotatedTilePtr->XL = selectedItemPtr->XL;
		copyRotatedTilePtr->YL = selectedItemPtr->YL;
		copyRotatedTilePtr->U = selectedItemPtr->U;
		copyRotatedTilePtr->V = selectedItemPtr->V;
		copyRotatedTilePtr->UL = selectedItemPtr->UL;
		copyRotatedTilePtr->VL = selectedItemPtr->VL;
		copyRotatedTilePtr->AnchorX = selectedItemPtr->AnchorX;
		copyRotatedTilePtr->AnchorY = selectedItemPtr->AnchorY;
		copyRotatedTilePtr->Texture = selectedItemPtr->Texture;
		copyRotatedTilePtr->isTextureLoaded = selectedItemPtr->isTextureLoaded;

		copyRotatedTilePtr->MarkerSize = selectedItemPtr->MarkerSize;
		copyRotatedTilePtr->MarkerColor[0] = selectedItemPtr->MarkerColor[0];
		copyRotatedTilePtr->MarkerColor[1] = selectedItemPtr->MarkerColor[1];
		copyRotatedTilePtr->MarkerColor[2] = selectedItemPtr->MarkerColor[2];
		copyRotatedTilePtr->MarkerOpacity = selectedItemPtr->MarkerOpacity;
		copyRotatedTilePtr->DrawMarker = selectedItemPtr->DrawMarker;
	}
	
	cvarManager->log("Copied : " + std::to_string(CanvasItems.at(canvasItemindex)->ItemLayer));
}

void CanvasTool::renderFileExplorer(std::shared_ptr<CanvasDrawTexture> drawTexture)
{
	ImGui::SetNextWindowSize(ImVec2(600.f, 429.f));
	if (ImGui::BeginPopupModal("Select an image", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char newFolderName[128] = "";

		static char fullPathBuff[256] = "C:/";
		std::filesystem::path currentPath = fullPathBuff;

		ImGui::BeginChild("##fullPath", ImVec2(ImGui::GetContentRegionAvailWidth(), 35.f), true);
		{
			ImGui::Columns(2, 0, true);
			ImGui::SetColumnWidth(0, 40.f);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
			if (ImGui::Selectable("<--"))
			{
				strncpy(fullPathBuff, currentPath.parent_path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
			}

			ImGui::NextColumn();

			std::vector<std::string> Drives = GetDrives();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 108.f);
			if (ImGui::BeginCombo("", fullPathBuff))
			{
				for (auto drive : Drives)
				{
					drive += ":/";
					if (ImGui::Selectable(drive.c_str()))
					{
						strncpy(fullPathBuff, drive.c_str(), IM_ARRAYSIZE(fullPathBuff));
						currentPath = fullPathBuff;
					}
				}
				ImGui::EndCombo();
			}

			currentPath = fullPathBuff;

			ImGui::SameLine();

			if (ImGui::Button("New Folder", ImVec2(100.f, 19.f)))
			{
				strncpy(newFolderName, "", IM_ARRAYSIZE(newFolderName));
				ImGui::OpenPopup("Folder Name");
			}
			if (ImGui::BeginPopupModal("Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("##newFloderNameInputText", newFolderName, IM_ARRAYSIZE(newFolderName));
				if (ImGui::Button("Confirm", ImVec2(100.f, 25.f)))
				{
					try
					{
						std::filesystem::create_directory(currentPath.string() + "/" + newFolderName);
					}
					catch (const std::exception& ex) //manage errors when trying to create a folder in an administrator folder
					{
						cvarManager->log(ex.what());
					}
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(100.f, 25.f)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::EndChild();
		}

		std::vector<std::filesystem::directory_entry> direcoriesList;
		std::vector<std::filesystem::directory_entry> filesList;
		static int selectedFile = -1;

		ImGui::BeginChild("##directories", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.75f), true);
		{
			try
			{
				for (const auto& dir : std::filesystem::directory_iterator(currentPath))
				{
					if (dir.is_directory())
					{
						direcoriesList.push_back(dir);
					}
					else if(dir.path().extension() == ".png" || dir.path().extension() == ".jpg" || dir.path().extension() == ".jpeg")
					{
						filesList.push_back(dir);
					}
				}


				for (auto directory : direcoriesList)
				{
					if (ImGui::Selectable(directory.path().filename().string().c_str()))
					{
						strncpy(fullPathBuff, directory.path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
					}
				}


				for (int i = 0; i < filesList.size(); i++)
				{
					if (ImGui::Selectable(filesList[i].path().filename().string().c_str(), selectedFile == i))
					{
						selectedFile = i;
					}
				}
			}
			catch (const std::exception& ex)
			{
				cvarManager->log("error : " + std::string(ex.what()));
				strncpy(fullPathBuff, currentPath.parent_path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
			}

			ImGui::EndChild();
		}

		if (ImGui::Button("Cancel", ImVec2(100.f, 30.f)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		AlignRightNexIMGUItItem(100.f, 8.f);
		if (ImGui::Button("Select", ImVec2(100.f, 30.f)))
		{
			strncpy(drawTexture->texturePathBuff, filesList[selectedFile].path().string().c_str(), IM_ARRAYSIZE(drawTexture->texturePathBuff));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void CanvasTool::renderFileExplorer(std::shared_ptr<CanvasDrawTile> drawTile)
{
	ImGui::SetNextWindowSize(ImVec2(600.f, 429.f));
	if (ImGui::BeginPopupModal("Select an image", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char newFolderName[128] = "";

		static char fullPathBuff[256] = "C:/";
		std::filesystem::path currentPath = fullPathBuff;

		ImGui::BeginChild("##fullPath", ImVec2(ImGui::GetContentRegionAvailWidth(), 35.f), true);
		{
			ImGui::Columns(2, 0, true);
			ImGui::SetColumnWidth(0, 40.f);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
			if (ImGui::Selectable("<--"))
			{
				strncpy(fullPathBuff, currentPath.parent_path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
			}

			ImGui::NextColumn();

			std::vector<std::string> Drives = GetDrives();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 108.f);
			if (ImGui::BeginCombo("", fullPathBuff))
			{
				for (auto drive : Drives)
				{
					drive += ":/";
					if (ImGui::Selectable(drive.c_str()))
					{
						strncpy(fullPathBuff, drive.c_str(), IM_ARRAYSIZE(fullPathBuff));
						currentPath = fullPathBuff;
					}
				}
				ImGui::EndCombo();
			}

			currentPath = fullPathBuff;

			ImGui::SameLine();

			if (ImGui::Button("New Folder", ImVec2(100.f, 19.f)))
			{
				strncpy(newFolderName, "", IM_ARRAYSIZE(newFolderName));
				ImGui::OpenPopup("Folder Name");
			}
			if (ImGui::BeginPopupModal("Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("##newFloderNameInputText", newFolderName, IM_ARRAYSIZE(newFolderName));
				if (ImGui::Button("Confirm", ImVec2(100.f, 25.f)))
				{
					try
					{
						std::filesystem::create_directory(currentPath.string() + "/" + newFolderName);
					}
					catch (const std::exception& ex) //manage errors when trying to create a folder in an administrator folder
					{
						cvarManager->log(ex.what());
					}
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(100.f, 25.f)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::EndChild();
		}

		std::vector<std::filesystem::directory_entry> direcoriesList;
		std::vector<std::filesystem::directory_entry> filesList;
		static int selectedFile = -1;

		ImGui::BeginChild("##directories", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.75f), true);
		{
			try
			{
				for (const auto& dir : std::filesystem::directory_iterator(currentPath))
				{
					if (dir.is_directory())
					{
						direcoriesList.push_back(dir);
					}
					else if (dir.path().extension() == ".png" || dir.path().extension() == ".jpg" || dir.path().extension() == ".jpeg")
					{
						filesList.push_back(dir);
					}
				}


				for (auto directory : direcoriesList)
				{
					if (ImGui::Selectable(directory.path().filename().string().c_str()))
					{
						strncpy(fullPathBuff, directory.path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
					}
				}


				for (int i = 0; i < filesList.size(); i++)
				{
					if (ImGui::Selectable(filesList[i].path().filename().string().c_str(), selectedFile == i))
					{
						selectedFile = i;
					}
				}
			}
			catch (const std::exception& ex)
			{
				cvarManager->log("error : " + std::string(ex.what()));
				strncpy(fullPathBuff, currentPath.parent_path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
			}

			ImGui::EndChild();
		}

		if (ImGui::Button("Cancel", ImVec2(100.f, 30.f)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		AlignRightNexIMGUItItem(100.f, 8.f);
		if (ImGui::Button("Select", ImVec2(100.f, 30.f)))
		{
			strncpy(drawTile->texturePathBuff, filesList[selectedFile].path().string().c_str(), IM_ARRAYSIZE(drawTile->texturePathBuff));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void CanvasTool::renderFileExplorer(std::shared_ptr<CanvasDrawRotatedTile> drawRotatedTile)
{
	ImGui::SetNextWindowSize(ImVec2(600.f, 429.f));
	if (ImGui::BeginPopupModal("Select an image", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char newFolderName[128] = "";

		static char fullPathBuff[256] = "C:/";
		std::filesystem::path currentPath = fullPathBuff;

		ImGui::BeginChild("##fullPath", ImVec2(ImGui::GetContentRegionAvailWidth(), 35.f), true);
		{
			ImGui::Columns(2, 0, true);
			ImGui::SetColumnWidth(0, 40.f);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
			if (ImGui::Selectable("<--"))
			{
				strncpy(fullPathBuff, currentPath.parent_path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
			}

			ImGui::NextColumn();

			std::vector<std::string> Drives = GetDrives();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - 108.f);
			if (ImGui::BeginCombo("", fullPathBuff))
			{
				for (auto drive : Drives)
				{
					drive += ":/";
					if (ImGui::Selectable(drive.c_str()))
					{
						strncpy(fullPathBuff, drive.c_str(), IM_ARRAYSIZE(fullPathBuff));
						currentPath = fullPathBuff;
					}
				}
				ImGui::EndCombo();
			}

			currentPath = fullPathBuff;

			ImGui::SameLine();

			if (ImGui::Button("New Folder", ImVec2(100.f, 19.f)))
			{
				strncpy(newFolderName, "", IM_ARRAYSIZE(newFolderName));
				ImGui::OpenPopup("Folder Name");
			}
			if (ImGui::BeginPopupModal("Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("##newFloderNameInputText", newFolderName, IM_ARRAYSIZE(newFolderName));
				if (ImGui::Button("Confirm", ImVec2(100.f, 25.f)))
				{
					try
					{
						std::filesystem::create_directory(currentPath.string() + "/" + newFolderName);
					}
					catch (const std::exception& ex) //manage errors when trying to create a folder in an administrator folder
					{
						cvarManager->log(ex.what());
					}
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel", ImVec2(100.f, 25.f)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::EndChild();
		}

		std::vector<std::filesystem::directory_entry> direcoriesList;
		std::vector<std::filesystem::directory_entry> filesList;
		static int selectedFile = -1;

		ImGui::BeginChild("##directories", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetWindowHeight() * 0.75f), true);
		{
			try
			{
				for (const auto& dir : std::filesystem::directory_iterator(currentPath))
				{
					if (dir.is_directory())
					{
						direcoriesList.push_back(dir);
					}
					else if (dir.path().extension() == ".png" || dir.path().extension() == ".jpg" || dir.path().extension() == ".jpeg")
					{
						filesList.push_back(dir);
					}
				}


				for (auto directory : direcoriesList)
				{
					if (ImGui::Selectable(directory.path().filename().string().c_str()))
					{
						strncpy(fullPathBuff, directory.path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
					}
				}


				for (int i = 0; i < filesList.size(); i++)
				{
					if (ImGui::Selectable(filesList[i].path().filename().string().c_str(), selectedFile == i))
					{
						selectedFile = i;
					}
				}
			}
			catch (const std::exception& ex)
			{
				cvarManager->log("error : " + std::string(ex.what()));
				strncpy(fullPathBuff, currentPath.parent_path().string().c_str(), IM_ARRAYSIZE(fullPathBuff));
			}

			ImGui::EndChild();
		}

		if (ImGui::Button("Cancel", ImVec2(100.f, 30.f)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		AlignRightNexIMGUItItem(100.f, 8.f);
		if (ImGui::Button("Select", ImVec2(100.f, 30.f)))
		{
			strncpy(drawRotatedTile->texturePathBuff, filesList[selectedFile].path().string().c_str(), IM_ARRAYSIZE(drawRotatedTile->texturePathBuff));
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void CanvasTool::AlignRightNexIMGUItItem(float itemWidth, float borderGap)
{
	auto windowWidth = ImGui::GetWindowSize().x;
	float totalWidth = itemWidth + borderGap;
	ImGui::SetCursorPosX(windowWidth - totalWidth);
}

// Name of the menu that is used to toggle the window.
std::string CanvasTool::GetMenuName()
{
	return "CanvasToolMenu";
}

// Title to give the menu
std::string CanvasTool::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void CanvasTool::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool CanvasTool::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool CanvasTool::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void CanvasTool::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void CanvasTool::OnClose()
{
	isWindowOpen_ = false;
}

std::string CanvasTool::GetPluginName()
{
	return "CanvasTool v1.0";
}

void CanvasTool::RenderSettings()
{
	if (ImGui::Button("Open Menu"))
	{
		gameWrapper->Execute([&](GameWrapper* gw) {
			cvarManager->executeCommand("togglemenu CanvasToolMenu");
			});
	}

	ImGui::Separator();

	ImGui::Text("Set A Bind To Open The Window");
	static char KeyBindInputBuf[200] = "F5";
	ImGui::InputText("##KeyBindInput", KeyBindInputBuf, IM_ARRAYSIZE(KeyBindInputBuf));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowWidth() / 2, ImGui::GetWindowHeight() / 2), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("BindSet", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Open plugin window : %s", std::string(KeyBindInputBuf));
		ImGui::NewLine();

		if (ImGui::Button("OK", ImVec2(100.f, 25.f))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	if (ImGui::Button("Set Bind"))
	{
		gameWrapper->Execute([&](GameWrapper* gw) {
			cvarManager->setBind(KeyBindInputBuf, "togglemenu CanvasToolMenu");
			cvarManager->log("New key bind set : " + std::string(KeyBindInputBuf) + " -> togglemenu CanvasToolMenu");
			});

		ImGui::OpenPopup("BindSet");
	}
	ImGui::Separator();
	ImGui::Text("Plugin made by Vync");
}