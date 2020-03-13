#pragma once
#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <iostream>
#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <typeinfo>
#include "ECSComponents.h"
#include "ECSMessage.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;


class ComponentCore;

class GameEntity : public Core::RefCounted
{
	__DeclareClass(GameEntity);

public:
	struct Loadout
	{
		int loadoutID;
		Util::StringAtom loMesh;
		Util::StringAtom loAnim;
		Util::StringAtom loSkel;
	};

	struct CompVar
	{

	public:
		enum Data
		{
			cvInt = 0,
			cvFloat = 1,
			cvMatrix = 2,
			cvStringAtom = 3,
			cvGEID = 4,
		};

		Data data;

		union
		{
			int vIntNum = 0;
			float vFloatNum;
			Util::StringAtom* vStrAtom;
			Math::matrix44* vMatrix;
			Graphics::GraphicsEntityId vGEIDref;
		};

		CompVar();
		CompVar(int vInt);
		CompVar(float vFloat);
		CompVar(Math::matrix44* vMat);
		CompVar(Util::StringAtom* vStrAtom);
		CompVar(Graphics::GraphicsEntityId vGEID);
		CompVar(const CompVar& ref);
		~CompVar();

		void Delete();
		void Copy(const CompVar& cv);
		void operator=(const CompVar& type);
	};

	Util::StringAtom entName;
	int entID;
	Util::Array<ComponentCore*> compList;
	bool T_instance = false;
	bool G_instance = false;
	bool registered = false;
	CompVar cv;
	Loadout lo;
	Util::Dictionary<Util::StringAtom, CompVar> varLibrary;

	enum Models
	{
		King = 0,
		Footman = 1,
		Spearman = 2
	};


	GameEntity();
	GameEntity(Util::StringAtom name);
	GameEntity(Util::StringAtom name, int loadout);
	~GameEntity();

	//Init the GameEntity
	void Init();

	//Update the GameEntity components
	void Update();

	//Shuts down the GameEntity
	void Shutdown();

	void MSGSend(ComponentCore* recieverComp, ECSMSG::ECSMSGTypes msg);

	ComponentCore* FindComp(Util::StringAtom compSearch);

	void AddCompVar(Util::StringAtom key, CompVar value);

	void SetVar(Util::StringAtom varName, CompVar newValue);

	CompVar* GetVar(Util::StringAtom varName);

	//Add Comp to array
	void AddComp(Util::StringAtom compToAdd);

	//Del Comp to array
	void DelComp(Util::StringAtom compToDel);
};
#endif