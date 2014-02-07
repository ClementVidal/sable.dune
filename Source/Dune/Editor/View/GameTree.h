#ifndef _DUNE_EDITOR_VIEW_GAMEOBJECTTREE_
#define _DUNE_EDITOR_VIEW_GAMEOBJECTTREE_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/View/BankObjectTree.h>
#include <Dune/Editor/Ctrl/EventClient.h>
#include <Dune/Editor/Utilities/IconTable.h>

#include <QtGui/QTreeWidget>
#include <QtCore/QMimeData>
#include <QtCore/QFileSystemWatcher>

namespace Dune
{

class CCtrlGame;
class CGame;
class CApplication;
class CPropertyInterface;
class CProject;
class CHandle;
class CViewGame;

/**
UI representation of a project
*/
class CViewGameTree : public CViewBankObjectTree
{

	Q_OBJECT
    
public:

	/** @name Constructor/Destructor*/
    //@{
    CViewGameTree( CViewGame& assetView, CApplication& app );
    virtual ~CViewGameTree();
    //@}

    /** @name Accesors*/
    //@{
    Bool            HasIcon( const Sb::CTypeInfo& type ) const;
    Bool            CanDropOn( const CHandle& h ) const;
	CCtrlGame&		GetCtrl();
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // friendship
    friend CCtrlGame;

    // Types

    // Methods
    CHandle         GetRootObject( CProject& p );

private :

	Q_SLOT void     OnUIRemoveGame( );
    Q_SLOT void     OnUIUseOwnerBank();
	Q_SLOT void		OnUIExportToLibrary();
	Q_SLOT void		OnUIEditGameObject();

	void			contextMenuEvent(QContextMenuEvent * e);
    Void			AddEditEntry( CBankObject& asset, QMenu& subMenu );
    Bool            IsDirectory( const QTreeWidgetItem& item );
    Void            OnProjectLoaded( CProject& p );
    Void            OnProjectCreated( CProject& p );
	Void			OnFBXBindingChange( CFBXDocInterface& colladaInt );
	Void			OnAddObject( const CHandle& parent, const CHandle& h );
	QIcon			GetIcon( const Sb::CTypeInfo& type ) const;

    // Attributes   
	Sb::CRef<CCtrlGame>		m_Ctrl;
	QFileSystemWatcher		m_FileWatcher;
	CViewGame*				m_GameView;

};

};

#endif