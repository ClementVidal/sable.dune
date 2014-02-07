#include <Dune/Editor/Ctrl/UI.h>

#include <Dune/Model/Bank/ObjectDirectory.h>
#include <Dune/Editor/View/UITree.h>
#include <Dune/Editor/View/Editor/UI/UI.h>
#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Editor/Ctrl/Editor/UI/UI.h>
#include <Dune/Editor/Ctrl/CommandProcessor.h>

using namespace Dune;

CCtrlUI::CCtrlUI( CApplication& app ):
    CCtrl( app )
{
}

CCtrlUI::~CCtrlUI()
{
}
