#include <Dune\Editor\View\Editor\Particle\ParticleSystem.h>

#include <Sable\Graphics\Particle\Operator\Header.h>

#include <Dune\Editor\View\Editor\Particle\Header.h>
#include <Dune\Bake\Context\Header.h>
#include <Dune\Editor\Ctrl\Editor\Particle\ParticleSystem.h>

#include <QtGui\QInputDialog>

using namespace Dune;

CViewParticleSystem::CViewParticleSystem( CApplication& app, const CHandle& ps  ) :
    CViewEditor( app )
{
    Sb::CSwapChainInfo info( 640, 480, nTextureFormat_R8G8B8A8, nMultiSamplingQuality_None, 60, FALSE );
    m_DisplayWindow = new CEditorDisplayWindow(  *this, info );
    m_RootHandle = ps;
	setContentsMargins( 0,0,0,0 );

	SetupMenu();

	// Left vertical panel
	m_EditorWidget = SetupEditorWidget( );
	
	QHBoxLayout* hLayout = new QHBoxLayout( );
	hLayout->setContentsMargins( 0, 0, 0, 0 );

	hLayout->addWidget( m_EditorWidget );
	hLayout->addWidget( m_DisplayWindow );

	setLayout( hLayout );

	layout()->setMenuBar( m_MenuBar );
}

CViewParticleSystem::~CViewParticleSystem()
{

}

void CViewParticleSystem::showEvent( QShowEvent * event )
{
	if( !m_Ctrl )
	{
		m_Ctrl = NEWOBJ( CCtrlEditorParticleSystem, ( *this, m_RootHandle ) );
		CCtrlEditorParticleSystem& ctrl = (CCtrlEditorParticleSystem&)GetCtrl();
		m_CamCtrl.Initialize( ctrl.GetSableCamera(), 
			m_SignalMouseEventCamCtrl, 
			m_SignalKeyboardEventCamCtrl );

		m_MaxParticleCount->setRange( 1, 1024 * 1024 );
		m_MaxParticleCount->setValue( m_Ctrl->GetParticleSystem()->GetGroup().GetParticleCapacity() );

		CAssetParticleSystem* psAsset = GetApp().GetProject()->GetObj<CAssetParticleSystem>( m_RootHandle );
		if( psAsset )
		{
			m_CurrentFilePath = psAsset->GetFilePath().GetValue();

			Sb::CParticleSystem* system = m_Ctrl->GetParticleSystem();
			Sb::CParticleGroup::OperatorTable::Iterator it;
			const Sb::CParticleGroup::OperatorTable& table = system->GetGroup().GetOperators();
			ForEachItem( it, table )
			{
				Sb::CParticleOperator* pop = (*it);
				const Sb::CTypeInfo& type = pop->GetTypeInfo();
				OnAddOperator( type, pop );
			}
		}
	}
}

CEditorDisplayWindow* CViewParticleSystem::GetDisplayWindow() const 
{
    return m_DisplayWindow;
}

CCtrlEditor&	CViewParticleSystem::GetCtrl() const
{
	return *m_Ctrl;
}

Void CViewParticleSystem::SetupMenu( )
{
	// Setup toolbar row
	m_MenuBar = new QMenuBar( this );
	QAction* action;
	QMenu* menuFile = new QMenu( "&File", this );
	action = menuFile->addAction( "&Open..." );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUILoad() ) );
	action = menuFile->addAction( "&Save As..." );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUISaveAs() ) );
	action = menuFile->addAction( "&Save" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUISave() ) );
	m_MenuBar->addMenu( menuFile );
	QMenu* editFile = new QMenu( "&Edit", this );
	//action = editFile->addAction( "&Enable Snapping" );
	QMenu* viewMenu = new QMenu( "&View", this );
	//action = editFile->addAction( "&Enable Snapping" );
	m_MenuBar->addMenu( editFile );

}

