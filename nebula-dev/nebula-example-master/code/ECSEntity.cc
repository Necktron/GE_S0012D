#include "ECSEntity.h"

GameEntity::GameEntity(Util::StringAtom name)
{
	entName = name;
}

GameEntity::GameEntity(Util::StringAtom name, GameEntity::Models loadout)
{
	entName = name;

	if (loadout == 0)
	{
		meshResource = "mdl:Units/Unit_King.n3";
		animResource = "ani:Units/Unit_King.nax3";
		skeletonResource = "ske:Units/Unit_King.nsk3";
	}

	else if (loadout == 1)
	{
		meshResource = "mdl:Units/Unit_Footman.n3";
		animResource = "ani:Units/Unit_Footman.nax3";
		skeletonResource = "ske:Units/Unit_Footman.nsk3";
	}
}

GameEntity::~GameEntity()
{
	compList = NULL;
}

void GameEntity::Init()
{
	GEID = Graphics::CreateEntity();

	//Init the GameEntity components
	for (int i = 0; i < compList.size(); i++)
	{
		if (compList[i]->compID == 1)
			compList[i]->Init(GEID, this->registered);

		if (compList[i]->compID == 2)
			compList[i]->Init(GEID, meshResource, animResource, skeletonResource, this->registered);
		
		this->registered = true;
	}
}

void GameEntity::Update()
{
	//Update the GameEntity components
	for (int i = 0; i < compList.size(); i++)
	{
		compList[i]->Update();
	}
}

void GameEntity::Shutdown()
{
	//Shuts down the GameEntity components
	while (compList.size() > 0)
	{
		compList[0]->Shutdown();
		compList[0] = NULL;
		compList.EraseFront();
	}

	this->~GameEntity();
	delete this;
}

void GameEntity::MSGSend(ComponentCore* recieverComp, ECSMSG::ECSMSGTypes msg)
{
	//Init the EntityManagers enteties
	for (int i = 0; i < compList.size(); i++)
	{
		if (compList[i]->compID == recieverComp->compID)
		{
			recieverComp->MSGRecieve(msg);
			n_printf("We have sen't a message from a GameEntity");
			break;
		}
	}
}

ComponentCore* GameEntity::FindComp(Util::StringAtom compSearch)
{
	//If we find the comp in the list, delete it and resize the list
	for (int i = 0; i < compList.size(); i++)
	{
		//If it's a transform comp
		if (T_instance == true && compSearch == "TransformComp" && compList[i]->compID == 1)
		{
			return compList[i];
		}

		//If it's a graphics comp
		else if (G_instance == true && compSearch == "GraphicalComp" && compList[i]->compID == 2)
		{
			return compList[i];
		}
	}

	n_printf("Could not find the requested components. It's avalible for adding or is missing\n");
	return 0;
}

void GameEntity::AddComp(Util::StringAtom compToAdd)
{
	//If it's a transform comp
	if (T_instance == false && compToAdd == "TransformComp")
	{
		TransComp* newTComp = new TransComp();
		compList.Append(newTComp);
		T_instance = true;
	}

	//If it's a graphics comp
	else if (G_instance == false && compToAdd == "GraphicalComp")
	{
		GraphicalComp* newGComp = new GraphicalComp();
		compList.Append(newGComp);
		G_instance = true;
	}

	else
	{
		n_printf("This component is already active! Can't have duplicates of component types!\n");
	}
}

void GameEntity::DelComp(Util::StringAtom compToDel)
{
	//If we find the comp in the list, delete it and resize the list
	for (int i = 0; i < compList.size(); i++)
	{
		//If it's a transform comp
		if (T_instance == true && compToDel == "TransformComp" && compList[i]->compID == 1)
		{
			compList.Erase(&compList[i]);
			T_instance = false;
		}

		//If it's a graphics comp
		else if (G_instance == true && compToDel == "GraphicalComp" && compList[i]->compID == 2)
		{
			compList.Erase(&compList[i]);
			G_instance = false;
		}

		else
		{
			n_printf("The component was not found, try again!\n");
		}
	}
}