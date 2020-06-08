//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core/refcounted.h"
#include "timing/timer.h"
#include "io/console.h"
#include "visibility/visibilitycontext.h"
#include "models/streammodelpool.h"
#include "models/modelcontext.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "exampleapp.h"
#include "math/vector.h"
#include "math/point.h"
#include "dynui/imguicontext.h"
#include "lighting/lightcontext.h"
#include "characters/charactercontext.h"
#include "imgui.h"
#include "dynui/im3d/im3dcontext.h"
#include "dynui/im3d/im3d.h"
#include "graphics/environmentcontext.h"
#include "clustering/clustercontext.h"
#include "io/debug/iopagehandler.h"
#include "io/logfileconsolehandler.h"
#include "io/debug/consolepagehandler.h"
#include "core/debug/corepagehandler.h"
#include "threading/debug/threadpagehandler.h"
#include "memory/debug/memorypagehandler.h"
#include "io/debug/iopagehandler.h"
#include "io/fswrapper.h"
#include "system/nebulasettings.h"
#include "profiling/profiling.h"
#include <time.h>
#include "P2CP.h"

#ifdef __WIN32__
#include <shellapi.h>
#elif __LINUX__

#endif

using namespace Timing;
using namespace Graphics;
using namespace Visibility;
using namespace Models;
using namespace IO;
using namespace Http;
using namespace Debug;

namespace Example
{

    EntityManager* neckManager;
    P2CP pyNeb;
    int cycleJSON;
    bool ableToPerform;

    //------------------------------------------------------------------------------
    /**
    */
    ExampleApplication::ExampleApplication() :
        defaultTcpPort(2100)
    {
        this->SetAppTitle("Viewer App");
        this->SetCompanyName("Nebula");
    }

