#include "ECSComponents.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

//COMP CORE
void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, bool reg)
{
	//Init the BaseCompClass
}

void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg)
{
	//Init the BaseCompClass
}

void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, float x, float y, float z, float moveX, float moveY, float moveZ, bool reg)
{
	//Init the BaseCompClass
}

void ComponentCore::Update()
{
	//Update the BaseCompClass
}

void ComponentCore::Shutdown()
{
	//Shuts down the BaseCompClass
}

void ComponentCore::MSGRecieve(ECSMSG::ECSMSGTypes msg)
{
	//DERP
}

//TRANS COMP
TransComp::TransComp()
{
	this->compID = 1;
}

TransComp::~TransComp()
{
	//Destructor
	ObservableContext::DeregisterEntity(this->GEID);
	ModelContext::DeregisterEntity(this->GEID);
	Graphics::DeregisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(this->GEID);
	ObservableContext::Destroy();
	ModelContext::Destroy();
}

void TransComp::Init(Graphics::GraphicsEntityId GEIDREF, bool reg)
{
	if (this->GEID == NULL)
		this->GEID = GEIDREF;
	//Init TransComp, overrides BaseCompClass
	if (reg == false)
	{
		Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(this->GEID);
	}

	ModelContext::SetTransform(this->GEID, this->transform.translation(Math::point(0, 0, 0)));
}

void TransComp::Init(Graphics::GraphicsEntityId GEIDREF, float x, float y, float z, float moveX, float moveY, float moveZ, bool reg)
{
	if (this->GEID == NULL)
		this->GEID = GEIDREF;
	//Init TransComp, overrides BaseCompClass
	if (reg == false)
	{
		Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(this->GEID);
	}

	xVal = x;
	yVal = y;
	zVal = z;
	movePosX = moveX;
	movePosY = moveY;
	movePosZ = moveZ;

	ModelContext::SetTransform(this->GEID, this->transform.translation(Math::point(xVal, yVal, zVal)));
}

void TransComp::Update()
{
	ModelContext::SetTransform(this->GEID, this->transform.translation(Math::point(xVal + movePosX, yVal + movePosY, zVal + movePosZ)));
}

void TransComp::Shutdown()
{
	//Shutdown TransComp, overrides BaseCompClass
	this->~TransComp();
	delete this;
}

void TransComp::MSGRecieve(ECSMSG::ECSMSGTypes msg)
{
	n_printf("We have recieved a message from a GameEntity in a Component!");
	if (msg == ECSMSG::ECSMSGTypes::UpdatePos)
	{
		Update();
	}

	else if (msg == ECSMSG::ECSMSGTypes::WalkForward)
	{
		ForwardWalk();
		Update();
	}

	else if (msg == ECSMSG::ECSMSGTypes::WalkBack)
	{
		BackWalk();
		Update();
	}

	else if (msg == ECSMSG::ECSMSGTypes::WalkLeft)
	{
		LeftWalk();
		Update();
	}

	else if (msg == ECSMSG::ECSMSGTypes::WalkRight)
	{
		RightWalk();
		Update();
	}
}

void TransComp::ForwardWalk()
{
	movePosZ += 0.2;
}

void TransComp::BackWalk()
{
	movePosZ -= 0.2;
}

void TransComp::RightWalk()
{
	movePosX -= 0.2;
}

void TransComp::LeftWalk()
{
	movePosX += 0.2;
}

//GRAPHICAL COMP
GraphicalComp::GraphicalComp()
{
	this->compID = 2;
}

GraphicalComp::~GraphicalComp()
{
	//Destructor
}

void GraphicalComp::Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg)
{
	if (this->GEID == NULL)
		this->GEID = GEIDREF;

	meshResource = mesh;
	animResource = anim;
	skeletonResource = skel;

	if (reg == false)
	{
		Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(this->GEID);
	}

	ModelContext::Setup(this->GEID, meshResource, "Examples");
	ObservableContext::Setup(this->GEID, VisibilityEntityType::Model);
	Characters::CharacterContext::Setup(this->GEID, skeletonResource, animResource, "Examples");
	Characters::CharacterContext::PlayClip(this->GEID, nullptr, 0, 0, Characters::Append, 1.0f, 1, Math::n_rand() * 100.0f, 0.0f, 0.0f, Math::n_rand() * 100.0f);
}

void GraphicalComp::Update()
{

}

void GraphicalComp::Shutdown()
{
	this->~GraphicalComp();
	delete this;
}

void GraphicalComp::MSGRecieve(ECSMSG::ECSMSGTypes msg)
{
	n_printf("We have recieved a message from a GameEntity in a Component!");
	/*if (msg == ECSMSG::ECSMSGTypes::UpdateGraphical)
	{
		Update();
	}*/
}