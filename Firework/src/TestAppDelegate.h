#pragma once

class TestAppDelegate : public Core::EngineAppDelegate {
public:
	TestAppDelegate();

	virtual void GameContentSize(int deviceWidth, int deviceHeight, int &width, int &height) override;
	virtual void ScreenMode(DeviceMode &mode) override;

	virtual void RegisterTypes() override;
	virtual void LoadResources() override;

	virtual void OnPostDraw() override;
};