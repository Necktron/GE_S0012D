#pragma once
#include "ECS.h"
#include <pybind11/pybind11.h>
#include "pybind11/embed.h"
#ifndef P2CP_H
#define P2CP_H

class P2CP
{
public:
	EntityManager* pyEM;
	GameEntity* currEntRef;
	ComponentCore* currCompRef;
	GameEntity::CompVar* currCompVarRef;
};
#endif