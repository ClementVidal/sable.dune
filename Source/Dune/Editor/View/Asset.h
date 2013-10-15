#ifndef _DUNE_EDITOR_VIEW_ASSET_
#define _DUNE_EDITOR_VIEW_ASSET_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Ctrl\EventClient.h>
#include <Dune\Editor\Utilities\IconTable.h>
#include <Dune\Editor\View\AssetTree.h>
#include <Dune\Editor\Widget\ObjectLibrary.h>
#include <Dune\Editor\View\FBXLoader.h>

#include <QtGui\QDockWidget>
#include <QtGui\QToolBar>
#include <QtCore\QMimeData>
#include <QtCore\QFileSystemWatcher>

namespace Dune
{

/**
Bank view
*/
class CViewAsset : public QDockWidget, public CEventClient
{

	Q_OBJECT 

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewAsset( CViewMainFrame& mainFrame );
    ~CViewAsset();
    //@}

    /** @name Accesors*/
    //@{
	CViewAssetTree&	GetTree();
	QIcon	GetIcon( const Sb::CTypeInfo& type ) const;
	//@}

    /** @name Manipulator*/
	//@{
	Void			ConfigureAddAssetMenu( QMenu& subMenu );
    //@}

private:

    // friendship

    // Types

    // Methods
	Q_SLOT void		OnUISelectLibraryObject( CLibraryObject& obj );
	Q_SLOT void		OnUIShowAddMenu();
	Q_SLOT void		OnUIAddAsset( );
	Q_SLOT void		OnUIAddAssetsFromFBXFile();
	Q_SLOT void		OnFBXFileLoaded( CFBXDoc* result );
	Q_SLOT void		OnFBXFileChangedOnDisk( const QString& );

	Void			OnFBXBindingChange( CFBXDocInterface& colladaInt );
	Void			OnProjectLoaded( CProject& p );
	Void			OnAddObject( const CHandle& parent, const CHandle& h );
	UInt32			GetIconIdFromType( const Sb::CTypeInfo& info ) const;
	Void			ConfigureAddAssetMenu( QMenu& subMenu, const Sb::CTypeInfo& info, UInt32& id );

    // Attributes   
	CIconTable										m_IconTypeTable;
	CIconTable										m_IconToolsTable;
    CViewAssetTree*									m_Tree;
	QToolBar*										m_ToolBar;
	CObjectLibrary*									m_ObjectLibrary;
	CViewFBXLoader									m_ViewFBXLoader;
	QFileSystemWatcher								m_FileWatcher;
	Sb::CEventSlot< CViewAsset, CFBXDocInterface& >	m_SlotOnFBXBindingChange;
};

};

#endif