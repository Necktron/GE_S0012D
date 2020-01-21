#pragma once
//------------------------------------------------------------------------------
/**
    @class Example::ExampleApplication
    
    (C) 2020 Individual contributors, see AUTHORS file
*/
#include "app/application.h"
#include "renderutil/mayacamerautil.h"
#include "renderutil/freecamerautil.h"
#include "graphics/graphicsserver.h"
#include "graphics/view.h"
#include "graphics/stage.h"
#include "graphics/cameracontext.h"
#include "resources/resourcemanager.h"
#include "models/modelcontext.h"
#include "input/inputserver.h"
#include "io/ioserver.h"
#include "debug/debuginterface.h"
#include "http/httpinterface.h"
#include "http/httpserverproxy.h"     
#include "http/httpclientregistry.h"
#include "core/coreserver.h"
#include "io/iointerface.h"

namespace Example
{

class ExampleApplication : public App::Application
{
public:
    /// Constructor
	ExampleApplication();
    /// Destructor
    ~ExampleApplication();

    /// Open
    bool Open();
    /// Close
    void Close();
    /// Run
    void Run();

protected:
    void RenderUI();
    void UpdateCamera();
    void ResetCamera();
    void ToMaya();
    void ToFree();

	Ptr<Core::CoreServer> coreServer;
    Ptr<Graphics::GraphicsServer> gfxServer;
    Ptr<Resources::ResourceManager> resMgr;
    Ptr<Input::InputServer> inputServer;
    Ptr<IO::IoServer> ioServer;
	Ptr<IO::IoInterface> ioInterface;

    CoreGraphics::WindowId wnd;
    Ptr<Graphics::View> view;
    Ptr<Graphics::Stage> stage;

    Graphics::GraphicsEntityId cam;
    
    Graphics::GraphicsEntityId globalLight;
    IndexT frameIndex = -1;

    float prevAverageFrameTime = 0.0f;
	float averageFrameTime = 0.0f;

    bool renderDebug = false;
    int cameraMode = 0;
    float zoomIn = 0.0f;
    float zoomOut = 0.0f;
    Math::float2 panning{ 0.0f,0.0f };
    Math::float2 orbiting{ 0.0f,0.0f };    
    RenderUtil::MayaCameraUtil mayaCameraUtil;
    RenderUtil::FreeCameraUtil freeCamUtil;        
	Math::point defaultViewPoint{ 10.0f, 10.0f, 10.0f };
    
#if __NEBULA_HTTP__
	Ptr<Debug::DebugInterface> debugInterface;
	Ptr<Http::HttpInterface> httpInterface;
	Ptr<Http::HttpServerProxy> httpServerProxy;

	ushort defaultTcpPort;
#endif

#if __NEBULA_HTTP_FILESYSTEM__
	Ptr<Http::HttpClientRegistry> httpClientRegistry;
#endif      
};

} // namespace Example