#include "foundation/stdneb.h"
#include "ECSComponents.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

__ImplementClass(ComponentCore, '9012', Core::RefCounted);
__ImplementClass(TransComp, '3456', ComponentCore);
__ImplementClass(GraphicalComp, '7890', ComponentCore);

//COMP CORE
void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, bool reg)
{
	
}

void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg)
{
	
}

void ComponentCore::Update()
{

}

void ComponentCore::Shutdown()
{
	//Shuts down the BaseCompClass
}

void ComponentCore::MSGRecieve(ECSMSG::ECSMSGTypes msg)
{

}

void ComponentCore::ForwardWalk()
{
	this->managerInst->entities[this->parentIDRef]->SetVar("zVal", (this->managerInst->entities[this->parentIDRef]->GetVar("zVal")->vFloatNum + 0.2f));
}

void ComponentCore::BackWalk()
{
	this->managerInst->entities[this->parentIDRef]->SetVar("zVal", (this->managerInst->entities[this->parentIDRef]->GetVar("zVal")->vFloatNum - 0.2f));
}

void ComponentCore::RightWalk()
{
	this->managerInst->entities[this->parentIDRef]->SetVar("xVal", (this->managerInst->entities[this->parentIDRef]->GetVar("xVal")->vFloatNum - 0.2f));
}

void ComponentCore::LeftWalk()
{
	this->managerInst->entities[this->parentIDRef]->SetVar("xVal", (this->managerInst->entities[this->parentIDRef]->GetVar("xVal")->vFloatNum + 0.2f));
}

void ComponentCore::Ascend()
{
	this->managerInst->entities[this->parentIDRef]->SetVar("yVal", (this->managerInst->entities[this->parentIDRef]->GetVar("yVal")->vFloatNum + 0.2f));
}

void ComponentCore::Descend()
{
	this->managerInst->entities[this->parentIDRef]->SetVar("yVal", (this->managerInst->entities[this->parentIDRef]->GetVar("yVal")->vFloatNum - 0.2f));
}

//TRANS COMP
TransComp::TransComp()
{

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
	//Look at ComponentCore Init
	this->managerInst = &this->managerInst->getInstance();

	for (int i = 0; i < this->managerInst->entities.size(); i++)
	{
		if (this->managerInst->entities[i]->entName == this->parentNameRef)
		{
			if(this->managerInst->entities[i]->GetVar("transform") != nullptr)
				this->managerInst->entities[i]->AddCompVar("transform", n_new(Math::matrix44));
			
			if (this->managerInst->entities[i]->GetVar("xVal") == nullptr)
				this->managerInst->entities[i]->AddCompVar("xVal", 0.0f);

			if (this->managerInst->entities[i]->GetVar("yVal") == nullptr)
				this->managerInst->entities[i]->AddCompVar("yVal", 0.0f);

			if (this->managerInst->entities[i]->GetVar("zVal") == nullptr)
				this->managerInst->entities[i]->AddCompVar("zVal", 0.0f);

			if (this->GEID == NULL)
				this->GEID = GEIDREF;

			if (this->managerInst->entities[i]->registered == false)
				Graphics::RegisterEntity<ModelContext, ObservableContext, Characters::CharacterContext>(GEIDREF);

			xVal = this->managerInst->entities[i]->GetVar("xVal")->vFloatNum;
			yVal = this->managerInst->entities[i]->GetVar("yVal")->vFloatNum;
			zVal = this->managerInst->entities[i]->GetVar("zVal")->vFloatNum;

			ModelContext::SetTransform(this->GEID, this->managerInst->entities[i]->GetVar("transform")->vMatrix->translation(Math::point(xVal, yVal, zVal)));
		}
	}
}

void TransComp::Update()
{
	ModelContext::SetTransform(this->GEID, this->transform.translation(Math::point(xVal, yVal, zVal)));
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

	else if (msg == ECSMSG::ECSMSGTypes::Ascend)
	{
		Ascend();
		Update();
	}

	else if (msg == ECSMSG::ECSMSGTypes::Descend)
	{
		Descend();
		Update();
	}
}

void TransComp::ForwardWalk()
{
	zVal += 0.2;
}

void TransComp::BackWalk()
{
	zVal -= 0.2;
}

void TransComp::RightWalk()
{
	xVal -= 0.2;
}

void TransComp::LeftWalk()
{
	xVal += 0.2;
}

void TransComp::Ascend()
{
	yVal += 0.2;
}

void TransComp::Descend()
{
	yVal -= 0.2;
}

//GRAPHICAL COMP
GraphicalComp::GraphicalComp()
{

}

GraphicalComp::~GraphicalComp()
{
	//Destructor
}

void GraphicalComp::Init(Graphics::GraphicsEntityId GEIDREF, Util::StringAtom mesh, Util::StringAtom anim, Util::StringAtom skel, bool reg)
{
	this->managerInst = &this->managerInst->getInstance();

	for (int i = 0; i < this->managerInst->entities.size(); i++)
	{

		if (this->managerInst->entities[i]->entName == this->parentNameRef)
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