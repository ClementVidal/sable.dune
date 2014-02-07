#include <Dune/Editor/View/Asset.h> 

#include <Dune/Resources/Icons/AssetTools.xpm>
#include <Dune/Resources/Icons/AssetType.xpm>
#include <Dune/Editor/Ctrl/Cmd/MergeBank.h>
#include <Dune/Editor/Widget/FBXObjectPicker.h>
#include <Dune/Editor/Ctrl/Cmd/AddObjectFromFBX.h>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

using namespace Dune;

CViewAsset::CViewAsset( CViewMainFrame& mainFrame ) :
    QDockWidget( "Asset Browser", &mainFrame ),
	CEventClient( mainFrame.GetApp() ),
	m_IconToolsTable( (const char**)AssetTools_xpm ),
	m_IconTypeTable( (const char**)AssetType_xpm ),
	m_ViewFBXLoader( mainFrame.GetApp() )
{
	QWidget* content = new QWidget();
	m_ToolBar = new QToolBar( this );
	m_ToolBar->setIconSize( QSize( 16, 16 ) );
	
	m_ObjectLibrary = new CObjectLibrary( GetApp(), "Asset library", CAsset::GetStaticTypeInfo() );
	m_ObjectLibrary->setFloating( TRUE );
	m_ObjectLibrary->hide();
	GetApp().GetMainFrame().addDockWidget( Qt::AllDockWidgetAreas, m_ObjectLibrary );
	connect( m_ObjectLibrary, SIGNAL( OnObjectSelected( CLibraryObject& ) ), this, SLOT( OnUISelectLibraryObject( CLibraryObject& ) ) );


	QAction* objLibToggleAction = m_ObjectLibrary->toggleViewAction();
	objLibToggleAction->setIcon( m_IconToolsTable.GetIcon( 0 ) );
	objLibToggleAction->setToolTip( "Open asset library" );
	GetApp().GetMainFrame().GetViewMenu().addAction( objLibToggleAction );

	m_ToolBar->addAction( objLibToggleAction );

	QAction* addAssetAction = m_ToolBar->addAction( m_IconToolsTable.GetIcon( 1 ), "Add asset" );
	addAssetAction->setToolTip( "Add asset" );
	connect( addAssetAction, SIGNAL( triggered( ) ), this, SLOT( OnUIShowAddMenu( ) ) );

	connect( addAssetAction, SIGNAL( toggled( bool ) ), addAssetAction, SLOT( setChecked( bool ) ) );

    QVBoxLayout* layout = new QVBoxLayout();

	m_Tree = new CViewAssetTree( *this, GetApp() );
	layout->addWidget( m_ToolBar );
	layout->addWidget( m_Tree );

	layout->setContentsMargins( 0, 0, 0, 0 );
	content->setLayout( layout );

	setWidget( content );

	connect( &m_ViewFBXLoader, SIGNAL( OnLoadingDone( CFBXDoc* ) ), this, SLOT( OnFBXFileLoaded( CFBXDoc* ) ) );
	connect( &m_FileWatcher, SIGNAL( fileChanged( const QString& ) ), this, SLOT( OnFBXFileChangedOnDisk( const QString& ) ) );
}

CViewAsset::~CViewAsset()
{

}

void CViewAsset::OnFBXFileLoaded( CFBXDoc* result )
{
	CFBXObjectPicker objectPicker( GetApp(), *result );

	if( objectPicker.exec() )
	{
		UInt32 i;
		const Sb::CArray<CFBXObjectPicker::SResult>& r = objectPicker.GetResult();

		// For each picked objects
		for( i=0; i<r.GetItemCount(); i++ )
		{
			// Add sub asset
			CHandle rootAsset = GetApp().GetProject()->GetRootAsset();

			//COMMAND: AddObject
			CCmd* cmd = NEWOBJ( CCmdAddObjectFromFBX, ( GetApp().GetProjectCtrl(), rootAsset, *r[i].Type, *r[i].FBXObject ) );
			GetApp().GetCommandProcessor().Submit( *cmd );
		}
	}
}

Void CViewAsset::OnProjectLoaded( CProject& p )
{
	Sb::CList<CHandle> list;
	Sb::CList<CHandle>::Iterator it;
	p.IssueQuery( "IsKindOf( CFBXDocInterface )", list );

	ForEachItem( it, list )
	{
		CFBXDocInterface* obj = GetProject()->GetObj<CFBXDocInterface>( it.GetData() );
		if( obj )
		{
			OnFBXBindingChange( *obj );
		}
	}

	CEventClient::OnProjectLoaded( p );
}

Void CViewAsset::OnAddObject( const CHandle& parent, const CHandle& h )
{
	CFBXDocInterface* fbxInterface = GetProject()->GetObj<CFBXDocInterface>( h );
	if( fbxInterface )
	{
		EVENT_CONNECT_SLOT( fbxInterface->SignalFBXBindingChange, m_SlotOnFBXBindingChange, &CViewAsset::OnFBXBindingChange );
	}
	CEventClient::OnAddObject( parent, h );
}

Void CViewAsset::OnFBXBindingChange( CFBXDocInterface& colladaInt )
{
	Sb::CFilePath file = colladaInt.GetFBXDocFilePath().GetValue();
	if( QFile::exists( file.GetPath() ) )
	{
		m_FileWatcher.addPath( file.GetPath() );
	}
}

void CViewAsset::OnFBXFileChangedOnDisk( const QString& )
{
	GetApp().GetEditorContainerView().BakeObject( );
}

