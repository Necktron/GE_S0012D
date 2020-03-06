#include "ECSComponents.h"

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

//COMP CORE
void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF)
{
	//Init the BaseCompClass
}

void ComponentCore::Init(Graphics::GraphicsEntityId GEIDREF, Resources::ResourceName mesh, Resources::ResourceName anim, Resources::ResourceName skel)
{
	//Init
}

void ComponentCore::Update()
{
	//Update the BaseCompClass
}

void ComponentCore::Shutdown()
{
	//Shuts down the BaseCompClass
}

void ComponentCore::Get()
{
	//Get value from certain variable
}

void ComponentCore::Set()
{
	//Set value of certain variable
}

//TRANS COMP
TransComp::TransComp()
{
	this->compID = 1;
}

void TransComp::Init(Graphics::GraphicsEntityId GEIDREF)
{
	if (this->GEID == NULL)
		this->GEID = GEIDREF;
	//Init TransComp, overrides BaseCompClass
}

void TransComp::Update()
{
	movePos += 0.01;
	ModelContext::SetTransform(this->GEID, transform.translation(Math::point(movePos, 0, 0)));
}

void TransComp::Shutdown()
{
	//Shutdown TransComp, overrides BaseCompClass
}

void TransComp::Get()
{
	//Get value from certain variable
}

void TransComp::Set()
{
	//Set value of certain variable
}

//GRAPHICAL COMP
GraphicalComp::GraphicalComp()
{
	this->compID = 2;
}

void GraphicalComp::Init(Graphics::GraphicsEntityId GEIDREF, Resources::ResourceName mesh, Resources::ResourceName anim, Resources::ResourceName skel)
{
	if (this->GEID == NULL)
		this->GEID = GEIDREF;

	meshResource == mesh;
	animResource == anim;
	skeletonResource == skel;
	ModelContext::Setup(this->GEID, meshResource.AsString(), "Examples");
	ObservableContext::Setup(this->GEID, VisibilityEntityType::Model);
	Characters::CharacterContext::Setup(this->GEID, skeletonResource.AsString(), animResource.AsString(), "Examples");
	Characters::CharacterContext::PlayClip(this->GEID, nullptr, 0, 0, Characters::Append, 1.0f, 1, Math::n_rand() * 100.0f, 0.0f, 0.0f, Math::n_rand() * 100.0f);
}

void GraphicalComp::Update()
{
	
}

void GraphicalComp::Shutdown()
{
	//Shutdown GraphicalComp, overrides BaseCompClass
}

void GraphicalComp::Get()
{
	//Get value from certain variable
}

void GraphicalComp::Set()
{
	//Set value of certain variable
}