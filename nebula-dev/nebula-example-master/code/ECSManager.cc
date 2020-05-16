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

	//If we were to add new entities after a Shutdown, we need to remember what index was used as ID
	//If we skip this we will get errors since it's already allocated with the GEID on GraphicsSetup in Nebula
	if (managerInstance->entCount == 1 && managerInstance->lastID != 0)
	{
		managerInstance->entities[0]->entID = managerInstance->lastID + 1;
	}

	//Make sure all entity ID's are unique while multiple entities exsist
	if (managerInstance->entities.size() > 1)
	{
		for (int i = 1; i < this->entities.size(); i++)
		{
			//If we found a entID duplicate, loop from the beginning untill a free ID is found and set that one
			if (managerInstance->entities[i]->entID == managerInstance->entities[i - 1]->entID)
			{
				n_printf("ERROR: We found a duplicate entID, solving problem!\n");
				managerInstance->entities[i]->entID++;
			}
		}
	}

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
	managerInstance->lastID = managerInstance->entities[managerInstance->entities.size() - 1]->entID;

	while (managerInstance->entities.size() > 0)
	{
		DelEnt(managerInstance->entities[0]->entName);
	}

	managerInstance->entCount = 0;
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
		return 0;
	}

	n_printf("Could not find the requested entity. It's avalible for adding or is missing\n");
	return 0;
}

void EntityManager::AddEnt(Util::StringAtom entityName)
{
	if (managerInstance == nullptr)
		managerInstance = &managerInstance->getInstance();

	if (FindEnt(entityName) != 0)
	{
		n_printf("ERROR: Entity is already exsisting\n");
	}

	else
	{
		n_printf("No duplicates found, entity has been added!\n");
		GameEntity* newEnt = GameEntity::Create();
		managerInstance->entities.Append(newEnt);
		managerInstance->entities[managerInstance->entities.size()]->entName = entityName;
		managerInstance->entities[managerInstance->entities.size() - 1]->entID = managerInstance->entities.size() - 1;
		managerInstance->entities[managerInstance->entities.size()]->lo.loadoutID = 1;
		managerInstance->entCount++;
	}
}

void EntityManager::AddEnt(Util::StringAtom entityName, int loadout)
{
	if (managerInstance == nullptr)
		managerInstance = &managerInstance->getInstance();

	if (FindEnt(entityName) != 0)
	{
		n_printf("ERROR: Entity is already exsisting\n");
	}

	else
	{
		n_printf("No duplicates found, entity has been added!\n");
		GameEntity* newEnt = GameEntity::Create();
		managerInstance->entities.Append(newEnt);
		managerInstance->entities[managerInstance->entities.size() - 1]->entName = entityName;
		managerInstance->entities[managerInstance->entities.size() - 1]->entID = managerInstance->entities.size() - 1;
		managerInstance->entities[managerInstance->entities.size() - 1]->lo.loadoutID = loadout;
		managerInstance->entCount++;
	}
}

void EntityManager::DelEnt(Util::StringAtom entToDel)
{
	for (int i = 0; i < managerInstance->entities.size(); i++)
	{
		//Check if we can find the entity in the list
		if (managerInstance->entities[i]->entName == entToDel)
		{
			//If the entity has a transform component, simply set the Y value to something like 99999999 and get it out of here
			if (managerInstance->entities[i]->FindComp("TransformComp"))
			{
				managerInstance->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::DeleteEnt);
			}

			n_printf("Old entity count: %i\n", managerInstance->entities.size());

			//Loop through the list of entities and take away the entity at i so it won't be affected by any commands or updates
			for (int j = i; j < managerInstance->entities.size() - 1; j++)
			{
				managerInstance->entities[j] = managerInstance->entities[j + 1];
			}

			managerInstance->entities[managerInstance->entities.size() - 1] = {};
			managerInstance->entities.resize(managerInstance->entities.size() - 1);

			n_printf("Entity has been found and deleted!\n");

			n_printf("New entity count: %i\n", managerInstance->entities.size());
			managerInstance->entCount--;
			return;
		}
	}

	n_printf("ERROR: Entity was not found!");
}