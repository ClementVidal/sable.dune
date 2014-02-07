#include <Dune/Editor/View/PropertyType.h> 

#include <Dune/Editor/View/PropertyArrayType.h> 
#include <Dune/Editor/Widget/TextEditor.h> 

#include <Dune/Common/Log.h>

#include <QtGui/QColorDialog>

using namespace Dune;

CViewPropertyBase* CViewPropertyBase::CreateViewFromModel( CApplication* app, const CPropertyInterface& p  )
{
    CViewPropertyBase* wdg = NULL;

    if( p.GetTypeInfo() == CPropertyInt32::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyInt32( app, (CPropertyInt32*)&p );
    }
    else if( p.GetTypeInfo() == CPropertyFloat32::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyFloat32( app, (CPropertyFloat32*)&p );
    }
	else if( p.GetTypeInfo() == CPropertyBool::GetStaticTypeInfo() )
	{
		wdg = new CViewPropertyBool( app, (CPropertyBool*)&p );
	}
    else if( p.GetTypeInfo() == CPropertyVector3f::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyVector3f( app, (CPropertyVector3f*) &p );
    }
	else if( p.GetTypeInfo() == CPropertyVector4f::GetStaticTypeInfo() )
	{
		wdg = new CViewPropertyVector4f( app, (CPropertyVector4f*) &p );
	}
    else if( p.GetTypeInfo() == CPropertyVector2f::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyVector2f( app, (CPropertyVector2f*) &p );
    }
	else if( p.GetTypeInfo() == CPropertyVector2i::GetStaticTypeInfo() )
	{
		wdg = new CViewPropertyVector2i( app, (CPropertyVector2i*) &p );
	}
    else if( p.GetTypeInfo() == CPropertyFilePath::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyFilePath( app, (CPropertyFilePath*) &p );
    }
    else if( p.GetTypeInfo() == CPropertyString::GetStaticTypeInfo() )
    {
        wdg =  new CViewPropertyString( app, (CPropertyString*) &p );
    }
    else if( p.GetTypeInfo() == CPropertyHandle::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyHandle( app, (CPropertyHandle*) &p );
    }
    else if( p.GetTypeInfo() == CPropertyArray::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyArray( app, (CPropertyArray*) &p );
    }
    else if( p.GetTypeInfo() == CPropertyColor::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyColor( app, (CPropertyColor*) &p );
    }
    else if( p.GetTypeInfo() == CPropertyEnum::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyEnum( app, (CPropertyEnum*)&p );
    }
    else if( p.GetTypeInfo() == CPropertyMatrix4x4::GetStaticTypeInfo() )
    {
        wdg = new CViewPropertyMatrix4x4( app, (CPropertyMatrix4x4*)&p );
    }

    return wdg;
}

CViewPropertyBase::CViewPropertyBase( CApplication* app, CPropertyInterface* p ):
	QWidget( )
{
	m_Property = p;
	TreeItem =new QTreeWidgetItem();
	setAutoFillBackground( TRUE );
	m_Layout = new QHBoxLayout( this );
	m_ValueLineEdit = new QLineEdit( this );
    m_ValueLineEdit->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	connect( m_ValueLineEdit, SIGNAL( editingFinished() ), this, SLOT( OnUILineEditChanged() ) );
	m_ValueLineEdit->setText( p->GetValueAsString().GetBuffer() );
	m_Layout->setContentsMargins( 0, 0, 0, 0 );
	m_Layout->setSpacing( 0 );
	m_Layout->addWidget( m_ValueLineEdit );
	setLayout( m_Layout );
    setToolTip( m_ValueLineEdit->text() );

	TreeItem->setText( 0, p->GetName() );
	m_App = app;

    setMinimumHeight( 20 );

	EVENT_CONNECT_SLOT( p->SignalPropertyChanged, m_SlotPropertyChanged, &CViewPropertyBase::OnModelPropertyChanged );
}

