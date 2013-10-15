#ifndef _DUNE_APPLICATION_WXAPPLICATION_
#define _DUNE_APPLICATION_WXAPPLICATION_

#include <Dune\Editor\Sable\Application.h>
#include <Dune\Editor\Application\Settings.h>

#include <Sable\Core\Event\Header.h>
#include <Sable\Core\ManagedObject\Ref.h>

#include <QtGui\QApplication>

namespace Dune
{

class CViewMainFrame;
class CCtrl;
class CViewEditorContainer;
class CViewProject;
class CViewProperty;
class CViewUI;
class CViewAsset;
class CViewBank;
class CViewScene;
class CCtrlProject;
class CCommandProcessor;

/** 
Application
*/
class CApplication : public QApplication
{

public:
    
    /** @name Constructor/Destructor*/
    //@{
    CApplication( Int32 & argc, Char ** argv );
    ~CApplication();
    //@}
    
    /** @name Accessor*/
    //@{
	CViewMainFrame&				GetMainFrame();

	CProject*					GetProject();
	CCommandProcessor&			GetCommandProcessor();
	CApplicationSettings&		GetSettings();
    CSableApplication&			GetSableApplication();

	CViewEditorContainer&		GetEditorContainerView() const;
    CViewUI&					GetUIView() const;
    CViewProperty&				GetPropertyView() const;
	CViewAsset&					GetAssetView() const;
    CViewBank&					GetBankView() const;
    CViewScene&					GetSceneView() const;

    CCtrlProject&				GetProjectCtrl() const;
    CCtrlProject*				GetProjectCtrlPtr() const;
	//@}

    /** @name Manipulator*/
    //@{
	bool    Init();
    //@}

private:

    // Methods
    Int32           Run();
    Void            OnFatalError();

    // Attributes
    CSableApplication*                  m_Dune;
    Sb::CRef<CCtrlProject>				m_ProjectCtrl;
    CViewMainFrame*				        m_MainFrame;
    Sb::CRef<CApplicationSettings>		m_Settings;
	Sb::CRef<CCommandProcessor>			m_CommandProcessor;
};

}

#endif