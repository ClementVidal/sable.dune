#ifndef _DUNE_EDITOR_VIEW_COLLADALOADER_
#define _DUNE_EDITOR_VIEW_COLLADALOADER_

#include <Sable\Core\Common\DataTypes.h>
#include <SAble\Core\Thread\Thread.h>

#include <QtCore\QTimer>



namespace Dune
{

/**
Provide an independant way to load asynchroneously a collada file
*/
class CViewFBXLoader : public QObject
{

	Q_OBJECT 

public:

    // Data Types

    /** @name Constructor/Destructor*/
    //@{
    CViewFBXLoader( CApplication& app );
    virtual ~CViewFBXLoader();
    //@}

    /** @name Manipulator*/
    //@{
	Void Load( CFBXManager& manager, const Sb::CFilePath& path );
    //@}

	/** @name Signals*/
	//@{
	Q_SIGNAL Bool OnLoadingDone( CFBXDoc* dae );
	//@}

private:

    // Types
	class CLoadingThread : public Sb::CThread
	{
	public:
		CLoadingThread( );
		
		Sb::CFilePath			FilePath;
		CFBXDoc*		Result;
		CFBXManager*	Manager;

		Void Entry();

	};

    // Methods
	Q_SLOT	Bool	OnTimeout();

    // Attributes
	CLoadingThread		m_LoadingThread;
	QTimer				m_Timer;
	CApplication*	m_App;
};

};

#endif