CViewPropertyBase::~CViewPropertyBase()
{

}

Void CViewPropertyBase::Initialize( QTreeWidget& tree, QTreeWidgetItem& root, QWidget* widgetToAdd )
{
    // Add child 
    root.addChild( TreeItem );
    if( widgetToAdd == NULL )
        tree.setItemWidget( TreeItem, 1, this );
    else
        tree.setItemWidget( TreeItem, 1, widgetToAdd );

    TreeItem->setBackgroundColor( 0, QColor( 200, 200, 200 ) );

    Enable( m_Property->GetIsEnabled() );

}

Void CViewPropertyBase::OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	if( m_ValueLineEdit )
    {
		m_ValueLineEdit->setText( m_Property->GetValueAsString().GetBuffer() );
        setToolTip( m_ValueLineEdit->text() );
    }
}

Bool CViewPropertyBase::OnUILineEditChanged( )
{
	QByteArray tmp = m_ValueLineEdit->text().toLocal8Bit();
    setToolTip( m_ValueLineEdit->text() );
    if( m_Property->GetValueAsString() != tmp.constData() )
    {
        CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, Sb::CString( tmp.data() ) ) );
	    m_App->GetCommandProcessor().Submit( *cmd );
    }

	return TRUE;
}

Void CViewPropertyBase::Enable( Bool onOff )
{
	setEnabled( onOff );
	if( onOff )
	{
		TreeItem->setFlags( Qt::ItemIsEnabled );
	}
	else 
	{
		TreeItem->setFlags( Qt::NoItemFlags );
	}
}

/**
Custom Sb::CString property
*/
CViewPropertyString::CViewPropertyString( CApplication* app, CPropertyString* value ) : 
    CViewPropertyBase( app, value )
{
	setToolTip( value->GetValue().GetBuffer() );

	if( value->GetType() == CPropertyString::nType_SingleLine )
	{
	}
	else if( value->GetType() == CPropertyString::nType_MultiLine )
	{
		delete m_ValueLineEdit;
		m_ValueLineEdit = NULL;

		m_Button = new QPushButton();
		connect( m_Button, SIGNAL( clicked() ), this, SLOT( OnUIBtnClicked() ) );
		m_Button->setText("Edit Text");
		m_Layout->addWidget(m_Button);
	}
	else if( value->GetType() == CPropertyString::nType_Script )
	{
		delete m_ValueLineEdit;
		m_ValueLineEdit = NULL;

		m_Button = new QPushButton();
		connect( m_Button, SIGNAL( clicked() ), this, SLOT( OnUIBtnClicked() ) );
		m_Button->setText("Edit Script");
		m_Layout->addWidget(m_Button);
	}
	else if( value->GetType() == CPropertyString::nType_Shader )
	{
		delete m_ValueLineEdit;
		m_ValueLineEdit = NULL;

		m_Button = new QPushButton();
		connect( m_Button, SIGNAL( clicked() ), this, SLOT( OnUIBtnClicked() ) );
		m_Button->setText("Edit Shader");
		m_Layout->addWidget(m_Button);
	}
}

CViewPropertyString::~CViewPropertyString()
{
}


Bool CViewPropertyString::OnUIBtnClicked()
{
	CPropertyString* ps = (CPropertyString*) m_Property;
	if( ps->GetType() == CPropertyString::nType_Shader ||
		ps->GetType() == CPropertyString::nType_Script ||
		ps->GetType() == CPropertyString::nType_MultiLine )
	{
		CTextEditor textEdit( *m_App );

		textEdit.SetText( ps->GetValue() );
		if( textEdit.exec() )
		{
			ps->SetValue( textEdit.GetText() );
			setToolTip( ps->GetValue().GetBuffer() );
		}
	}

	return TRUE;
}

