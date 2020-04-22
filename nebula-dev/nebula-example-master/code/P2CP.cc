#include "foundation/stdneb.h"
#include "P2CP.h"
namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(pyNebula, m)
{
	P2CP pyNebRef;
	pyNebRef.pyEM = &pyNebRef.pyEM->getInstance();

	m.doc() = "pybind example plugin";

	//ENTITY MANAGER BINDINGS
	m.def("managerInit", &EntityManager::Init, "Initialize everything within the Entity Manager. ( All entities and compontents included )");
	m.def("managerUpdate", &EntityManager::Update, "Update everything withint the Entity Manager");
	m.def("managerShutdown", &EntityManager::Shutdown, "Shutdown Entity Manager and all of it's belonging entities and their components");


	m.def("findEnt",
		[&](Util::StringAtom entityName)
		{
			pyNebRef.currEntRef = pyNebRef.pyEM->FindEnt(entityName);
		}
	, "Find an entity in the EM with corresponding name and save it as a ref");

	m.def("listOfEnt",
		[&]
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				n_printf("Entity Name: %s\n", pyNebRef.pyEM->entities[i]->entName);
			}

			n_printf("There is a total of %s entities!\n", pyNebRef.pyEM->entCount);
		}
		, "Prints all entity names and the amount of entities");

	m.def("addEnt",
		[&](Util::StringAtom entityName)
		{
			pyNebRef.pyEM->AddEnt(entityName);
		}
	, "Add an entity to the EM");

	m.def("addEnt",
		[&](Util::StringAtom entityName, int loadout)
		{
			pyNebRef.pyEM->AddEnt(entityName, loadout);
		}
	, "Add an entity to the EM with specific mesh given by ID");

	m.def("delEnt",
		[&](Util::StringAtom entityName)
		{
			pyNebRef.pyEM->DelEnt(entityName);
		}
	, "Delete an entity from the EM");

	//GAME ENTITY BINDINGS
	m.def("entityInit", &GameEntity::Init, "Initialize everything within the Game Entity. ( The entity and all of it's compontents included ). REQUIERES 'findEnt' TO BE PERFORMED FIRST");
	m.def("entityUpdate", &GameEntity::Update, "Update everything withint the Game Entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");
	m.def("entityShutdown", &GameEntity::Shutdown, "Shutdown the Game Entity and all of it's belonging components. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("addComp",
		[&](Util::StringAtom compToAdd)
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				if (pyNebRef.pyEM->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyEM->entities[i]->AddComp(compToAdd);
					break;
				}
			}
		}
	, "Add a component to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("findComp",
		[&](Util::StringAtom compToFind)
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				if (pyNebRef.pyEM->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.currCompRef = pyNebRef.pyEM->entities[i]->FindComp(compToFind);
					break;
				}
			}
		}
	, "Add a component to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("delComp",
		[&](Util::StringAtom compToFind)
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				if (pyNebRef.pyEM->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyEM->entities[i]->DelComp(compToFind);
					break;
				}
			}
		}
	, "Delete a component from the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("addCompVar",
		[&](Util::StringAtom varName, GameEntity::CompVar varValue)
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				if (pyNebRef.pyEM->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyEM->entities[i]->AddCompVar(varName, varValue);
					break;
				}
			}
		}
	, "Add a component variable to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("setCompVar",
		[&](Util::StringAtom varName, GameEntity::CompVar varValue)
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				if (pyNebRef.pyEM->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyEM->entities[i]->SetVar(varName, varValue);
					break;
				}
			}
		}
	, "Change the value of an exsisting comp variable of the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("getCompVar",
		[&](Util::StringAtom compToFind)
		{
			for (int i = 0; i < pyNebRef.pyEM->entCount; i++)
			{
				if (pyNebRef.pyEM->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.currCompVarRef = pyNebRef.pyEM->entities[i]->GetVar(compToFind);
					n_printf("Comp var value: %s", pyNebRef.currCompVarRef->data);
					break;
				}
			}
		}
	, "Get the value of an exsisting comp variable from the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");
}