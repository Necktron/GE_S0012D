#pragma once
#include "ECS.h"
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "pybind11/numpy.h"
#include "pybind11/operators.h"
#include "pybind11/embed.h"
#ifndef P2CP_H
#define P2CP_H

class P2CP
{
public:
	EntityManager* pyManager;
	GameEntity* currEntRef;
	ComponentCore* currCompRef;
	GameEntity::CompVar* currCompVarRef;

	void PyManagerSetup(EntityManager* neckManagerRef);
};
#endif