/**
Custom Float32 property
*/
CViewPropertyFloat32::CViewPropertyFloat32( CApplication* app, CPropertyFloat32* value ) : 
	CViewPropertyBase( app, value )
{
	m_SpinBox = new CScalarWidget( *this, value->GetValue() );
    m_SpinBox->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Layout->addWidget(m_SpinBox);	

    connect( m_SpinBox, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
}

CViewPropertyFloat32::~CViewPropertyFloat32()
{
}

void CViewPropertyFloat32::OnUIValueChanged( )
{
    CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, ( Float32 )m_SpinBox->GetValueAsFloat() ) );
    m_App->GetCommandProcessor().Submit( *cmd );
}


/**
Custom UInt32 property
*/

CViewPropertyInt32::CViewPropertyInt32( CApplication* app, CPropertyInt32* value ) : 
    CViewPropertyBase( app, value )
{
	m_SpinBox = new CScalarWidget( *this,  (Int32)value->GetValue() );
    m_SpinBox->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );

	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Layout->addWidget( m_SpinBox );	

    connect( m_SpinBox, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
}

CViewPropertyInt32::~CViewPropertyInt32()
{
}

void CViewPropertyInt32::OnUIValueChanged( )
{
    CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, (Int32 )m_SpinBox->GetValueAsInt() ) );
    m_App->GetCommandProcessor().Submit( *cmd );
}

/**
Custom Vector2 property
*/

CViewPropertyVector2f::CViewPropertyVector2f( CApplication* app, CPropertyVector2f* value ) : 
    CViewPropertyBase( app, value )
{
}

CViewPropertyVector2f::~CViewPropertyVector2f()
{
}

/**
Custom Vector2i property
*/

CViewPropertyVector2i::CViewPropertyVector2i( CApplication* app, CPropertyVector2i* value ) : 
    CViewPropertyBase( app, value )
{
}

CViewPropertyVector2i::~CViewPropertyVector2i()
{
}


/**
Custom Matrix4x4 property
*/

CViewPropertyMatrix4x4::CViewPropertyMatrix4x4( CApplication* app, CPropertyMatrix4x4* value ) : 
    CViewPropertyBase( app, value )
{
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Widget = new CMatrix4x4Widget( *this, value->GetValue() );
    m_Widget->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	connect( m_Widget, SIGNAL( OnValueChanged() ), this, SLOT( OnUIValueChanged() ) );
    m_Layout->addWidget( m_Widget );
}

CViewPropertyMatrix4x4::~CViewPropertyMatrix4x4()
{
}

Bool CViewPropertyMatrix4x4::OnUIValueChanged()
{
	Sb::CString newValue = CLexicalCast::ValueToString( m_Widget->GetValue() );
	setToolTip( newValue.GetBuffer() );
	
	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, newValue ) );
    m_App->GetCommandProcessor().Submit( *cmd );
    return TRUE;
}

Void CViewPropertyMatrix4x4::OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  )
{
	setToolTip( m_Property->GetValueAsString().GetBuffer() );
    // Set the value of the sub widget but do note trigger a OnValueChanged signal
    m_Widget->SetValue( ((CPropertyMatrix4x4*)m_Property)->GetValue(), FALSE );
}

/**
Custom Vector3 property
*/

CViewPropertyVector3f::CViewPropertyVector3f( CApplication* app, CPropertyVector3f* value ) : 
    CViewPropertyBase( app, value )
{
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Widget = new CVector3Widget( *this, value->GetValue() );
    m_Widget->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	connect( m_Widget, SIGNAL( OnValueChanged() ), this, SLOT( OnUIValueChanged() ) );
    m_Layout->addWidget( m_Widget );
}

CViewPropertyVector3f::~CViewPropertyVector3f()
{
}

Bool CViewPropertyVector3f::OnUIValueChanged()
{
	Sb::CString newValue = CLexicalCast::ValueToString( m_Widget->GetValue() );
	setToolTip( newValue.GetBuffer() );
	
	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, newValue ) );
    m_App->GetCommandProcessor().Submit( *cmd );
    return TRUE;
}

