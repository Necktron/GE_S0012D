#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H
#endif
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

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

class ComponentCore
{
public:
	int compID;
	Graphics::GraphicsEntityId GEID = NULL;

	virtual void Init(Graphics::GraphicsEntityId GEIDREF);
	virtual void Init(Graphics::GraphicsEntityId GEIDREF, Resources::ResourceName mesh, Resources::ResourceName anim, Resources::ResourceName skel);
	virtual void Update();
	virtual void Shutdown();
	virtual void Get();
	virtual void Set();
};

//WIP
class TransComp : public ComponentCore
{
public:
	Math::matrix44 transform;
	float movePos = 0;

	TransComp();
	void Init(Graphics::GraphicsEntityId GEIDREF);
	void Update();
	void Shutdown();
	void Get();
	void Set();
};

//WIP
class GraphicalComp : public ComponentCore
{
public:
	const Resources::ResourceName& meshResource = nullptr;
	const Resources::ResourceName& animResource = nullptr;
	const Resources::ResourceName& skeletonResource = nullptr;

	GraphicalComp();
	void Init(Graphics::GraphicsEntityId GEIDREF, Resources::ResourceName mesh, Resources::ResourceName anim, Resources::ResourceName skel);
	void Update();
	void Shutdown();
	void Get();
	void Set();
};