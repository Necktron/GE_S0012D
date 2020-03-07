#pragma once
#ifndef MESSAGER_H
#define MESSAGER_H
#endif

class ECSMSG
{
public:
	//Different message types
	enum ECSMSGTypes
	{
		WalkForward = 1,
		WalkLeft = 2,
		WalkRight = 3,
		WalkBack = 4,
		UpdatePos = 5
	};
};