Void CViewPropertyVector3f::OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	setToolTip( m_Property->GetValueAsString().GetBuffer() );
    // Set the value of the sub widget but do note trigger a OnValueChanged signal
    m_Widget->SetValue( ((CPropertyVector3f*)m_Property)->GetValue(), FALSE );
}

/**
Custom Vector3 property
*/

CViewPropertyVector4f::CViewPropertyVector4f( CApplication* app, CPropertyVector4f* value ) : 
    CViewPropertyBase( app, value )
{
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Widget = new CVector4Widget( *this, value->GetValue() );
    m_Widget->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	connect( m_Widget, SIGNAL( OnValueChanged() ), this, SLOT( OnUIValueChanged() ) );
    m_Layout->addWidget( m_Widget );
}

CViewPropertyVector4f::~CViewPropertyVector4f()
{
}

Bool CViewPropertyVector4f::OnUIValueChanged()
{
	Sb::CString newValue = CLexicalCast::ValueToString( m_Widget->GetValue() );
	setToolTip( newValue.GetBuffer() );
	
	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, newValue ) );
    m_App->GetCommandProcessor().Submit( *cmd );
    return TRUE;
}

Void CViewPropertyVector4f::OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	setToolTip( m_Property->GetValueAsString().GetBuffer() );
    // Set the value of the sub widget but do note trigger a OnValueChanged signal
    m_Widget->SetValue( ((CPropertyVector4f*)m_Property)->GetValue(), FALSE );
}

/**
CHandle property
*/

CViewPropertyFilePath::CViewPropertyFilePath( CApplication* app, CPropertyFilePath* value ) : 
    CViewPropertyBase( app, value )
{
	m_Button = new QPushButton();
	connect( m_Button, SIGNAL( clicked() ), this, SLOT( OnUIBtnClicked() ) );
	m_Button->setText("...");
	m_Button->setMaximumWidth( 15 );
	m_Button->setMinimumWidth( 15 );
	m_Layout->addWidget(m_Button);
}

CViewPropertyFilePath::~CViewPropertyFilePath()
{
}

Bool CViewPropertyFilePath::OnUIBtnClicked()
{
	QString dir = "s:";
	const Sb::CFilePath& path = ((CPropertyFilePath*)m_Property)->GetValue();
	if( path.Exist() )
	{
		Char tmp[256];
		path.GetDirectory( tmp, 256 );
		dir = tmp;
	}

	QString fileName = QFileDialog::getOpenFileName(this, "Open file", dir, ((CPropertyFilePath*)m_Property)->GetWildcard(), NULL, QFileDialog::DontUseNativeDialog  );

	if( fileName.length() )
	{
		QByteArray arr = fileName.toLocal8Bit();
        Sb::CFilePath newPath( arr.data() );

        CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, Sb::CString( newPath.GetBuffer() ) ) );
        if( m_App->GetCommandProcessor().Submit( *cmd ) )
        {
            return TRUE;
        }
	}

	return FALSE;
}

/**
CHandle property
*/

CViewPropertyHandle::CViewPropertyHandle( CApplication* app, CPropertyHandle* value ) : 
	CViewPropertyBase( app, value )
{
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;

	m_Button = new QPushButton();
    m_DisplayName = new QLineEdit();
    m_DisplayName->setReadOnly( TRUE );
    m_DisplayName->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );

	connect( m_Button, SIGNAL( clicked() ), this, SLOT( OnUIBtnClicked() ) );
	m_Button->setText("...");
	m_Button->setMaximumWidth( 15 );
	m_Button->setMinimumWidth( 15 );
	m_Layout->addWidget(m_Button);
    m_Layout->insertWidget( 0, m_DisplayName );

    if( value->GetValue().IsValid() )
	{
		CBankObject* obj = app->GetProject()->GetObj( value->GetValue() );
		if( obj )
			m_DisplayName->setText( obj->GetName() );
		else
			m_DisplayName->setText( "NULL" );
	}
    else
	{
        m_DisplayName->setText( "NULL" );
	}
}