    //------------------------------------------------------------------------------
    /**
    */
    ExampleApplication::~ExampleApplication()
    {
        // empty
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
        ExampleApplication::Open()
    {
        if (Application::Open())
        {
            // create Nebula runtime
            this->coreServer = Core::CoreServer::Create();
            this->coreServer->SetCompanyName(Application::Instance()->GetCompanyName());
            this->coreServer->SetAppName(Util::StringAtom("Nebula"));

            this->inputServer = Input::InputServer::Create();
            this->ioServer = IO::IoServer::Create();

            this->ioInterface = IO::IoInterface::Create();
            this->ioInterface->Open();

            this->resMgr = Resources::ResourceManager::Create();
            this->resMgr->Open();

            Util::String root = IO::FSWrapper::GetHomeDirectory();

#if PUBLIC_BUILD
            if (System::NebulaSettings::Exists(Application::Instance()->GetCompanyName(), Application::Instance()->GetAppTitle(), "path"))
            {
                root = System::NebulaSettings::ReadString(Application::Instance()->GetCompanyName(), Application::Instance()->GetAppTitle(), "path");
            }
#else 
            if (System::NebulaSettings::Exists("gscept", "ToolkitShared", "workdir"))
            {
                root = System::NebulaSettings::ReadString("gscept", "ToolkitShared", "workdir");
            }
            if (System::NebulaSettings::Exists("gscept", "ToolkitShared", "path"))
            {
                this->coreServer->SetToolDirectory(System::NebulaSettings::ReadString("gscept", "ToolkitShared", "path"));
            }
#endif

            //n_assert2(System::NebulaSettings::ReadString("gscept", "ToolkitShared", "workdir"), "No working directory defined!");

            this->coreServer->SetRootDirectory(root);
            this->coreServer->Open();


#if __WIN32__
            Ptr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
            Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());
#endif

#if __NEBULA_HTTP_FILESYSTEM__
            // setup http subsystem
            this->httpClientRegistry = Http::HttpClientRegistry::Create();
            this->httpClientRegistry->Setup();
#endif

#if __NEBULA_HTTP__
            // setup http subsystem
            this->httpInterface = Http::HttpInterface::Create();
            this->httpInterface->SetTcpPort(this->defaultTcpPort);
            this->httpInterface->Open();
            this->httpServerProxy = Http::HttpServerProxy::Create();
            this->httpServerProxy->Open();
            this->httpServerProxy->AttachRequestHandler(Debug::CorePageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::ThreadPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::MemoryPageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::ConsolePageHandler::Create());
            this->httpServerProxy->AttachRequestHandler(Debug::IoPageHandler::Create());

            // setup debug subsystem
            this->debugInterface = DebugInterface::Create();
            this->debugInterface->Open();
#endif

            this->gfxServer = GraphicsServer::Create();
            this->inputServer->Open();
            this->gfxServer->Open();

            SizeT width = this->GetCmdLineArgs().GetInt("-w", 1680);
            SizeT height = this->GetCmdLineArgs().GetInt("-h", 1050);

            CoreGraphics::WindowCreateInfo wndInfo =
            {
                CoreGraphics::DisplayMode{ 100, 100, width, height },
                this->GetAppTitle(), "", CoreGraphics::AntiAliasQuality::None, true, true, false
            };
            this->wnd = CreateWindow(wndInfo);
            this->cam = Graphics::CreateEntity();

#if NEBULA_ENABLE_PROFILING
            Profiling::ProfilingRegisterThread();
#endif

            // create contexts, this could and should be bundled together
            CameraContext::Create();
            ModelContext::Create();
            ObserverContext::Create();
            ObservableContext::Create();

            Graphics::RegisterEntity<CameraContext, ObserverContext>(this->cam);
            CameraContext::SetupProjectionFov(this->cam, width / (float)height, Math::n_deg2rad(60.f), 0.1f, 1000.0f);

            Clustering::ClusterContext::Create(0.1f, 1000.0f, this->wnd);
            Lighting::LightContext::Create();
            Characters::CharacterContext::Create();
            Im3d::Im3dContext::Create();
            Dynui::ImguiContext::Create();

            this->view = gfxServer->CreateView("mainview", "frame:vkdefault.json"_uri);
            this->stage = gfxServer->CreateStage("stage1", true);

            this->globalLight = Graphics::CreateEntity();
            Lighting::LightContext::RegisterEntity(this->globalLight);
            Lighting::LightContext::SetupGlobalLight(this->globalLight, Math::float4(1, 1, 1, 0), 1.0f, Math::float4(0, 0, 0, 0), Math::float4(0, 0, 0, 0), 0.0f, -Math::vector(1, 1, 1), true);

            this->ResetCamera();
            CameraContext::SetTransform(this->cam, this->mayaCameraUtil.GetCameraTransform());

            this->view->SetCamera(this->cam);
            this->view->SetStage(this->stage);

            // register visibility system
            ObserverContext::CreateBruteforceSystem({});

            ObserverContext::Setup(this->cam, VisibilityEntityType::Camera);

            // create environment context for the atmosphere effects
            EnvironmentContext::Create(this->globalLight);

            this->pythonSrv = Scripting::PythonServer::Create();

            this->UpdateCamera();

            return true;
        }
        return false;
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::Close()
    {
        App::Application::Close();
        DestroyWindow(this->wnd);
        this->gfxServer->DiscardStage(this->stage);
        this->gfxServer->DiscardView(this->view);

        ObserverContext::Discard();
        Lighting::LightContext::Discard();

        this->gfxServer->Close();
        this->inputServer->Close();
        this->resMgr->Close();

        this->ioInterface->Close();
        this->ioInterface = nullptr;

        this->ioServer = nullptr;

#if __NEBULA_HTTP__
        this->debugInterface->Close();
        this->debugInterface = nullptr;

        this->httpServerProxy->Close();
        this->httpServerProxy = nullptr;
        this->httpInterface->Close();
        this->httpInterface = nullptr;
#endif

#if __NEBULA_HTTP_FILESYSTEM__
        this->httpClientRegistry->Discard();
        this->httpClientRegistry = nullptr;
#endif

        this->coreServer->Close();
        this->coreServer = nullptr;
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::Run()
    {
        bool run = true;
        ableToPerform = true;
        cycleJSON = 0;

        const Ptr<Input::Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
        const Ptr<Input::Mouse>& mouse = inputServer->GetDefaultMouse();

        neckManager = &neckManager->getInstance();
        pyNeb.PyManagerSetup(neckManager);

        Graphics::GraphicsEntityId world = Graphics::CreateEntity();
        // Register entity to various graphics contexts.
        // The template parameters are which contexts that the entity should be registered to.
        // ModelContext takes care of loading models and also handles transforms for instances of models.
        // Registering an entity to the ObservableContext will allow cameras to observe the entity (adds the entity to visibility culling system)
        Graphics::RegisterEntity<ModelContext, ObservableContext>(world);
        ModelContext::Setup(world, "mdl:environment/Groundplane.n3", "Examples");
        ModelContext::SetTransform(world, Math::matrix44::translation(Math::point(0, 0, 0)));
        ObservableContext::Setup(world, VisibilityEntityType::Model);
           
        this->pythonSrv->Open();
        this->pythonSrv->EvalFile("../../../nebula-example-master/code/neckPyScript.py");

        neckManager->AddEnt("King", GameEntity::Models::King);
        neckManager->entities[(neckManager->entCount - 1)]->AddComp("TransformComp");
        neckManager->entities[(neckManager->entCount - 1)]->AddComp("GraphicalComp");

        neckManager->entities[(neckManager->entCount - 1)]->AddCompVar("xVal", 10.0f);
        neckManager->entities[(neckManager->entCount - 1)]->AddCompVar("zVal", 0.0f);
        neckManager->entities[(neckManager->entCount - 1)]->AddCompVar("transform", n_new(Math::matrix44));

        neckManager->AddEnt("Derp", GameEntity::Models::Footman);
        neckManager->entities[(neckManager->entCount - 1)]->AddComp("TransformComp");
        neckManager->entities[(neckManager->entCount - 1)]->AddComp("GraphicalComp");

        neckManager->entities[(neckManager->entCount - 1)]->AddCompVar("xVal", 5.0f);
        neckManager->entities[(neckManager->entCount - 1)]->AddCompVar("zVal", -10.0f);
        neckManager->entities[(neckManager->entCount - 1)]->AddCompVar("transform", n_new(Math::matrix44));

        neckManager->Init();

        // Create a point light entity
        Graphics::GraphicsEntityId pointLight = Graphics::CreateEntity();
        // You can also register to contexts directly
        Lighting::LightContext::RegisterEntity(pointLight);
        Lighting::LightContext::SetupPointLight(pointLight, Math::float4(4.5, 0.5, 0.2, 1), 10.0f, Math::matrix44::translation(1, 2, 1), 100.0f, true);

        while (run && !inputServer->IsQuitRequested())
        {
#if NEBULA_ENABLE_PROFILING
            Profiling::ProfilingNewFrame();
#endif

#if __NEBULA_HTTP__
            this->httpServerProxy->HandlePendingRequests();
#endif

            this->coreServer->Trigger();

            this->inputServer->BeginFrame();
            this->inputServer->OnFrame();

            this->resMgr->Update(this->frameIndex);

            this->gfxServer->BeginFrame();

            // put game code which doesn't need visibility data or animation here

            this->gfxServer->BeforeViews();

            this->RenderUI();

            if (this->renderDebug)
            {
                this->gfxServer->RenderDebug(0);
            }

            // put game code which need visibility data here
            this->gfxServer->RenderViews();
            
            // put game code which needs rendering to be done (animation etc) here
            this->gfxServer->EndViews();

            // do stuff after rendering is done
            this->gfxServer->EndFrame();

            // force wait immediately
            WindowPresent(wnd, frameIndex);
            if (this->inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::Escape)) run = false;

            if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::LeftMenu))
                this->UpdateCamera();

            if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::F8))
                Resources::ResourceManager::Instance()->ReloadResource("shd:imgui.fxb");

            //CONTROLS FOR THE PRIMARY TARGET P1
            if (neckManager->entities.size() > 1)
            {
                for (int i = 0; i < neckManager->entities.size(); i++)
                {
                    if (neckManager->entities[i]->entName == "P1")
                    {
                        if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::W) && neckManager->entities.size() > 0)
                            neckManager->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::WalkForward);

                        if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::S) && neckManager->entities.size() > 0)
                            neckManager->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::WalkBack);

                        if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::A) && neckManager->entities.size() > 0)
                            neckManager->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::WalkLeft);

                        if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::D) && neckManager->entities.size() > 0)
                            neckManager->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::WalkRight);

                        if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::Space) && neckManager->entities.size() > 0)
                            neckManager->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::Ascend);

                        if (this->inputServer->GetDefaultKeyboard()->KeyPressed(Input::Key::LeftControl) && neckManager->entities.size() > 0)
                            neckManager->entities[i]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::Descend);
                    }
                }
            }

            //"DELETE" ALL ENTITIES CURRENTLY IN THE SCENE
            if (this->inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::Q) && neckManager->entities.size() > 0 && ableToPerform == true)
            {
                ableToPerform = false;
                neckManager->Shutdown();
                ableToPerform = true;
            }

            //SPAWN A ENTITY WITH RANDOM LOADOUT AND POSITION
            if (this->inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::E) && ableToPerform == true)
            {
                ableToPerform = false;

                char spawnName[64] = "RandSpawn_";
                char num[8];
                sprintf(num, "%d", neckManager->entCount);
                strcat(spawnName, num);

                if (neckManager->FindEnt(spawnName))
                {
                    n_printf("ERROR: A randomly generated entity with this name already exsists!\n");
                }

                else
                {
                    neckManager->AddEnt(spawnName);
                    neckManager->entities[(neckManager->entCount - 1)]->AddComp("TransformComp");
                    neckManager->entities[(neckManager->entCount - 1)]->AddComp("GraphicalComp");
                    neckManager->entities[(neckManager->entCount - 1)]->Init();
                    neckManager->entities[(neckManager->entCount - 1)]->compList[0]->MSGRecieve(ECSMSG::ECSMSGTypes::SpawnEnt);
                    n_printf("Entities created during runtime before crash with AT implementation: %i!\n", neckManager->totalEntCount);
                    ableToPerform = true;
                }
            }

            //SWITCH BETWEEN JSON FILES WITH SCENE DESRIPTIONS
            if (this->inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::X) && ableToPerform == true)
            {
                ableToPerform = false;
                n_printf("X is down!\n");

                StreamReader* neckParser = StreamReader::Create();
                Stream* pathToJSON = Stream::Create();

                //DELETE ALL PREVIOUS ENTITIES, IF ANY EXSIST
                if (neckManager->entCount > 0)
                    neckManager->Shutdown();

                //CYCLE JSON FILE, let's say Map 1, Map 2, Map 3

                pathToJSON->SetURI("JSONONE.json");
                neckParser->SetStream(pathToJSON);
                neckParser->HasStream();

                if (pathToJSON->CanRead())
                    n_printf("Bless...! <3\n");

                //CYCLE JSON FILE, let's say Map 1, Map 2, Map 3
                neckParser->Open();

                if (pathToJSON->IsOpen())
                {
                    n_printf("Sweet <3\n");
                }

                else
                {
                    n_printf("Fuck...\n");
                }

                pathToJSON->Close();

                //READ JSON FILE

                ableToPerform = true;
            }

            if (this->inputServer->GetDefaultKeyboard()->KeyDown(Input::Key::F1))
            {
                // Open browser with debug page.
                Util::String url = "http://localhost:";
                url.AppendInt(this->defaultTcpPort);
#ifdef __WIN32__
                ShellExecute(0, 0, url.AsCharPtr(), 0, 0, SW_SHOW);
#elif __LINUX__
                Util::String shellCommand = "open ";
                shellCommand.Append(url);
                system(shellCommand.AsCharPtr());
#else
                n_printf("Cannot open browser. URL is %s\n", url.AsCharPtr());
#endif
            }

            frameIndex++;
            this->inputServer->EndFrame();
        }
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::RenderUI()
    {
        this->averageFrameTime += (float)this->gfxServer->GetFrameTime();
        if (this->gfxServer->GetFrameIndex() % 35 == 0)
        {
            this->prevAverageFrameTime = this->averageFrameTime / 35.0f;
            this->averageFrameTime = 0.0f;
        }
        ImGui::Begin("Debug", nullptr, 0);
        ImGui::Text("ms - %.2f\nFPS - %.2f", this->prevAverageFrameTime * 1000, 1 / this->prevAverageFrameTime);
        ImGui::SetWindowSize(ImVec2(240, 400));
        if (ImGui::CollapsingHeader("Camera mode", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::RadioButton("Maya", &this->cameraMode, 0))this->ToMaya();
            ImGui::SameLine();
            if (ImGui::RadioButton("Free", &this->cameraMode, 1))this->ToFree();
            ImGui::SameLine();
            if (ImGui::Button("Reset")) this->ResetCamera();
        }
        ImGui::Checkbox("Debug Rendering", &this->renderDebug);

        ImGui::End();
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::UpdateCamera()
    {
        const Ptr<Input::Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
        const Ptr<Input::Mouse>& mouse = inputServer->GetDefaultMouse();

        this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(Input::MouseButton::LeftButton));
        this->mayaCameraUtil.SetPanButton(mouse->ButtonPressed(Input::MouseButton::MiddleButton));
        this->mayaCameraUtil.SetZoomButton(mouse->ButtonPressed(Input::MouseButton::RightButton));
        this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
        this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
        this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());

        // process keyboard input
        Math::float4 pos(0.0f);
        if (keyboard->KeyDown(Input::Key::Space))
        {
            this->mayaCameraUtil.Reset();
        }
        if (keyboard->KeyPressed(Input::Key::Left))
        {
            panning.x() -= 0.1f;
            pos.x() -= 0.1f;
        }
        if (keyboard->KeyPressed(Input::Key::Right))
        {
            panning.x() += 0.1f;
            pos.x() += 0.1f;
        }
        if (keyboard->KeyPressed(Input::Key::Up))
        {
            panning.y() -= 0.1f;
            if (keyboard->KeyPressed(Input::Key::LeftShift))
            {
                pos.y() -= 0.1f;
            }
            else
            {
                pos.z() -= 0.1f;
            }
        }
        if (keyboard->KeyPressed(Input::Key::Down))
        {
            panning.y() += 0.1f;
            if (keyboard->KeyPressed(Input::Key::LeftShift))
            {
                pos.y() += 0.1f;
            }
            else
            {
                pos.z() += 0.1f;
            }
        }

        this->mayaCameraUtil.SetPanning(panning);
        this->mayaCameraUtil.SetOrbiting(orbiting);
        this->mayaCameraUtil.SetZoomIn(zoomIn);
        this->mayaCameraUtil.SetZoomOut(zoomOut);
        this->mayaCameraUtil.Update();

        this->freeCamUtil.SetForwardsKey(keyboard->KeyPressed(Input::Key::W));
        this->freeCamUtil.SetBackwardsKey(keyboard->KeyPressed(Input::Key::S));
        this->freeCamUtil.SetRightStrafeKey(keyboard->KeyPressed(Input::Key::D));
        this->freeCamUtil.SetLeftStrafeKey(keyboard->KeyPressed(Input::Key::A));
        this->freeCamUtil.SetUpKey(keyboard->KeyPressed(Input::Key::Q));
        this->freeCamUtil.SetDownKey(keyboard->KeyPressed(Input::Key::E));

        this->freeCamUtil.SetMouseMovement(mouse->GetMovement());
        this->freeCamUtil.SetAccelerateButton(keyboard->KeyPressed(Input::Key::LeftShift));

        this->freeCamUtil.SetRotateButton(mouse->ButtonPressed(Input::MouseButton::LeftButton));
        this->freeCamUtil.Update();

        switch (this->cameraMode)
        {
        case 0:
            CameraContext::SetTransform(this->cam, Math::matrix44::inverse(this->mayaCameraUtil.GetCameraTransform()));
            break;
        case 1:
            CameraContext::SetTransform(this->cam, Math::matrix44::inverse(this->freeCamUtil.GetTransform()));
            break;
        default:
            break;
        }
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::ResetCamera()
    {
        this->freeCamUtil.Setup(this->defaultViewPoint, Math::float4::normalize(this->defaultViewPoint));
        this->freeCamUtil.Update();
        this->mayaCameraUtil.Setup(Math::point(0.0f, 0.0f, 0.0f), this->defaultViewPoint, Math::vector(0.0f, 1.0f, 0.0f));
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::ToMaya()
    {
        this->mayaCameraUtil.Setup(this->mayaCameraUtil.GetCenterOfInterest(), this->freeCamUtil.GetTransform().get_position(), Math::vector(0, 1, 0));
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
        ExampleApplication::ToFree()
    {
        Math::float4 pos = this->mayaCameraUtil.GetCameraTransform().get_position();
        this->freeCamUtil.Setup(pos, Math::float4::normalize(pos - this->mayaCameraUtil.GetCenterOfInterest()));
    }
}