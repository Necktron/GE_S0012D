#include "foundation/stdneb.h"
#include "P2CP.h"
namespace py = pybind11;

P2CP pyNebRef;

void P2CP::PyManagerSetup(EntityManager* neckManagerRef)
{
	pyNebRef.pyManager = neckManagerRef;
}

PYBIND11_EMBEDDED_MODULE(pyNebula, m)
{
	m.doc() = "pybind example plugin";

	//ENTITY MANAGER BINDINGS
	m.def("managerInit",
		[&]
		{
			pyNebRef.pyManager->Init();
		}
	, "Initialize everything within the Entity Manager. ( All entities and compontents included )");
	m.def("managerUpdate", &EntityManager::Update, "Update everything withint the Entity Manager");
	m.def("managerShutdown", &EntityManager::Shutdown, "Shutdown Entity Manager and all of it's belonging entities and their components");

	m.def("findEnt",
		[&](const char *s)
		{
			n_printf("We are currently searching for an entity passed through PyBind!");
			Util::StringAtom entityName;
			entityName = s;
			pyNebRef.currEntRef = pyNebRef.pyManager->FindEnt(entityName);
		}
	, "Find an entity in the EM with corresponding name and save it as a ref");

	m.def("listOfEnt",
		[&]
		{
			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				n_printf("Entity Name: %s\n", pyNebRef.pyManager->entities[i]->entName);
			}

			n_printf("There is a total of %i entities!\n", pyNebRef.pyManager->entCount);
		}
		, "Prints all entity names and the amount of entities");

	m.def("addEnt",
		[&](const char *s)
		{
			n_printf("We are currently adding an entity passed through PyBind!");
			Util::StringAtom entityName;
			entityName = s;
			pyNebRef.pyManager->AddEnt(entityName);
		}
	, "Add an entity to the EM");

	m.def("addEnt",
		[&](const char *s, int loadout)
		{
			Util::StringAtom entityName;
			entityName = s;
			pyNebRef.pyManager->AddEnt(entityName, loadout);
		}
	, "Add an entity to the EM with specific mesh given by ID");

	m.def("delEnt",
		[&](const char *s)
		{
			Util::StringAtom entityName;
			entityName = s;
			pyNebRef.pyManager->DelEnt(entityName);
		}
	, "Delete an entity from the EM");

	//GAME ENTITY BINDINGS
	m.def("entityInit", &GameEntity::Init, "Initialize everything within the Game Entity. ( The entity and all of it's compontents included ). REQUIERES 'findEnt' TO BE PERFORMED FIRST");
	m.def("entityUpdate", &GameEntity::Update, "Update everything withint the Game Entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");
	m.def("entityShutdown", &GameEntity::Shutdown, "Shutdown the Game Entity and all of it's belonging components. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("addComp",
		[&](const char* s)
		{
			Util::StringAtom compToAdd;
			compToAdd = s;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->AddComp(compToAdd);
					break;
				}
			}
		}
	, "Add a component to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("findComp",
		[&](const char* s)
		{
			Util::StringAtom compToFind;
			compToFind = s;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.currCompRef = pyNebRef.pyManager->entities[i]->FindComp(compToFind);
					break;
				}
			}
		}
	, "Add a component to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("delComp",
		[&](const char* s)
		{
			Util::StringAtom compToFind;
			compToFind = s;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->DelComp(compToFind);
					break;
				}
			}
		}
	, "Delete a component from the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("addCompVarInt",
		[&](const char* s, int varValue)
		{
			Util::StringAtom varName;
			varName = s;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->AddCompVar(varName, varValue);
					break;
				}
			}
		}
	, "Add a int variable to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("addCompVarFloat",
		[&](const char* s, float varValue)
		{
			Util::StringAtom varName;
			varName = s;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->AddCompVar(varName, varValue);
					break;
				}
			}
		}
	, "Add a float variable to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("addCompVar",
		[&](const char* s, const char* varValue)
		{
			Util::StringAtom varName;
			varName = s;

			Util::StringAtom varV;
			varV = varValue;

			//If it's a matrix
			if (varV == "matrix")
			{
				for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
				{
					if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
					{
						pyNebRef.pyManager->entities[i]->AddCompVar(varName, n_new(Math::matrix44));
						break;
					}
				}
			}

			//If it's just a StringAtom
			else
			{
				for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
				{
					if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
					{
						pyNebRef.pyManager->entities[i]->AddCompVar(varName, &varV);
						break;
					}
				}
			}
		}
	, "Add a Matrix or SA variable to the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("setCompVarInt",
		[&](const char* s, int varValue)
		{
			Util::StringAtom varName;
			varName = s;

			GameEntity::CompVar varV(varValue);

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->SetVar(varName, varV);
					break;
				}
			}
		}
	, "Change the value of an exsisting int comp variable of the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("setCompVarFloat",
		[&](const char* s, float varValue)
		{
			Util::StringAtom varName;
			varName = s;

			GameEntity::CompVar varV(varValue);

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->SetVar(varName, varV);
					break;
				}
			}
		}
	, "Change the value of an exsisting float comp variable of the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("setCompVar",
		[&](const char* s, const char* varValue)
		{
			Util::StringAtom varName;
			varName = s;

			Util::StringAtom varV;
			varV = varValue;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.pyManager->entities[i]->SetVar(varName, &varV);
					break;
				}
			}
		}
	, "Change the value of an exsisting SA comp variable of the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");

	m.def("getCompVar",
		[&](const char* s)
		{
			Util::StringAtom compToFind;
			compToFind = s;

			for (int i = 0; i < pyNebRef.pyManager->entCount; i++)
			{
				if (pyNebRef.pyManager->entities[i]->entName == pyNebRef.currEntRef->entName)
				{
					pyNebRef.currCompVarRef = pyNebRef.pyManager->entities[i]->GetVar(compToFind);

					if (pyNebRef.currCompVarRef == nullptr)
					{
						n_printf("Comp var does not exsist! Sad times ;_;");
						break;
					}

					switch (pyNebRef.currEntRef->varLibrary[compToFind].data)
					{
					case GameEntity::CompVar::cvInt:
						n_printf("Comp var value: %i", pyNebRef.currCompVarRef->vIntNum);
						break;

					case GameEntity::CompVar::cvFloat:
						n_printf("Comp var value: %f", pyNebRef.currCompVarRef->vFloatNum);
						break;

					case GameEntity::CompVar::cvMatrix:
						n_printf("Comp var value: %f", pyNebRef.currCompVarRef->vMatrix);
						break;

					case GameEntity::CompVar::cvStringAtom:
						n_printf("Comp var value: %s", pyNebRef.currCompVarRef->vStrAtom);
						break;

					case GameEntity::CompVar::cvGEID:
						n_printf("Comp var value: %i", pyNebRef.currCompVarRef->vGEIDref);
						break;
					}
				}
			}
		}
	, "Get the value of an exsisting comp variable from the specific entity. REQUIERES 'findEnt' TO BE PERFORMED FIRST");
}