QWidget* CViewParticleSystem::SetupEditorWidget()
{
	QStringList choices;
	QFrame* container = new QFrame( );
    container->setContentsMargins( 0,0,0,0 );
	container->setFrameStyle( QFrame::Sunken | QFrame::Box );

	// Setup component list row
	m_OperatorTree = new QTreeWidget( container );
    m_OperatorTree->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    m_OperatorTree->setIndentation( 0 );
    m_OperatorTree->setHeaderHidden( TRUE );

	// Setup button row
	QHBoxLayout* btnLayout = new QHBoxLayout( );
    btnLayout->setContentsMargins( 0,0,0,0 );
	QPushButton* btn;
	btn =  new QPushButton();
	btn->setText( "Add" );
	connect( btn, SIGNAL( clicked() ), this, SLOT( OnUIAddOperator() ) );
	btnLayout->addWidget( btn );

	btn =  new QPushButton();
	btn->setText( "Remove" );
	connect( btn, SIGNAL( clicked() ), this, SLOT( OnUIRemoveOperator() ) );
	btnLayout->addWidget( btn );

	btn =  new QPushButton();
	btn->setText( "Rename" );
	connect( btn, SIGNAL( clicked() ), this, SLOT( OnUIRenameOperator() ) );
	btnLayout->addWidget( btn );

	// Setup max particle row
	QFormLayout* formLayout = new QFormLayout( );
    formLayout->setContentsMargins( 0,0,0,0 );
	m_MaxParticleCount = new QSpinBox( );
	connect( m_MaxParticleCount, SIGNAL( valueChanged( int ) ), this, SLOT( OnUIMaxParticleCountChanged( int ) ) ) ;
	formLayout->addRow( "Max particle", m_MaxParticleCount );

	// Add every column to a vertical layout
	QVBoxLayout* vLayout = new QVBoxLayout( );
    formLayout->setContentsMargins( 0,0,0,0 );

	vLayout->addLayout( btnLayout );
	vLayout->addLayout( formLayout  );
	vLayout->addWidget( m_OperatorTree );

	container->setLayout( vLayout );

	return container;
}

Bool CViewParticleSystem::OnUIMaxParticleCountChanged( int value )
{
	Sb::CParticleSystem* sys = m_Ctrl->GetParticleSystem();
	if( sys )
	{
		sys->GetGroup().SetParticleCapacity( value );
	}
	return TRUE;
}

Void CViewParticleSystem::OnAddOperator( const Sb::CTypeInfo& type, Sb::CParticleOperator* pop )
{
	if( pop == NULL )
	{
		pop = m_Ctrl->AddOperator( type );
	}

	QWidget* panel = NULL;
	if( type == Sb::CParticleOpSourcePoint::GetStaticTypeInfo() )
	{
		panel = new CPSEmitterPointPanel( *this, (Sb::CParticleOpSourcePoint&)*pop);
	}
	else if( type == Sb::CParticleOpSourceRectangle::GetStaticTypeInfo() )
	{
		panel = new CPSEmitterRectanglePanel( *this, (Sb::CParticleOpSourceRectangle&)*pop);
	}
	else if( type == Sb::CParticleOpTargetSize::GetStaticTypeInfo() )
	{
		panel = new CPSTargetSizePanel( *this, (Sb::CParticleOpTargetSize&)*pop);
	}
	else if( type == Sb::CParticleOpGravity::GetStaticTypeInfo() )
	{
		panel = new CPSGravityPanel( *this, (Sb::CParticleOpGravity&)*pop);
	}
	else if( type == Sb::CParticleOpKillOld::GetStaticTypeInfo() )
	{
		panel = new CPSKillOldPanel( *this, (Sb::CParticleOpKillOld&)*pop);
	}
	else if( type == Sb::CParticleOpTargetColor::GetStaticTypeInfo() )
	{
		panel = new CPSTargetColorPanel( *this, (Sb::CParticleOpTargetColor&)*pop );
	}
	else if( type == Sb::CParticleOpSourceRectangle::GetStaticTypeInfo() )
	{
		panel = new CPSEmitterRectanglePanel( *this, (Sb::CParticleOpSourceRectangle&)*pop );
	}
	else
	{
		DebugError("");
	}

	if( panel )
	{
		QTreeWidgetItem* item = new QTreeWidgetItem();
		QTreeWidgetItem* itemWidget = new QTreeWidgetItem();
		item->addChild( itemWidget );
		item->setText( 0, type.GetTypeName() );
		m_OperatorTree->invisibleRootItem()->addChild( item );
		m_OperatorTree->setItemWidget( itemWidget, 0, panel );
	}
	

	
}

