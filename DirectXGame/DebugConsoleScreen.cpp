#include "DebugConsoleScreen.h"

#include "imGUI/imgui.h"


DebugConsoleScreen::DebugConsoleScreen() :AUIScreen("Debug Console")
{
}

DebugConsoleScreen::~DebugConsoleScreen()
{
}

void DebugConsoleScreen::drawUI()
{
	ImGui::Begin("Debug Window");
	ImGui::SetWindowSize(ImVec2(500, 100));
	for (int i = 0; i < printList.size(); i++)
	{
		ImGui::Text("%s", printList[i].c_str());
	}
	ImGui::End();
}

void DebugConsoleScreen::addToList(std::string text)
{
	printList.push_back(text + "\n");
}
