//------------------------------------------------------------------------------
//  main.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "foundation.h"
#include "core/coreserver.h"
#include "core/sysfunc.h"
#include "exampleapp.h"
#include "system/appentry.h"

using namespace Core;

ImplementNebulaApplication()

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const Util::CommandLineArgs& args)
{
	Example::ExampleApplication app;
	app.SetCmdLineArgs(args);
    if (app.Open())
    {
        app.Run();
        app.Close();
    }

	Core::SysFunc::Exit(0);
}
