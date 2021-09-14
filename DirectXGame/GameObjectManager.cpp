#include "GameObjectManager.h"
#include "AGameObject.h"

GameObjectManager* GameObjectManager::sharedInstance = nullptr;

GameObjectManager* GameObjectManager::getInstance()
{
    if (sharedInstance == nullptr)
        GameObjectManager::initialize();
    return sharedInstance;
}

void GameObjectManager::initialize()
{
    sharedInstance = new GameObjectManager();
    sharedInstance->init();
}

void GameObjectManager::destroy()
{
    if (sharedInstance != nullptr)
        sharedInstance->release();
}

void GameObjectManager::addGameObject(AGameObject* gameObject)
{
    if (findObjectByName(gameObject->getName()) != nullptr)
    {
        int count = 1;
        std::string newName = gameObject->getName() + "(" + std::to_string(count) + ")";
        while (findObjectByName(newName) != nullptr)
        {
            count++;
            newName = gameObject->getName() + "(" + std::to_string(count) + ")";
        }
        gameObject->name = newName;
    }

    gameObjectList.push_back(gameObject);
    gameObjectNames.push_back(gameObject->getName());
    gameObjectMap[gameObject->getName()] = gameObject;
}

void GameObjectManager::updateAllGameObjects(float deltaTime)
{
    for (int i = 0; i < gameObjectList.size(); i++)
    {
        gameObjectList[i]->update(deltaTime);
    }
}

void GameObjectManager::drawAllGameObjects(ConstantBuffer* cb)
{
    for (int i = 0; i < gameObjectList.size(); i++)
    {
        gameObjectList[i]->draw(cb);
    }
}

std::vector<std::string> GameObjectManager::getGameObjectNames()
{
    return gameObjectNames;
}

void GameObjectManager::selectObject(int index)
{
    selectedObject = gameObjectList[index];
}

void GameObjectManager::selectObject(AGameObject* gameObject)
{
    selectedObject = gameObject;
}

AGameObject* GameObjectManager::getSelectedObject()
{
    return selectedObject;
}

AGameObject* GameObjectManager::findObjectByName(std::string name)
{
    if (gameObjectMap[name] != NULL) {
        return gameObjectMap[name];
    }

    return nullptr;
}

void GameObjectManager::deleteObject(AGameObject* gameObject)
{
    gameObjectMap.erase(gameObject->getName());

    for (int i = 0; i < gameObjectList.size(); i++) {
        if (gameObjectList[i] == gameObject) {
            gameObjectList.erase(gameObjectList.begin() + i);
            break;
        }
    }
    for (int i = 0; i < gameObjectNames.size(); i++)
    {
        if (std::strcmp(gameObjectNames[i].c_str(), gameObject->getName().c_str()) == 0)
        {
            gameObjectNames.erase(gameObjectNames.begin() + i);
            break;
        }
    }

    //if (EngineBackend::getInstance()->getMode() == EngineMode::Editor)
    //    delete gameObject;
}

void GameObjectManager::deleteObject(std::string name)
{
    AGameObject* object = findObjectByName(name);

    if (object != NULL) {
        if (object == selectedObject)
            selectedObject = nullptr;
        deleteObject(object);
    }
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::init()
{
}

void GameObjectManager::release()
{
    for (int i = 0; i < gameObjectList.size(); i++)
    {
        delete gameObjectList[i];
    }
    gameObjectList.clear();
}
