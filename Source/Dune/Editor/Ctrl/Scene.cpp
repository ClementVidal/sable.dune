#include <Dune/Editor/Ctrl/Scene.h>

#include <Dune/Editor/View/Editor/Scene/Scene.h>
#include <Dune/Editor/View/SceneTree.h>
#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Editor/Ctrl/CommandProcessor.h>
#include <Dune/Model/Scene/World.h>

using namespace Dune;

CCtrlScene::CCtrlScene( CApplication& app ):
    CCtrl( app )
{
}

CCtrlScene::~CCtrlScene()
{
}
