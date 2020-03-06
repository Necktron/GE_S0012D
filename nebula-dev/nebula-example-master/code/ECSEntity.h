#pragma once
#ifndef GAMEENTITY_H
#define GAMEENTITY_H
#endif

#include <iostream>
#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <typeinfo>
#include "ECSComponents.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

class GameEntity
{
	//TODO: ADD HASTH TABLE OF VARIABLES FOR ALL COMPONENTS
public:
	Util::StringAtom entName;
	int entID;
	Util::Array<ComponentCore*> compList;
	bool T_instance;
	bool G_instance;
	Resources::ResourceName meshResource;
	Resources::ResourceName animResource;
	Resources::ResourceName skeletonResource;
	Graphics::GraphicsEntityId GEID;

	GameEntity(Util::StringAtom name);

	GameEntity(Util::StringAtom name, Resources::ResourceName mesh, Resources::ResourceName anim, Resources::ResourceName skel);

	//Init the GameEntity
	void Init();

	//Update the GameEntity components
	void Update();

	//Shuts down the GameEntity
	void Shutdown();

	ComponentCore* FindComp(Util::StringAtom compSearch);

	//Add Comp to array
	void AddComp(Util::StringAtom compToAdd);

	//Del Comp to array
	void DelComp(Util::StringAtom compToDel);
};