#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"


#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class CanvasTool : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginWindow, public BakkesMod::Plugin::PluginSettingsWindow
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	int SelectedIndex = 0;

	//Canvas
	std::vector<std::shared_ptr<CanvasItem>> CanvasItems;
	virtual void RenderCanvas(CanvasWrapper canvas);


	void addItem(std::string itemName);
	void updateTexture(std::shared_ptr<CanvasDrawTexture> drawTexture);
	void updateTexture(std::shared_ptr<CanvasDrawTile> drawTile);
	void updateTexture(std::shared_ptr<CanvasDrawRotatedTile> drawRotatedTile);
	bool FileExists(char pathBuff[100]);
	bool FileIsImage(char pathBuff[100]);
	void removeCanvasItem(int canvasItemindex);
	void MoveLayerUp(int canvasItemindex);
	void MoveLayerDown(int canvasItemindex);
	void Copy(int canvasItemindex);



	// Inherited via PluginWindow
	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "CanvasTool v1.0.1";


	virtual void Render() override;

	virtual void RenderCanvasItems();
	virtual void RenderCanvasItemDetails();

	//Render the differents canvas items details
	virtual void RenderStringDetails(std::shared_ptr<CanvasDrawString> drawString);
	virtual void RenderBoxDetails(std::shared_ptr<CanvasDrawBox> drawBox);
	virtual void RenderFillBoxDetails(std::shared_ptr<CanvasDrawFillBox> drawFillBox);
	virtual void RenderLineDetails(std::shared_ptr<CanvasDrawLine> drawLine);
	virtual void RenderRectDetails(std::shared_ptr<CanvasDrawRect> drawRect);
	virtual void RenderFillTriangleDetails(std::shared_ptr<CanvasDrawFillTriangle> drawFilTriangle);
	virtual void RenderTextureDetails(std::shared_ptr<CanvasDrawTexture> drawTexture);
	virtual void RenderTileDetails(std::shared_ptr<CanvasDrawTile> drawTile);
	virtual void RenderRotatedTileDetails(std::shared_ptr<CanvasDrawRotatedTile> drawRotatedTile);

	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	//Render Settings
	virtual std::string GetPluginName() override;
	virtual void RenderSettings() override;

};