Bool CViewParticleSystem::OnUIAddOperator()
{
	QStringList choices;
	FillOperatorChoices( Sb::CParticleOperator::GetStaticTypeInfo(), choices );
	QInputDialog  dialog( this );

	QString text = QInputDialog::getItem(this, "Add operator", "Type", choices, 0, FALSE );

	if( text.isEmpty() == FALSE )
	{
		QString fullTypeName = "Sable::"+text;
		const Sb::CTypeInfo& info = Sb::CParticleOperator::GetStaticTypeInfo().GetDerivedClass( fullTypeName.toAscii().constData() );
		OnAddOperator( info, NULL );
	}
	return TRUE;
}

Bool CViewParticleSystem::OnUIRemoveOperator()
{
	QList<QTreeWidgetItem*> list = m_OperatorTree->selectedItems();

	QTreeWidgetItem* item;
	Q_FOREACH( item, list )
	{
		CPSOperatorPanel* op = NULL;	
		if( item->parent() == NULL )
		{
			op = (CPSOperatorPanel*) m_OperatorTree->itemWidget( item->child( 0 ), 0 );
		}
		else
		{
			op = (CPSOperatorPanel*) m_OperatorTree->itemWidget( item, 0 );
			item = item->parent();
		}
		
		// We have clicked on a item name/title
		m_Ctrl->RemoveOperator( op->GetOperator() );

		delete item;
	}

	
	return TRUE;
}

Bool CViewParticleSystem::OnUIRenameOperator()
{
	return TRUE;
}

Void CViewParticleSystem::FillOperatorChoices( const Sb::CTypeInfo& type, QStringList& arr )
{
	UInt32 i, j;

	j = type.GetDerivedClassCount();
	for(i=0;i<j;i++)
	{
		if( type.GetDerivedClass( i ).GetDerivedClassCount() )
		{
			FillOperatorChoices( type.GetDerivedClass( i ), arr );
		}
		else
		{
			arr << QString( type.GetDerivedClass( i ).GetTypeName() );
		}
	}
}

Bool CViewParticleSystem::OnUILoad()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open particle system", "s:", "Dune particle system  (*.dpart)", NULL, QFileDialog::DontUseNativeDialog  );

	if( fileName.length() )
	{
		m_CurrentFilePath = Sb::CFilePath( fileName.toAscii().constData() );
		m_Ctrl->SerializeParticleFile( m_CurrentFilePath, nAccesMode_Read );
	}
	return TRUE;
}

Bool CViewParticleSystem::OnUISaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save particle system", "s:", "Dune particle system  (*.dpart)", NULL, QFileDialog::DontUseNativeDialog  );

	if( fileName.length() )
	{
		m_CurrentFilePath = Sb::CFilePath( fileName.toAscii().constData() );
		m_Ctrl->SerializeParticleFile( m_CurrentFilePath, nAccesMode_Write );
	}
	return TRUE;
}

Bool CViewParticleSystem::OnUISave()
{
	if( m_CurrentFilePath.IsValid() )
	{
		m_Ctrl->SerializeParticleFile( m_CurrentFilePath, nAccesMode_Write );
	}
	else
	{
		OnUISaveAs();
	}

	return TRUE;
}


Void CViewParticleSystem::OnMouseLMBUp( const Sb::CVector2f& pos )
{	
	m_SignalMouseEventCamCtrl( nMouseEventType_KeyUp, nMouseEventData_LeftBtn, pos  );
}


Void CViewParticleSystem::OnMouseLMBDown( const Sb::CVector2f& pos )
{
    m_SignalMouseEventCamCtrl( nMouseEventType_KeyDown, nMouseEventData_LeftBtn, pos );
}

Void CViewParticleSystem::OnMouseMove( const Sb::CVector2f& pos )
{
	m_SignalMouseEventCamCtrl( nMouseEventType_Move, nMouseEventData_None, pos  );

}