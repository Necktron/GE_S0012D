#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <typeinfo>

//WIP
class ECS_MSG
{
	//Different message types
	enum MSGType
	{
		MSGT_AssignVariables = 0,
		MSGT_UpdatePos = 1,
		MSGT_UpdateGraphical = 2,
		MSGT_Shutdown = 3
	};

	//What to do when you send a message
	void MSG_Send()
	{

	}

	//What to do when you recieve a message
	void MSG_Recieve()
	{

	}
};

//WIP
class ComponentStruct
{
public:

	int compID;

	virtual void Init()
	{
		//Init the BaseCompClass
	}

	virtual void Update()
	{
		//Update the BaseCompClass
	}

	virtual void Shutdown()
	{
		//Shuts down the BaseCompClass
	}

	virtual void Get()
	{
		//Get value from certain variable
	}

	virtual void Set()
	{
		//Set value of certain variable
	}
};

//WIP
class TransComp : public ComponentStruct
{
public:
	Math::matrix44 transform;

	TransComp()
	{
		this->compID = 1;
	}

	TransComp(Math::matrix44 initTrans)
	{
		transform = initTrans;
		this->compID = 1;
	}

	void Init()
	{
		//Init TransComp, overrides BaseCompClass
	}

	void Update()
	{
		//Init TransComp, overrides BaseCompClass
	}

	void Shutdown()
	{
		//Shutdown TransComp, overrides BaseCompClass
	}

	void Get()
	{
		//Get value from certain variable
	}

	void Set()
	{
		//Set value of certain variable
	}
};

//WIP
class GraphicalComp : public ComponentStruct
{
public:

	Util::StringAtom meshResource;
	Util::StringAtom animResource;
	Util::StringAtom skeletonResource;

	GraphicalComp()
	{
		meshResource = "mdl:Units / Unit_Footman.n3";
		animResource = "ani:Units/Unit_Footman.nax3";
		skeletonResource = "ske:Units/Unit_Footman.nsk3";
		this->compID = 2;
	}

	void Init()
	{
		//Init GraphicalComp, overrides BaseCompClass
	}

	void Update()
	{
		//Init GraphicalComp, overrides BaseCompClass
	}

	void Shutdown()
	{
		//Shutdown GraphicalComp, overrides BaseCompClass
	}

	void Get()
	{
		//Get value from certain variable
	}

	void Set()
	{
		//Set value of certain variable
	}
};

//WIP
class GameEntity
{
//TODO: ADD HASTH TABLE OF VARIABLES FOR ALL COMPONENTS
public:
	Util::StringAtom entName;
	int entID;
	Util::Array<ComponentStruct*> compList;
	bool T_instance;
	bool G_instance;
	Util::StringAtom meshResource;
	Util::StringAtom animResource;
	Util::StringAtom textureResource;
	Util::StringAtom skeletonResource;

	GameEntity(Util::StringAtom name)
	{
		entName = name;
		//entID = 213;
	}

	//Init the GameEntity
	void Init()
	{
		//Init the GameEntity components
		for (int i = 0; i < compList.size(); i++)
		{
			compList[i]->Init();
		}
	}

	//Update the GameEntity components
	void Update()
	{
		//Update the GameEntity components
		for (int i = 0; i < compList.size(); i++)
		{
			compList[i]->Update();
		}
	}

	//Shuts down the GameEntity
	void Shutdown()
	{
		//Shuts down the GameEntity components
		for (int i = 0; i < compList.size(); i++)
		{
			compList[i]->Shutdown();
		}
	}

	//Add Comp to array
	void AddComp(Util::StringAtom compToAdd)
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

	//Del Comp to array
	void DelComp(Util::StringAtom compToDel)
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
};

//WIP
class EntityManager
{
public:
	Util::Array<GameEntity*> entities;

	//Init the GameEntity
	void Init()
	{
		//Init the EntityManagers enteties
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->Init();
		}
	}

	//Update the EntityManagers enteties
	void Update()
	{
		//Update the EntityManagers enteties
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->Update();
		}
	}

	//Shuts down the GameEntity
	void Shutdown()
	{
		//Shuts down the EntityManagers enteties
		for (int i = 0; i < entities.size(); i++)
		{
			entities[i]->Shutdown();
		}
	}

	//WIP
	GameEntity* FindEnt(Util::StringAtom entitySearch)
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

	//Add entity to array
	bool AddEnt(Util::StringAtom entityName)
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

	//Del Comp to array
	void DelEnt(Util::StringAtom entToDel)
	{
		for (int i = 0; i < entities.size(); i++)
		{
			//If we find the entity in the list, delete it and resize the list
			if (entToDel == entities[i]->entName)
			{
				entities.Erase(&entities[i]);
				n_printf("Entity has been found and deleted!");
				return;
			}
		}

		n_printf("ERROR: Entity was not found!");
	}
};

//namespace std