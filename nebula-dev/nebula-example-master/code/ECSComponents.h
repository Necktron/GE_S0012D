#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H
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
#include "ECSMessage.h"
#include "ECSManager.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

class EntityManager;

class ComponentCore : public Core::RefCounted
{
	__DeclareClass(Core::RefCounted);

public:
	//friend Core::RefCounted& friendRef(ComponentCore&);

	int compID;
	Util::StringAtom parentRef;
	EntityManager* managerInst;
	Graphics::GraphicsEntityId GEID = NULL;
	virtual void Init(Graphics::GraphicsEntityId GEIDREF, bool reg);
	virtual void Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg);
	virtual void Update();
	virtual void Shutdown();
	virtual void MSGRecieve(ECSMSG::ECSMSGTypes msg);
};

//WIP
class TransComp : public ComponentCore
{
	__DeclareClass(Core::RefCounted);

public:
	Math::matrix44 transform;
	float movePosX = 0;
	float movePosY = 0;
	float movePosZ = 0;
	float xVal = 0;
	float yVal = 0;
	float zVal = 0;

	TransComp(Util::StringAtom parent);
	~TransComp();
	void Init(Graphics::GraphicsEntityId GEIDREF, bool reg);
	void Update();
	void Shutdown();
	void MSGRecieve(ECSMSG::ECSMSGTypes msg);
	void ForwardWalk();
	void BackWalk();
	void RightWalk();
	void LeftWalk();
};

//WIP
class GraphicalComp : public ComponentCore
{
	__DeclareClass(Core::RefCounted);

public:
	Util::StringAtom meshResource = nullptr;
	Util::StringAtom animResource = nullptr;
	Util::StringAtom skeletonResource = nullptr;

	GraphicalComp(Util::StringAtom parent);
	~GraphicalComp();
	void Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg);
	void Update();
	void Shutdown();
	void MSGRecieve(ECSMSG::ECSMSGTypes msg);
};
#endif