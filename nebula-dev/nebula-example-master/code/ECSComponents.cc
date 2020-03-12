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

}

void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg)
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
TransComp::TransComp(Util::StringAtom parent)
{
	this->parentRef = parent;
	this->compID = 1;
	this->managerInst = &this->managerInst->getInstance();
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
	for (int i = 0; i < this->managerInst->entities.size(); i++)
	{
		if (this->managerInst->entities[i]->entName == this->parentRef)
		{
			this->managerInst->entities[i]->AddCompVar("transform", n_new(Math::matrix44));
			this->managerInst->entities[i]->AddCompVar("movePosX", 0.0f);
			this->managerInst->entities[i]->AddCompVar("movePosY", 0.0f);
			this->managerInst->entities[i]->AddCompVar("movePosZ", 0.0f);
			this->managerInst->entities[i]->AddCompVar("xVal", 0.0f);
			this->managerInst->entities[i]->AddCompVar("yVal", 0.0f);
			this->managerInst->entities[i]->AddCompVar("zVal", 0.0f);

			if (this->GEID == NULL)
			{
				this->managerInst->entities[i]->AddCompVar("GEID", GEIDREF);
				this->GEID = this->managerInst->entities[i]->GetVar("GEID")->vGEIDref;
			}

			xVal = this->managerInst->entities[i]->GetVar("xVal")->vFloatNum;
			yVal = this->managerInst->entities[i]->GetVar("yVal")->vFloatNum;
			zVal = this->managerInst->entities[i]->GetVar("zVal")->vFloatNum;
			movePosX = this->managerInst->entities[i]->GetVar("movePosX")->vFloatNum;
			movePosY = this->managerInst->entities[i]->GetVar("movePosY")->vFloatNum;
			movePosZ = this->managerInst->entities[i]->GetVar("movePosZ")->vFloatNum;

			if (reg == false)
				Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(this->GEID);

			ModelContext::SetTransform(this->GEID, this->transform.translation(Math::point(0, 0, 0)));
			break;
		}
	}
}

void TransComp::Update()
{
	ModelContext::SetTransform(this->GEID, this->transform.translation(Math::point(xVal + movePosX, yVal + movePosY, zVal + movePosZ)));
}

void TransComp::Shutdown()
{
	//Shutdown TransComp, overrides BaseCompClass
	this->~TransComp();
}

void TransComp::MSGRecieve(ECSMSG::ECSMSGTypes msg)
{
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
GraphicalComp::GraphicalComp(Util::StringAtom parent)
{
	this->parentRef = parent;
	this->compID = 2;
	this->managerInst = &this->managerInst->getInstance();
}

GraphicalComp::~GraphicalComp()
{
	//Destructor
}

void GraphicalComp::Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg)
{
	for (int i = 0; i < this->managerInst->entities.size(); i++)
	{
		if (this->managerInst->entities[i]->entName == this->parentRef)
		{
			this->managerInst->entities[i]->AddCompVar("meshResource", n_new(Util::StringAtom(mesh)));
			this->managerInst->entities[i]->AddCompVar("animResource", n_new(Util::StringAtom(anim)));
			this->managerInst->entities[i]->AddCompVar("skeletonResource", n_new(Util::StringAtom(skel)));

			if (this->GEID == NULL)
			{
				this->managerInst->entities[i]->AddCompVar("GEID", GEIDREF);
				this->GEID = this->managerInst->entities[i]->GetVar("GEID")->vGEIDref;
			}

			meshResource = this->managerInst->entities[i]->GetVar("meshResource")->vStrAtom->Value();
			animResource = this->managerInst->entities[i]->GetVar("animResource")->vStrAtom->Value();
			skeletonResource = this->managerInst->entities[i]->GetVar("skeletonResource")->vStrAtom->Value();

			if (reg == false)
				Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(this->GEID);

			ModelContext::Setup(this->GEID, meshResource, "Examples");
			ObservableContext::Setup(this->GEID, VisibilityEntityType::Model);
			Characters::CharacterContext::Setup(this->GEID, skeletonResource, animResource, "Examples");
			Characters::CharacterContext::PlayClip(this->GEID, nullptr, 0, 0, Characters::Append, 1.0f, 1, Math::n_rand() * 100.0f, 0.0f, 0.0f, Math::n_rand() * 100.0f);
			break;
		}
	}
}

void GraphicalComp::Update()
{

}

void GraphicalComp::Shutdown()
{
	this->~GraphicalComp();
}

void GraphicalComp::MSGRecieve(ECSMSG::ECSMSGTypes msg)
{
	n_printf("We have recieved a message from a GameEntity in a Component!");
}