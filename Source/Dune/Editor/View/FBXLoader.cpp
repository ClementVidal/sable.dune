#include <Dune/Editor/View/FBXLoader.h> 

using namespace Dune;

CViewFBXLoader::CLoadingThread::CLoadingThread()
{
	Result = NULL;
}

Void CViewFBXLoader::CLoadingThread::Entry()
{
	Result = Manager->GetOrAdd( FilePath );
}

CViewFBXLoader::CViewFBXLoader(  CApplication& app  ) :
	QObject( &app.GetMainFrame() )
{
	m_App = &app;
	connect( &m_Timer, SIGNAL( timeout() ), this, SLOT( OnTimeout() ) );
}

CViewFBXLoader::~CViewFBXLoader()
{

}

Bool CViewFBXLoader::OnTimeout()
{
	if( m_LoadingThread.IsFinished() )
	{
		m_Timer.stop();
		//m_App->GetMainFrame().setEnabled( TRUE );
		m_App->GetMainFrame().PulseProgressBar( FALSE );
		emit OnLoadingDone( m_LoadingThread.Result );
	}
	return TRUE;
}

Void CViewFBXLoader::Load( CFBXManager& manager, const Sb::CFilePath& path )
{
	m_LoadingThread.Manager = &manager;
	m_LoadingThread.FilePath = path;

	//m_App->GetMainFrame().setEnabled( FALSE );
	m_App->GetMainFrame().PulseProgressBar( TRUE, "Loading FBX...");
	m_LoadingThread.Start();
	m_Timer.start( 10 );
}