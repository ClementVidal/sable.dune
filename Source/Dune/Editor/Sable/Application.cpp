#include <Dune/Editor/Sable/Application.h>

#include <Dune/Editor/View/Input/Mouse.h>
#include <Dune/Editor/View/Input/Keyboard.h>

#include <Sable/Input/Pad/Header.h>

using namespace Dune;

CSableApplication::CSableApplication( Dune::CApplication& duneApp ) :
	Sb::CApplication( ),
	m_LogHandler( CoreManager.GetLog() )
{
    SignalUpdate = new Sb::CEventSignal<CSableEditorRenderer& >();
    SignalRender = new Sb::CEventSignal<CSableEditorRenderer& >();
}

CSableApplication::~CSableApplication()
{
    delete SignalUpdate;
    delete SignalRender;
    UnInitialize();
}

Void CSableApplication::Initialize()
{
    Sb::CApplication::Initialize();

    // Register log handler
   //CoreManager.GetLog().AddHandler( m_LogHandler );
}

CDuneLogHandler& CSableApplication::GetLogHandler()
{
	return m_LogHandler;
}

Void CSableApplication::UnInitialize()
{
    Sb::CApplication::UnInitialize();
}

Bool CSableApplication::Update()
{
    if( !Sb::CApplication::Update() )
        return FALSE;

	return TRUE;
}
