class ECS_MSG
{
	//Different message types
	enum MSGType
	{
		MSGT_AssignVariables = 0,
		MSGT_UpdatePos = 1,
		MSGT_UpdateGraphical = 2,
		MSGT_Shutdown = 3
	};

	//What to do when you send a message
	void MSG_Send()
	{

	}

	//What to do when you recieve a message
	void MSG_Recieve()
	{

	}
};