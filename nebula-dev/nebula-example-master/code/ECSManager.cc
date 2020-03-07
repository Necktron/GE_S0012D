#include "ECSManager.h"

void EntityManager::Init()
{
	//Init the EntityManagers enteties
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Init();
	}
}

void EntityManager::Update()
{
	//Update the EntityManagers enteties
	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update();
	}
}

void EntityManager::Shutdown()
{
	//Shuts down the EntityManagers enteties
	while (entities.size() > 0)
	{
		entities[0]->Shutdown();
		entities[0] = NULL;
		entities.EraseFront();
	}

	delete this;
}

GameEntity* EntityManager::FindEnt(Util::StringAtom entitySearch)
{
	//Search entity by name
	for (int i = 0; i < entities.size(); i++)
	{
		if (entitySearch == entities[i]->entName)
		{
			n_printf("We found the entity with the name %s\n", entities[i]->entName);
			return entities[i];
		}
	}

	n_printf("Could not find the requested entity. It's avalible for adding or is missing\n");
	return 0;
}

bool EntityManager::AddEnt(Util::StringAtom entityName)
{
	if (FindEnt(entityName) != 0)
	{
		n_printf("ERROR: Entity is already exsisting\n");
		return false;
	}

	n_printf("No duplicates found, entity has been added!\n");
	entities.Append(new GameEntity(entityName));
	return 1;
}

bool EntityManager::AddEnt(Util::StringAtom entityName, GameEntity::Models loadout)
{
	if (FindEnt(entityName) != 0)
	{
		n_printf("ERROR: Entity is already exsisting\n");
		return false;
	}

	n_printf("No duplicates found, entity has been added!\n");
	entities.Append(new GameEntity(entityName, loadout));
	return 1;
}

void EntityManager::DelEnt(Util::StringAtom entToDel)
{
	for (int i = 0; i < entities.size(); i++)
	{
		//If we find the entity in the list, delete it and resize the list
		if (entToDel == entities[i]->entName)
		{
			for(int i = 0; i < entities.size(); i++)
			entities.Erase(&entities[i]);
			n_printf("Entity has been found and deleted!");
			return;
		}
	}

	n_printf("ERROR: Entity was not found!");
}