CViewPropertyHandle::~CViewPropertyHandle()
{
}

Bool CViewPropertyHandle::OnUIBtnClicked()
{
	Bool b;
	QMenu menu( this );

	QAction* action = NULL;
    if( ((CPropertyHandle*)m_Property)->GetValue().IsValid() )
    {
		action = menu.addAction( "Edit..."  );
		b = connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditAsset( ) ) );
    }
	else
	{
		QMenu* subMenu = menu.addMenu("Add...");
		CPropertyHandle::FilterList::Iterator it;
		ForEachItem( it, ((CPropertyHandle*)m_Property)->GetFilterList() )
		{
			action = subMenu->addAction( (*it)->GetTypeName() );
			b = connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddAsset( ) ) );
		}
	}

    menu.addSeparator();
	action = menu.addAction( "Bind..."  );
	b = connect( action, SIGNAL( triggered() ), this, SLOT( OnUIBindAsset( ) ) );
	action = menu.addAction( "UnBind"  );
	b = connect( action, SIGNAL( triggered() ), this, SLOT( OnUIUnbindAsset( ) ) );

	menu.exec( QCursor::pos() );

	return TRUE;
}

Bool CViewPropertyHandle::OnUIAddAsset()
{
	QAction* senderAction = (QAction*) sender();

	QString fullTypeName = "Dune::"+senderAction->text();
	QByteArray assetType = fullTypeName.toLocal8Bit();
	const Sb::CTypeInfo& assetTI = CBankObject::GetStaticTypeInfo().GetDerivedClass( assetType.data() );

	CCmdMeta* meta = NEWOBJ( CCmdMeta, ( "Add and assign asset" ) );

	//COMMAND: AddObject
	meta->PushCommand( * NEWOBJ( CCmdAddObject, ( m_App->GetProjectCtrl(), m_App->GetProject()->GetRootAsset(), assetTI ) ) );
/*
	Sb::CString s = CLexicalCast::ValueToString( CHandle );
	meta->PushCommand( * NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, s ) ) );
*/
	m_App->GetCommandProcessor().Submit( *meta );

	return TRUE;
}

Bool CViewPropertyHandle::OnUIEditAsset( )
{
    CBankObject* currentObj = m_App->GetProject()->GetObj( ((CPropertyHandle*)m_Property)->GetValue() );

	DebugAssert( currentObj );

    CCmdMeta* meta = NEWOBJ( CCmdMeta, ( ((Sb::CString)"SelectObject: ") + currentObj->GetName() ) );

    CCtrlProject& prjCtrl = m_App->GetProjectCtrl();
    Sb::CArray<CHandle> objList( prjCtrl.GetSelectedObjects() );
    meta->PushCommand( * NEWOBJ( CCmdSelectObject, ( prjCtrl, objList.GetBuffer(), objList.GetItemCount(), CCmdSelectObject::nAction_DeSelect) ) );
    meta->PushCommand( * NEWOBJ( CCmdSelectObject, ( prjCtrl, currentObj->GetHandle(), CCmdSelectObject::nAction_Select ) ) );
    m_App->GetCommandProcessor().Submit( *meta );	

    return TRUE;
}

Bool CViewPropertyHandle::OnUIBindAsset( )
{
	CWidgetObjectPicker::SConfig config;
	config.Filter = ((CPropertyHandle*)m_Property)->GetFilterList();

	CWidgetObjectPicker picker( *m_App, *m_App->GetProject(), &config );
	if( picker.exec() )
	{
		Sb::CString s = CLexicalCast::ValueToString( picker.GetAsset() );
		CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, s ) );
		if( m_App->GetCommandProcessor().Submit( *cmd ) )
        {
            if( picker.GetAsset().IsValid() )
			{
				CBankObject* pickedObject = m_App->GetProject()->GetObj( picker.GetAsset() );
                m_DisplayName->setText( pickedObject->GetName() );
			}
            else
			{
                m_DisplayName->setText( "NULL" );
			}
        }

	}

	return TRUE;	
}

