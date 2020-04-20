#include "foundation/stdneb.h"
#include "ECSManager.h"

__ImplementSingleton(EntityManager);

EntityManager::EntityManager()
{

}

void EntityManager::Init()
{
	if (managerInstance == nullptr)
		managerInstance = &managerInstance->getInstance();

	//Init the EntityManagers enteties
	for (int i = 0; i < this->entities.size(); i++)
	{
		managerInstance->entities[i]->Init();
	}
}

void EntityManager::Update()
{
	//Update the EntityManagers enteties
	for (int i = 0; i < this->managerInstance->entities.size(); i++)
	{
		managerInstance->entities[i]->Update();
	}
}

void EntityManager::Shutdown()
{
	//Shuts down the EntityManagers enteties
	while (managerInstance->entities.size() > 0)
	{
		managerInstance->entities[0]->Shutdown();
		managerInstance->entities[0] = nullptr;
		managerInstance->entities.EraseFront();
	}
}

GameEntity* EntityManager::FindEnt(Util::StringAtom entitySearch)
{
	if (managerInstance == nullptr)
		managerInstance = &managerInstance->getInstance();

	//Search entity by name
	for (int i = 0; i < managerInstance->entities.size(); i++)
	{
		if (entitySearch == managerInstance->entities[i]->entName)
		{
			n_printf("We found the entity with the name %s\n", managerInstance->entities[i]->entName);
			return managerInstance->entities[i];
		}
	}

	if (managerInstance->entities.size() == 0)
	{
		n_printf("No entities exsist...\n");
		return nullptr;
	}

	n_printf("Could not find the requested entity. It's avalible for adding or is missing\n");
	return 0;
}

bool EntityManager::AddEnt(Util::StringAtom entityName)
{
	if (managerInstance == nullptr)
		managerInstance = &managerInstance->getInstance();

	if (FindEnt(entityName)->entName != 0)
	{
		n_printf("ERROR: Entity is already exsisting\n");
		return false;
	}

	n_printf("No duplicates found, entity has been added!\n");
	GameEntity* newEnt = GameEntity::Create();
	managerInstance->entities.Append(newEnt);
	managerInstance->entities[managerInstance->entities.size()]->entName = entityName;
	managerInstance->entities[managerInstance->entities.size()]->lo.loadoutID = 1;
	managerInstance->entCount++;
	return true;
}

bool EntityManager::AddEnt(Util::StringAtom entityName, int loadout)
{
	if (managerInstance == nullptr)
		managerInstance = &managerInstance->getInstance();

	if (FindEnt(entityName) != nullptr)
	{
		if (FindEnt(entityName)->entName != 0)
		{
			n_printf("ERROR: Entity is already exsisting\n");
			return false;
		}
	}

	//const Util::StringAtom newEnt = "GameEntity";
	GameEntity* newEnt = GameEntity::Create();

	n_printf("No duplicates found, entity has been added!\n");
	managerInstance->entities.Append(newEnt);
	managerInstance->entities[managerInstance->entities.size() - 1]->entName = entityName;
	managerInstance->entities[managerInstance->entities.size() - 1]->entID = managerInstance->entities.size() - 1;
	managerInstance->entities[managerInstance->entities.size() - 1]->lo.loadoutID = loadout;
	managerInstance->entCount++;
	return true;
}

void EntityManager::DelEnt(Util::StringAtom entToDel)
{
	for (int i = 0; i < managerInstance->entities.size(); i++)
	{
		//If we find the entity in the list, delete it and resize the list
		if (entToDel == managerInstance->entities[i]->entName)
		{
			for(int i = 0; i < managerInstance->entities.size(); i++)
				managerInstance->entities.Erase(&managerInstance->entities[i]);
			n_printf("Entity has been found and deleted!");
			managerInstance->entCount--;
			return;
		}
	}

	n_printf("ERROR: Entity was not found!");
}