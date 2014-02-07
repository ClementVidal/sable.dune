#ifndef _DUNE_EDITOR_VIEW_UITREE_
#define _DUNE_EDITOR_VIEW_UITREE_

#include <Sable/Core/Common/DataTypes.h>
#include <Dune/Editor/Ctrl/EventClient.h>
#include <Dune/Editor/Utilities/ColorTable.h>
#include <Dune/Editor/Utilities/IconTable.h>
#include <Dune/Editor/View/BankObjectTree.h>


namespace Dune
{

class CCtrlUI;
class CProject;
class CApplication;

/**
UI representation of a project
*/
class CViewUITree : public CViewBankObjectTree
{
    
	Q_OBJECT

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewUITree( CViewUI& uiView, CApplication& app );
    ~CViewUITree();
    //@}

    /** @name Accesors*/
    //@{
	QIcon			GetIcon( const Sb::CTypeInfo& type ) const;
	Bool			HasIcon( const Sb::CTypeInfo& type ) const;
    Bool            CanDropOn( const CHandle& h ) const;
	CCtrlUI&		GetCtrl();
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types
 
    // Methods
    Q_SLOT Bool		OnUIEditElement( );
    Q_SLOT Bool		OnUIRemoveElement( );

	Void			contextMenuEvent(QContextMenuEvent * event );
	UInt32			GetIconIdFromType( const Sb::CTypeInfo& info ) const;
    Void			OnProjectLoaded( CProject& p );
    Void            OnProjectCreated( CProject& p );

    CHandle         GetRootObject( CProject& p );

    // Attributes
 	Sb::CRef<CCtrlUI>	m_CtrlUI;
	CViewUI*			m_UIView;

    // Static methods

};

};

#endif