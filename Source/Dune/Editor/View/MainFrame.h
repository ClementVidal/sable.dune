#ifndef _DUNE_EDITOR_VIEW_MAINFRAME_
#define _DUNE_EDITOR_VIEW_MAINFRAME_

#include <SAble/Core/Math/Vector2f.h>

#include <Dune/Editor/Ctrl/EventClient.h>

#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QStatusBar>
#include <QtCore/QSignalMapper>
#include <QtGui/QProgressBar>

namespace Dune
{

class CViewProject;
class CViewEditorContainer;
class CViewLog;
class CViewProperty;
class CViewCommandStacks;
class CViewBank;
class CViewAsset;
class CViewUI;
class CViewScene;
class CViewGame;

/**
*/
class CViewMainFrame: public QMainWindow, public CEventClient
{

	Q_OBJECT

public:

    // Data Types

    /** @name Constructor/Destructor*/
    //@{
    CViewMainFrame( CApplication& app );
    virtual ~CViewMainFrame();
    //@}

    /** @name Accessors*/
    //@{
    CApplication&			GetApp() const;
    CViewEditorContainer&		GetEditorContainerView();
	CViewBank&				    GetBankView() const;
	CViewAsset&				    GetAssetView() const;
	CViewScene&				    GetSceneView() const;
	CViewUI&				    GetUIView() const;
    CViewProperty&				GetPropertyView() const;

	QMenu&						GetViewMenu();
    //@}

    /** @name Manipulator*/
    //@{
    Void						Initialize();

	Void						PulseProgressBar( Bool onOff, String desc = FALSE );
    //@}

private:

	Q_SLOT void OnUIUndo( );
	Q_SLOT void OnUIRedo( );
	Q_SLOT void OnUIQuit( );
	Q_SLOT void OnUINew( );
	Q_SLOT void OnUIOpen( );
	Q_SLOT void OnUISave( );
	Q_SLOT void OnUIClose( );
	Q_SLOT void OnUISaveAs( );
	Q_SLOT void OnUIEditSettings( );
	Q_SLOT void OnUIBakeProject( );
	Q_SLOT Bool OnUIOpenRecentProject( );

private:

    // Methods
	Void	OnProjectCreated( CProject& p );
	Void	OnProjectDestroyed();
	Void	OnSubmitCommand( const CCmd& cmd );

	Void	OnLoadDAE( Bool onOff );
    Void	UpdateRecentProjectMenu();
	Void	UpdateTitle();
    Void	EnableUI( Bool onOff );

    Bool    OnUICreateProject();
    Bool    OnUILoadProject( const Sb::CFilePath& path );
    Bool    OnUISaveProject( const Sb::CFilePath& path );
    Bool    OnUICloseProject();

	Void	SetupMenu();

    // Attributes
	CViewGame*				m_GameView;
    CViewBank*			    m_BankView;
	CViewScene*				m_SceneView;
	CViewAsset*				m_AssetView;
	CViewUI*				m_UIView;
    CViewEditorContainer*   m_EditorView;
    CViewLog*				m_LogView;
    CViewProperty*			m_PropertyView;
	CViewCommandStacks*		m_CommandStacksView;

    CApplication*       m_App;
    QMenuBar*				m_MenuBar;
	QMenu*					m_ViewMenu;
	QProgressBar*			m_MenuProgressBar;
    QMenu*					m_RecentProjectsMenu;
	QStatusBar*				m_StatusBar;
	QAction*				m_ActionClose;
	QAction*				m_ActionSave;
	QAction*				m_ActionSaveAs;
	QMenu*					m_MenuEdit;
	QMenu*					m_MenuProject;
	Sb::CEventSlot<CViewMainFrame, Bool >	m_SlotLoadDAE;
};

}

#endif