#include "foundation/stdneb.h"
#include "ECSEntity.h"

__ImplementClass(GameEntity, '1234', Core::RefCounted);

GameEntity::GameEntity()
{

}

GameEntity::GameEntity(Util::StringAtom name)
{

}

GameEntity::GameEntity(Util::StringAtom name, int loadout)
{

}

GameEntity::~GameEntity()
{
	compList = NULL;
}

void GameEntity::Init()
{
	CompVar test = Graphics::GraphicsEntityId();
	test.vGEIDref = this->entID;

	AddCompVar("GEID", test);

	if (this->lo.loadoutID == 0)
	{
		this->lo.loMesh = "mdl:Units/Unit_King.n3";
		this->lo.loAnim = "ani:Units/Unit_King.nax3";
		this->lo.loSkel = "ske:Units/Unit_King.nsk3";
	}

	else if (this->lo.loadoutID == 1)
	{
		this->lo.loMesh = "mdl:Units/Unit_Footman.n3";
		this->lo.loAnim = "ani:Units/Unit_Footman.nax3";
		this->lo.loSkel = "ske:Units/Unit_Footman.nsk3";
	}

	else if (this->lo.loadoutID == 2)
	{
		this->lo.loMesh = "mdl:Units/Unit_Spearman.n3";
		this->lo.loAnim = "ani:Units/Unit_Spearman.nax3";
		this->lo.loSkel = "ske:Units/Unit_Spearman.nsk3";
	}

	//Init the GameEntity components
	for (int i = 0; i < compList.size(); i++)
	{
		if (compList[i]->compID == 1)
			compList[i]->Init(GetVar("GEID")->vGEIDref, this->registered);

		else if (compList[i]->compID == 2)
			compList[i]->Init(GetVar("GEID")->vGEIDref, this->lo.loMesh.Value(), this->lo.loAnim.Value(), this->lo.loSkel.Value(), this->registered);

		this->registered = true;
	}
}

void GameEntity::Update()
{
	//Update the GameEntity components
	for (int i = 0; i < compList.size(); i++)
		compList[i]->Update();
}

void GameEntity::Shutdown()
{
	//Shuts down the GameEntity components
	while (compList.size() > 0)
	{
		compList[0]->Shutdown();
		compList[0] = nullptr;
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
			return compList[i];

		//If it's a graphics comp
		else if (G_instance == true && compSearch == "GraphicalComp" && compList[i]->compID == 2)
			return compList[i];
	}

	n_printf("Could not find the requested components. It's avalible for adding or is missing\n");
	return nullptr;
}

void GameEntity::AddCompVar(Util::StringAtom key, CompVar value)
{
	if (this->varLibrary.Contains(key))
		n_printf("This key already exsists! Can't add pair value!");

	else
		switch (value.data)
		{
		case CompVar::cvInt:
			this->varLibrary.Add(key, value.vIntNum);
			break;

		case CompVar::cvFloat:
			this->varLibrary.Add(key, value.vFloatNum);
			break;

		case CompVar::cvMatrix:
			this->varLibrary.Add(key, value.vMatrix);
			break;

		case CompVar::cvStringAtom:
			this->varLibrary.Add(key, value.vStrAtom);
			break;

		case CompVar::cvGEID:
			this->varLibrary.Add(key, value.vGEIDref);
			break;
		}
}

void GameEntity::SetVar(Util::StringAtom varName, CompVar newValue)
{
	if (this->varLibrary.Contains(varName))
	{
		n_printf("This key exsists! We can now change the value of it!");

		switch (this->varLibrary[varName].data)
		{
			case CompVar::cvInt:
				this->varLibrary[varName] = newValue.vIntNum;
				break;

			case CompVar::cvFloat:
				this->varLibrary[varName] = newValue.vFloatNum;
				break;

			case CompVar::cvMatrix:
				this->varLibrary[varName] = newValue.vMatrix;
				break;

			case CompVar::cvStringAtom:
				this->varLibrary[varName] = newValue.vStrAtom;
				break;

			case CompVar::cvGEID:
				this->varLibrary[varName] = newValue.vGEIDref;
				break;
		}
	}

	n_printf("This key does not exsist! We can't set the value!");
}

GameEntity::CompVar* GameEntity::GetVar(Util::StringAtom varName)
{
	if (this->varLibrary.Contains(varName))
	{
		n_printf("This key exsists! We can now retrieve the value of it!");

		return &this->varLibrary[varName];
	}

	n_printf("This key does not exsist! We can't retrieve the value!");
	return nullptr;
}

void GameEntity::AddComp(Util::StringAtom compToAdd)
{
	//If it's a transform comp
	if (T_instance == false && compToAdd == "TransformComp")
	{
		TransComp* newTComp = TransComp::Create();
		compList.Append(newTComp);
		compList[compList.size() - 1]->parentNameRef = this->entName;
		compList[compList.size() - 1]->parentIDRef = this->entID;
		compList[compList.size() - 1]->compID = 1;

		T_instance = true;
	}

	//If it's a graphics comp
	else if (G_instance == false && compToAdd == "GraphicalComp")
	{
		GraphicalComp* newGComp = GraphicalComp::Create();
		compList.Append(newGComp);
		compList[compList.size() - 1]->parentNameRef = this->entName;
		compList[compList.size() - 1]->parentIDRef = this->entID;
		compList[compList.size() - 1]->compID = 2;

		G_instance = true;
	}

	else
		n_printf("This component is already active! Can't have duplicates of component types!\n");
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
			n_printf("The component was not found, try again!\n");
	}
}

//Comp stuff
GameEntity::CompVar::CompVar()
{

};

GameEntity::CompVar::CompVar(int vInt)
{
	this->vIntNum = vInt;
	data = cvInt;
};

GameEntity::CompVar::CompVar(float vFloat)
{
	this->vFloatNum = vFloat;
	data = cvFloat;
};

GameEntity::CompVar::CompVar(Math::matrix44* vMat)
{
	this->vMatrix = vMat;
	data = cvMatrix;
};

GameEntity::CompVar::CompVar(Util::StringAtom* vStrAtom)
{
	this->vStrAtom = vStrAtom;
	data = cvStringAtom;
};

GameEntity::CompVar::CompVar(Graphics::GraphicsEntityId vGEID)
{
	this->vGEIDref = vGEID;
	data = cvGEID;
};

GameEntity::CompVar::CompVar(const CompVar& ref)
{
	this->Copy(ref);
};

GameEntity::CompVar::~CompVar()
{
	//this->Delete();
}

void GameEntity::CompVar::Delete()
{
	if (this->data == cvMatrix)
	{
		n_delete(this->vMatrix);
		this->vMatrix = 0;
	}

	else if (this->data == cvStringAtom)
	{
		n_assert(this->vStrAtom);
		n_delete(this->vStrAtom);
		this->vStrAtom = 0;
	}
}

void GameEntity::CompVar::Copy(const CompVar& cv)
{
	this->data = cv.data;

	switch (cv.data)
	{
		case cvInt:
			this->vIntNum = cv.vIntNum;
			break;

		case cvFloat:
			this->vFloatNum = cv.vFloatNum;
			break;

		case cvMatrix:
			this->vMatrix = n_new(Math::matrix44);
			this->vMatrix = cv.vMatrix;
			break;

		case cvStringAtom:
			this->vStrAtom = n_new(Util::StringAtom);
			this->vStrAtom = cv.vStrAtom;
			break;

		case cvGEID:
			this->vGEIDref = cv.vGEIDref;
			break;
	}
};

void GameEntity::CompVar::operator=(const CompVar& type)
{
	//this->Delete();
	this->Copy(type);
};