#pragma once
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <iostream>
#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <typeinfo>
#include "stdneb.h"
#include "core/refcounted.h"
#include "timing/timer.h"
#include "io/console.h"
#include "visibility/visibilitycontext.h"
#include "models/streammodelpool.h"
#include "models/modelcontext.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "exampleapp.h"
#include "math/vector.h"
#include "math/point.h"
#include "dynui/imguicontext.h"
#include "lighting/lightcontext.h"
#include "characters/charactercontext.h"
#include "imgui.h"
#include "dynui/im3d/im3dcontext.h"
#include "dynui/im3d/im3d.h"
#include "graphics/environmentcontext.h"
#include "clustering/clustercontext.h"
#include "io/debug/iopagehandler.h"
#include "io/logfileconsolehandler.h"
#include "io/debug/consolepagehandler.h"
#include "core/debug/corepagehandler.h"
#include "threading/debug/threadpagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "io/debug/iopagehandler.h"
#include "io/fswrapper.h"
#include "system/nebulasettings.h"
#include "profiling/profiling.h"
#include "core/factory.h"
#include "ECSEntity.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

class GameEntity;

class EntityManager
{
	__DeclareSingleton(EntityManager);

public:

	static EntityManager& getInstance()
	{
		static EntityManager instance;
		return instance;
	}

	Util::Array<GameEntity*> entities;

	EntityManager* managerInstance;
	EntityManager();
	void Init();
	void Update();
	void Shutdown();
	GameEntity* FindEnt(Util::StringAtom entitySearch);
	bool AddEnt(Util::StringAtom entityName);
	bool AddEnt(Util::StringAtom entityName, int loadout);
	void DelEnt(Util::StringAtom entToDel);
};
//namespace std
#endif