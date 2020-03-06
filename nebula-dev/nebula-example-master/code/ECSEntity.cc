#include "ECSEntity.h"

GameEntity::GameEntity(Util::StringAtom name)
{
	entName = name;
}

GameEntity::GameEntity(Util::StringAtom name, Resources::ResourceName mesh, Resources::ResourceName anim, Resources::ResourceName skel)
{
	entName = name;
	meshResource = mesh;
	animResource = anim;
	skeletonResource = skel;
}

void GameEntity::Init()
{
	GEID = Graphics::CreateEntity();
	Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(GEID);

	//Init the GameEntity components
	for (int i = 0; i < compList.size(); i++)
	{
		if (compList[i]->compID == 1)
			compList[i]->Init(GEID);

		if (compList[i]->compID == 2)
			compList[i]->Init(GEID, meshResource, animResource, skeletonResource);
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
	for (int i = 0; i < compList.size(); i++)
	{
		compList[i]->Shutdown();
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