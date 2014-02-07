#include <Dune/Editor/Application/Application.h>

#include <Sable/Gui/Common/RenderPass.h>
#include <Sable/Graphics/Shader/Compiler.h>

#include <Dune/Editor/View/MainFrame.h>
#include <Dune/Editor/View/bank.h>
#include <Dune/Editor/View/Property.h>
#include <Dune/Editor/View/Editor/EditorContainer.h>
#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Editor/Ctrl/CommandProcessor.h>
#include <Dune/Common/XMLDataLayout.h>
#include <Dune/Resources/Icons/Application.xpm>

#include <QtCore/QFile>

using namespace Dune;

CApplication::CApplication( Int32 & argc, Char ** argv  ) :
	QApplication( argc, argv )
{
    m_Dune = NULL;
    m_MainFrame = NULL;
    m_Settings = NULL;
}

CApplication::~CApplication()
{
    // Project controller must be destroyed BEFORE the Sable application destruction.
    // Otherwise important stuff such as the MemoryManager won't be accessible anymore
    m_ProjectCtrl = NULL;
	m_CommandProcessor = NULL;
    m_Settings = NULL;

    delete m_Dune;

    //UnInitialize windows COM Library (This should not be done here)
    ::CoUninitialize( );
}

CApplicationSettings& CApplication::GetSettings()
{
    return *m_Settings;
}

CSableApplication& CApplication::GetSableApplication()
{
    return *m_Dune;
}

#include <Sable/Core/Bank/Bank.h>

bool CApplication::Init()
{
    //Initialize windows COM Library (This should not be done here)
    ::CoInitialize( NULL );

#if 0
        setStyleSheet( 
#include <Dune/Common/Qt/QtStyleSheet.inc>
            ); 
#endif

	// 1. Sable application framework
	m_Dune =  new CSableApplication( *this );

	// 2. Command processor
	m_CommandProcessor = NEWOBJ( CCommandProcessor, ( *this ) );

    m_ProjectCtrl = NEWOBJ( CCtrlProject, ( *this) );

	// 3. Main frame
	m_MainFrame = new CViewMainFrame(*this);

	m_Settings = NEWOBJ( CApplicationSettings, () );

	m_MainFrame->Initialize();
    m_Dune->Initialize();

    m_MainFrame->show();
/*
	Char buffer[512];
	m_Dune->GetFormattedInformation( buffer, 512 );
	Sb::DebugLogInfo("\n---- Dune startup\n%s\n----", buffer);
*/
    return TRUE;
} 

CCommandProcessor&	CApplication::GetCommandProcessor()
{
	return *m_CommandProcessor;
}

Int32 CApplication::Run()
{
    __try
    {
        return exec();
    }
    
    __except( Sb::DebugCreateDump( GetExceptionInformation() ), EXCEPTION_EXECUTE_HANDLER ) 
    {
        OnFatalError();
        ExitProcess( 0 );
        return 0;
    }
}

Void CApplication::OnFatalError()
{
/*    wxDateTime time = wxDateTime::Now();
    Char buffer[256];

	{
		// Create dir
		if( ! wxDirExists( WT("./Recover/") ) )
			wxFileName::Mkdir( WT("./Recover/") );
		// Try to save current project
		wxString recoverPath = time.Format( WT("./Recover/%d_%m_%Y__%H_%M.dprj") );
		Sb::WStringToString( (WString )recoverPath.GetData(), buffer, 256 );
		GetProjectView().GetCtrl().Save( buffer );
	}
*/
}


CViewEditorContainer& CApplication::GetEditorContainerView() const
{
    return m_MainFrame->GetEditorContainerView();
}

CProject* CApplication::GetProject()
{
	return GetProjectCtrl().GetProject();
}

CViewMainFrame&	CApplication::GetMainFrame()
{
	return *m_MainFrame;
}

CViewProperty& CApplication::GetPropertyView() const
{
    return m_MainFrame->GetPropertyView();
}

CViewUI& CApplication::GetUIView() const
{
    return m_MainFrame->GetUIView();
}

CViewAsset& CApplication::GetAssetView() const
{
    return m_MainFrame->GetAssetView();
}

CViewScene& CApplication::GetSceneView() const
{
    return m_MainFrame->GetSceneView();
}

CViewBank& CApplication::GetBankView() const
{
    return m_MainFrame->GetBankView();
}

CCtrlProject& CApplication::GetProjectCtrl() const
{
    return *m_ProjectCtrl;
}

CCtrlProject* CApplication::GetProjectCtrlPtr() const
{
    return m_ProjectCtrl;
}
