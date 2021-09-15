#pragma once
#include <iostream>
#include <stack>

class Storage;
class AGameObject;
class History
{
public:
	static History* getInstance();
	static void initialize();
	static void destroy();

	void recordHistory(AGameObject* agameObject);
	void recordUndoHistory(AGameObject* agameObject);
	bool hasRemainingUndoActions();
	bool hasRemainingRedoActions();
	Storage* undoAction();
	Storage* redoAction();

private:
	History();
	~History();
	History(History const&) {};
	History& operator = (History const&) {};
	static History* sharedInstance;

	std::stack<Storage*> actionsPerformed;
	std::stack<Storage*> actionsCancelled;
};