Void CViewAsset::ConfigureAddAssetMenu( QMenu& subMenu, const Sb::CTypeInfo& info, UInt32& id )
{
	UInt32 i;

	for( i=0; i < info.GetDerivedClassCount(); i++)
	{
		if( info.GetDerivedClass( i ).GetDerivedClassCount() )
		{
			ConfigureAddAssetMenu( subMenu, info.GetDerivedClass( i ), id );
		}
		else
		{
			const Sb::CTypeInfo& type = info.GetDerivedClass( i );
			QAction* action = subMenu.addAction( type.GetTypeName() );
			connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddAsset() ) );

			action->setIcon( GetIcon( type ) );

			id++;
		}
	}
}

/**
Ask to load a collada file.
Note that loading is done in a other thread.
Whn loading is finished, a signal is emitter by the m_ViewFBXLoader member, and the OnFBXFileLoaded is executed
*/
void CViewAsset::OnUIAddAssetsFromFBXFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Select a file", "s:", "FBX files (*.fbx);;Collada files (*.dae)", NULL, QFileDialog::DontUseNativeDialog );

	if( fileName.length() )
	{
		m_ViewFBXLoader.Load( GetProject()->GetFBXManager(), Sb::CFilePath( fileName.toLocal8Bit().constData() ) );
	}
}

Void CViewAsset::ConfigureAddAssetMenu( QMenu& subMenu )
{
	UInt32 id = 0;

	QAction* action = subMenu.addAction( "From FBX File..." );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddAssetsFromFBXFile() ) );
	subMenu.addSeparator();
	action = subMenu.addAction( CBankObjectDirectory::GetStaticTypeInfo().GetTypeName() );
	action->setIcon( GetIcon( CBankObjectDirectory::GetStaticTypeInfo() ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddAsset() ) );
	subMenu.addSeparator();
	ConfigureAddAssetMenu( subMenu, CAsset::GetStaticTypeInfo(), id );
}

UInt32 CViewAsset::GetIconIdFromType( const Sb::CTypeInfo& info ) const
{
	UInt32 iconId = 0;

	if( info == CAssetTexture2D::GetStaticTypeInfo() )
	{
		iconId = 1;
	}
	else if( info == CAssetTexture3D::GetStaticTypeInfo() )
	{
		iconId = 2;
	}
	else if( info == CAssetModel::GetStaticTypeInfo() )
	{
		iconId = 4;
	}
	else if( info == CAssetMaterial::GetStaticTypeInfo() )
	{
		iconId = 5;
	}
	else if( info == CAssetShader::GetStaticTypeInfo() )
	{
		iconId = 7;
	}
	else if( info == CAssetHelper::GetStaticTypeInfo() )
	{
		iconId = 8;
	}
	else if( info == CAssetFont::GetStaticTypeInfo() )
	{
		iconId = 9;
	}
	else if( info == CAssetAudioBuffer::GetStaticTypeInfo() )
	{
		iconId = 10;
	}
	else if( info == CAssetParticleSystem::GetStaticTypeInfo() )
	{
		iconId = 11;
	}
	else if( info == CAssetStatesSampler::GetStaticTypeInfo() )
	{
		iconId = 12;
	}
	else if( info == CAssetStatesBlend::GetStaticTypeInfo() )
	{
		iconId = 13;
	}
	else if( info == CAssetStatesRasterizer::GetStaticTypeInfo() )
	{
		iconId = 14;
	}
	else if( info == CAssetStatesDepthStencil::GetStaticTypeInfo() )
	{
		iconId = 15;
	}
	else if( info == CAssetStatesBlock::GetStaticTypeInfo() )
	{
		iconId = 16;
	}
	return iconId;
}

QIcon CViewAsset::GetIcon( const Sb::CTypeInfo& type ) const
{
	return m_IconTypeTable.GetIcon( GetIconIdFromType( type ) );
}

void CViewAsset::OnUIAddAsset( )
{
	CBank* currentBank = GetApp().GetProjectCtrl().GetCurrentBank();
	if( !currentBank )
	{
		QMessageBox::warning( this, "Warning", "There is no active bank, please add one before" );
		return;
	}

	QAction* senderAction = (QAction*) sender();

	QString fullTypeName = "Dune::"+senderAction->text();
	QByteArray assetType = fullTypeName.toLocal8Bit();
	const Sb::CTypeInfo& assetTI = CBankObject::GetStaticTypeInfo().GetDerivedClass( assetType.data() );

	//COMMAND: AddObject
	CCmd* cmd = NEWOBJ( CCmdAddObject, ( GetApp().GetProjectCtrl(), GetProject()->GetRootAsset(), assetTI ) );
	GetApp().GetCommandProcessor().Submit( *cmd );

}

void CViewAsset::OnUIShowAddMenu()
{
	QMenu menu;
	ConfigureAddAssetMenu( menu );
	menu.exec( QCursor::pos() );
}

void CViewAsset::OnUISelectLibraryObject( CLibraryObject& obj )
{
	CCtrlProject& ctrl = GetApp().GetProjectCtrl(); 
	DebugAssert( ctrl.GetCurrentBank() );

	CCmdMergeBank* mergeBank = NEWOBJ( CCmdMergeBank, ( ctrl, obj.GetBank(), *ctrl.GetCurrentBank(), ctrl.GetProject()->GetRootAsset() ) );	
	GetApp().GetCommandProcessor().Submit( *mergeBank );	
}

CViewAssetTree&	CViewAsset::GetTree()
{
	return *m_Tree;
}