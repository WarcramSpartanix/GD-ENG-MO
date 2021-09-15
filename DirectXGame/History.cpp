#include "History.h"
#include "Storage.h"
#include "EngineBackend.h"

History* History::sharedInstance = nullptr;

History* History::getInstance()
{
	return sharedInstance;
}

void History::initialize()
{
	sharedInstance = new History();
}

void History::destroy()
{
	delete sharedInstance;
}

void History::recordHistory(AGameObject* agameObject)
{
	if (EngineBackend::getInstance()->getMode() == EngineBackend::EditorMode::EDITOR)
	{
		std::cout << "hi";
		Storage* storage = new Storage(agameObject);
		this->actionsPerformed.push(storage);
	}
}

void History::recordUndoHistory(AGameObject* agameObject)
{
	Storage* storage = new Storage(agameObject);
	this->actionsCancelled.push(storage);
}

bool History::hasRemainingUndoActions()
{
	return !this->actionsPerformed.empty();
}

bool History::hasRemainingRedoActions()
{
	return !this->actionsCancelled.empty();
}

Storage* History::undoAction()
{
	if (EngineBackend::getInstance()->getMode() == EngineBackend::EditorMode::EDITOR)
	{
		if (this->hasRemainingUndoActions())
		{
			Storage* storage = this->actionsPerformed.top();
			this->actionsPerformed.pop();
			return storage;
		}
	}
	return nullptr;
}

Storage* History::redoAction()
{
	if (EngineBackend::getInstance()->getMode() == EngineBackend::EditorMode::EDITOR)
	{
		if (this->hasRemainingRedoActions())
		{
			Storage* storage = this->actionsCancelled.top();
			this->actionsCancelled.pop();
			return storage;
		}
	}
	return nullptr;
}

History::History()
{
}

History::~History()
{
}