Bool CViewPropertyHandle::OnUIUnbindAsset(  )
{
	CHandle h;
	Sb::CString s = CLexicalCast::ValueToString( h );
	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, s ) );
	if( m_App->GetCommandProcessor().Submit( *cmd ) )
    {
        m_DisplayName->setText( "NULL" );
    }

	return TRUE;
}

/**
Boolean property
*/
CViewPropertyBool::CViewPropertyBool( CApplication* app, CPropertyBool* value ) : 
    CViewPropertyBase( app, value )
{
	m_CheckBox = new QCheckBox();
	connect( m_CheckBox, SIGNAL( stateChanged( int ) ), this, SLOT( OnUICheckBoxClicked( int ) ) );
	m_CheckBox->setMaximumWidth( 25 );
	m_CheckBox->setMinimumWidth( 25 );
	m_CheckBox->setCheckState( value->GetValue() ? Qt::Checked : Qt::Unchecked );
    m_Layout->addWidget(m_CheckBox, 0, Qt::AlignCenter );
}

CViewPropertyBool::~CViewPropertyBool()
{
}

Bool CViewPropertyBool::OnUICheckBoxClicked( int state )
{
	Bool v;
	if( state == Qt::Unchecked )
	{
		v = FALSE;
	}
	else
	{
		v = TRUE;
	}

	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, v ) );
	m_App->GetCommandProcessor().Submit( *cmd );

	return TRUE;
}

/**
Custom Color property
*/
CViewPropertyColor::CViewPropertyColor( CApplication* app, CPropertyColor* value ) : 
    CViewPropertyBase( app, value )
{
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Widget = new CColorWidget( *this, value->GetValue() );
    m_Widget->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	connect( m_Widget, SIGNAL( OnValueChanged() ), this, SLOT( OnUIValueChanged() ) );
	m_Layout->addWidget( m_Widget );
}

CViewPropertyColor::~CViewPropertyColor()
{
}

Bool CViewPropertyColor::OnUIValueChanged()
{
	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, m_Widget->GetValue() ) );
	m_App->GetCommandProcessor().Submit( *cmd );
	return TRUE;
}

/**
Custom Enum property
*/
CViewPropertyEnum::CViewPropertyEnum( CApplication* app, CPropertyEnum* value ) : 
    CViewPropertyBase( app, value )
{
    DebugAssert( value );
	delete m_ValueLineEdit;
	m_ValueLineEdit = NULL;
	m_Combo = new QComboBox();
    m_Combo->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	connect( m_Combo, SIGNAL( activated( int ) ), this,SLOT( OnUIComboBoxChanged( int ) ) );
	m_Layout->addWidget( m_Combo );

    const CPropertyEnum::CDef& def = value->GetDefinition();

    UInt32 i, index = 0;
    for(i=0;i<def.GetEntryCount();i++ )
    {
		if( def.GetEntryValue( i ) == value->GetValue() )
			index = i;
		m_Combo->addItem( def.GetEntryName( i ), QVariant( def.GetEntryValue( i ) ) );
    }

	m_Combo->setCurrentIndex( index );
}

CViewPropertyEnum::~CViewPropertyEnum()
{
}

Bool CViewPropertyEnum::OnUIComboBoxChanged( int index )
{
	Sb::CString text = m_Combo->itemText( index ).toAscii().constData();

	CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( m_App->GetProjectCtrl(), *m_Property, text ) );
	m_App->GetCommandProcessor().Submit( *cmd );

	return TRUE;
}