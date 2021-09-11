#pragma once
#include "AUIScreen.h"
#include <vector>
class DebugConsoleScreen : AUIScreen
{
private:
	DebugConsoleScreen();
	~DebugConsoleScreen();

	virtual void drawUI() override;
public:
	void addToList(std::string text);
private:
	friend class UIManager;
	std::vector<std::string> printList;
};
