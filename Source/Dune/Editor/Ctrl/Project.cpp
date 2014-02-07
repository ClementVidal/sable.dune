#include <Dune/Editor/Ctrl/Project.h>

#include <Dune/Editor/View/Asset.h>
#include <Dune/Editor/View/Scene.h>
#include <Dune/Editor/View/UI.h>
#include <Dune/Editor/Ctrl/CommandProcessor.h>
#include <Dune/Model/Project/Project.h>
#include <Dune/Model/Asset/Asset.h>
#include <Dune/Model/Gui/Widget.h>
#include <Dune/Common/XMLDataLayout.h>
#include <Dune/Model/Asset/FBX/Interface.h>
#include <Dune/Model/Property/PropertyArray.h>
#include <Dune/Model/Scene/World.h>
#include <Dune/Editor/Ctrl/Editor/UI/UI.h>
#include <Dune/Editor/View/Editor/UI/UI.h>
#include <Dune/Editor/Ctrl/Editor/Scene/Scene.h>
#include <Dune/Editor/View/Editor/Scene/Scene.h>
#include <Dune/Editor/Widget/ShaderEditor.h> 
#include <Dune/Editor/Widget/FontEditor.h> 
#include <Dune/Editor/Widget/GameObjectEditor.h> 
#include <Dune/Editor/Widget/ImageViewer.h> 
#include <Dune/Editor/Widget/MaterialEditor.h> 
#include <Dune/Editor/View/Editor/Particle/ParticleSystem.h>

#include <Sable/Core/Log/Logger.h>

using namespace Dune;

CCtrlProject::CCtrlProject( CApplication& app ) :
    CCtrl( app )
{
    m_Model = NULL;
}

CCtrlProject::~CCtrlProject()
{
    SignalCurrentBankChanged.DisconnectAll();
}

CProject*	CCtrlProject::GetProject() const
{
	return m_Model;
}

Bool CCtrlProject::IsObjectSelected( const CHandle& obj ) const
{
	return m_SelectedObject.HasItem( obj );
}

Void CCtrlProject::SelectObjects( Bool selectOrDeselect )
{
	ObjectList::Iterator it;
	ForEachItem( it, m_SelectedObject )
	{
		SignalObjectDeSelected( *it );
	}
	m_SelectedObject.ClearAllItems();
}

Void CCtrlProject::SelectObjects( const CHandle& current, Bool selectOrDeselect )
{
	if( selectOrDeselect )
	{
		if( ! m_SelectedObject.HasItem( current ) )
		{
			m_SelectedObject.AddItemAtEnd( current );
			SignalObjectSelected( current );
		}
	}
	else
	{
		if( m_SelectedObject.HasItem( current ) )
		{	
			m_SelectedObject.RemoveItem( current  );
			SignalObjectDeSelected( current );
		}
	}
}

Void CCtrlProject::SelectObjects( const Sb::CArray<CHandle>& current, Bool selectOrDeselect )
{
	UInt32 i;

	for(i=0;i<current.GetItemCount();i++)
	{
		if( selectOrDeselect )
		{
			if( ! m_SelectedObject.HasItem( current[i] ) )
			{
				m_SelectedObject.AddItemAtEnd( current[i] );
				SignalObjectSelected( current[i] );
			}
		}
		else
		{
			if( m_SelectedObject.HasItem( current[i] ) )
			{	
				m_SelectedObject.RemoveItem( current[i] );
				SignalObjectDeSelected( current[i] );
			}
		}
	}
}

const CCtrlProject::ObjectList&	CCtrlProject::GetSelectedObjects()const
{
	return m_SelectedObject;
}

Void CCtrlProject::Destroy()
{
    if( m_Model )
	{
		// Deselect every object before cleaning everything...
		SelectObjects( FALSE );

		GetCommandProcessor().Reset();

        SignalProjectDestroyed();

		DebugAssert( m_Model->GetRefCount() == 1 );
        m_Model = NULL;

        m_CurrentBank = CUuid();
	}

    m_Model = NULL;
}

CProject* CCtrlProject::Create(const Sb::CFilePath& path )
{
    DebugAssert( m_Model == NULL );
    m_Model = NEWOBJ( CProject, ( path ) );

    SetCurrentBank( CUuid() );

    EVENT_CONNECT_SLOT( m_Model->SignalBankAdded, m_SlotBankAdded, &CCtrlProject::OnBankAdded );
    EVENT_CONNECT_SLOT( m_Model->SignalBankRemoved, m_SlotBankRemoved, &CCtrlProject::OnBankRemoved );

    SignalProjectCreated( *m_Model );

    return m_Model;
}

Void CCtrlProject::OnEditElement( const CHandle& h )
{
	if( GetProject()->GetObj<CWidget>( h ) )
	{
		CViewEditorContainer& editorView = GetApp().GetEditorContainerView();
		editorView.RemoveAllEditors();

		CViewEditorUI* editor = new CViewEditorUI( GetApp(), h );
		editorView.AddEditor( *editor, "Gui" );
	}
	else if( GetProject()->GetObj<CSceneWorld>( h ) )
	{
		CViewEditorContainer& editorView = GetApp().GetEditorContainerView();
		editorView.RemoveAllEditors();

		CViewEditorScene* editor = new CViewEditorScene( GetApp(), h );
		editorView.AddEditor( *editor, "Scene" );
	}
	else if( GetProject()->GetObj<CAssetTexture2D>( h ) )
	{
		CAssetTexture2D& texture = *GetProject()->GetObj<CAssetTexture2D>( h );
		if( !texture.GetIsEdited() )
		{
			CImageViewer* editor = new CImageViewer( GetApp(), texture );
			GetApp().GetMainFrame().addDockWidget( Qt::AllDockWidgetAreas, editor  );
			editor->setFloating( TRUE );
			editor->show();
			texture.SetIsEdited( TRUE );
		}
	}
	else if( GetProject()->GetObj<CAssetShader>( h ) )
	{
		CAssetShader& shader = *GetProject()->GetObj<CAssetShader>( h );
		if( !shader.GetIsEdited() )
		{
			CShaderEditor* editor = new CShaderEditor( GetApp(), shader );
			GetApp().GetMainFrame().addDockWidget( Qt::AllDockWidgetAreas, editor  );
			editor->setFloating( TRUE );
			editor->show();
			shader.SetIsEdited( TRUE );
		}
	}
	else if( GetProject()->GetObj<CAssetFont>( h ) )
	{
		CAssetFont& font = *GetProject()->GetObj<CAssetFont>( h );
		if( !font.GetIsEdited() )
		{
			CFontEditor* editor = new CFontEditor( GetApp(), font );
			GetApp().GetMainFrame().addDockWidget( Qt::AllDockWidgetAreas, editor  );
			editor->setFloating( TRUE );
			editor->show();
			font.SetIsEdited( TRUE );
		}
	}
	else if( GetProject()->GetObj<CAssetParticleSystem>( h ) )
	{
		CAssetParticleSystem& ps = *GetProject()->GetObj<CAssetParticleSystem>( h );
		if( !ps.GetIsEdited() )
		{
			CViewEditorContainer& editorView = GetApp().GetEditorContainerView();
			editorView.RemoveAllEditors();

			CViewParticleSystem* editor = new CViewParticleSystem( GetApp(), ps.GetHandle() );
			editorView.AddEditor( *editor, "ParticleSystem" );

			ps.SetIsEdited( TRUE );
		}
	}
	else if( GetProject()->GetObj<CAssetMaterial>( h ) )
	{
		CAssetMaterial& material = *GetProject()->GetObj<CAssetMaterial>( h );
		CMaterialEditor editor( GetApp(), material );
		editor.exec();
	}
	else if( GetProject()->GetObj<CGameObjectPrototype>( h ) )
	{
		CGameObjectPrototype& gameObject = *GetProject()->GetObj<CGameObjectPrototype>( h );
		CGameObjectEditor editor( GetApp(), gameObject );
		editor.exec();
	}
}


Bool CCtrlProject::Serialize( Sb::CPersistentArchive& ar )
{
	if( ar.Serialize(*m_Model, WT("Project") ) )
	{
		ar.FlushData();
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

CProject* CCtrlProject::Save( const Sb::CFilePath& path  )
{
    Sb::CFilePath finalPath;
    
    if( m_Model == NULL )
        return NULL;

    if( path.IsValid() == FALSE )
    {
        finalPath = m_Model->GetProjectFilePath();
    }
    else
    {
        finalPath = path;
    }

    Sb::CXMLDataLayout      dataLayout( finalPath );
    Sb::CPersistentArchive ar(dataLayout, nAccesMode_Write );

	CoreManager.GetLog().LogInfo( "Saving project: %s", finalPath.GetPath() );
	Serialize( ar );

    return NULL;
}

/**
Utility function used to validate and correct the content of the Parent parameter.
This is only usefull in case parent ship has been broken by some kind of manipulation
*/
Void CCtrlProject::RebuildParentRelation( const CHandle& parent )
{
    UInt32 i=0;
    CBankObject* parentObj = GetProject()->GetObj<CBankObject>( parent );

    if( parentObj )
    {
        for(i=0;i<parentObj->GetChildCount();i++)
        {   
            CBankObject* childObj = GetProject()->GetObj<CBankObject>( parentObj->GetChildAtIndex(i) );
            if( childObj )
            {
                childObj->SetParent( parent );
                RebuildParentRelation( childObj->GetHandle() );
            }
        }
    }
}

Void CCtrlProject::OnObjectAdded( CBankObject& b )
{
	EVENT_CONNECT_SIGNAL( b.SignalPropertyChanged, SignalObjectPropertyChanged );
	EVENT_CONNECT_SIGNAL( b.SignalChildIndexChanged, SignalObjectChildIndexChanged );		
	EVENT_CONNECT_SIGNAL( b.SignalChildAdded, SignalObjectChildAdded );

    SignalObjectAdded( b.GetParent(), b.GetHandle() );
}

Void CCtrlProject::OnObjectRemoved( CBankObject& b )
{
    b.SignalPropertyChanged.Disconnect( SignalObjectPropertyChanged );
}

Void CCtrlProject::OnBankAdded( CBank& b )
{
    CBank::Iterator it;
    // Connect all preexisting element to SignalObjectPropertyChanged 
    ForEachItem( it, b )
    {
		EVENT_CONNECT_SIGNAL( it.GetData().SignalPropertyChanged, SignalObjectPropertyChanged );
		EVENT_CONNECT_SIGNAL( it.GetData().SignalChildIndexChanged, SignalObjectChildIndexChanged );
		EVENT_CONNECT_SIGNAL( it.GetData().SignalChildAdded, SignalObjectChildAdded );
    }

    // Connect to know when object are added/removed
    EVENT_CONNECT_SLOT( b.SignalObjectAdded, m_SlotObjectAdded, &CCtrlProject::OnObjectAdded );
    EVENT_CONNECT_SLOT( b.SignalObjectRemoved, m_SlotObjectRemoved, &CCtrlProject::OnObjectRemoved );
}

Void CCtrlProject::OnBankRemoved( CBank& b )
{
    b.SignalObjectAdded.Disconnect( m_SlotObjectAdded );
    b.SignalObjectRemoved.Disconnect( m_SlotObjectRemoved );
}

CProject* CCtrlProject::Load( const Sb::CFilePath& path  )
{
    DebugAssert( m_Model );
	if( path.Exist() == FALSE )
		return NULL;

    CoreManager.GetLog().LogInfo( "Loading project: %s", path.GetPath() );

	Sb::CXMLDataLayout      dataLayout( path );
	Sb::CPersistentArchive ar(dataLayout, nAccesMode_Read );

	if( Serialize( ar ))
	{
		SignalProjectLoaded( *m_Model );
		CoreManager.GetLog().LogInfo( "Loading done" );

		return m_Model;
	}
	else
	{
		CoreManager.GetLog().LogError( "Failled to load project !" );
		Destroy();
	}

	return NULL;
}


/**
Entry point used to remove an element
*/
Void CCtrlProject::RemoveObject( const CHandle& h, Bool freeHandle  )
{
	// Detach this object from it's parent
	CBankObject* obj = GetProject()->GetObj<CBankObject>( h );
	if( obj )
		obj->SetParent( CHandle() );

	SignalObjectRemove( h );
	CBank* bank = GetApp().GetProject()->GetBank( h.GetBankUuid() );
	DebugAssert( bank );
	bank->RemoveObject( h, freeHandle );
}

/**
Entry point used to create a new element from an existing object and a reserved handle
*/
CHandle CCtrlProject::AddObject( const CHandle& parent, CHandle& reservedHandle, CBankObject& obj ) 
{
	GetCurrentBank()->BindObject( reservedHandle, obj );

	DebugAssert( obj.GetTypeInfo().IsKindOf( CBankObject::GetStaticTypeInfo() ) );
	
	((CBankObject*) &obj)->SetParent( parent );

	return reservedHandle;
}

/**
Entry point used to create a new element of type type
*/
CHandle CCtrlProject::AddObjectFromFBX( const CHandle& parent, const Sb::CTypeInfo& type, CFBXObject& fbxObject ) 
{
	// Create the NEW element
	DebugAssert( type.IsKindOf( CFBXDocInterface::GetStaticTypeInfo() ) );

	CFBXDocInterface* fbxInterface = (CFBXDocInterface*)GetApp().GetProjectCtrl().GetCurrentBank()->AddObject( type, parent );

	fbxInterface->SetName( fbxObject.GetName() );

	if( fbxInterface )
	{
		Sb::DebugLogInfo( "Add Object from FBX: (%s)", type.GetTypeName() );
		
		fbxInterface->SetFBXBinding( &fbxObject );
	}
	else
	{
		Sb::DebugLogInfo( "Failed to add Object from FBX: (%s)", type.GetTypeName() );
	}

	return fbxInterface->GetHandle();
}

/**
Entry point used to create a new element of type type
*/
CHandle CCtrlProject::AddObject( const CHandle& parent, const Sb::CTypeInfo& type ) 
{
	CBank* currentBank = GetApp().GetProjectCtrl().GetCurrentBank();
	if( !currentBank )
	{
		return CHandle();
	}

    // Create the NEW element
    CBankObject* obj = currentBank->AddObject( type, parent );

	if( obj )
	{
		Sb::DebugLogInfo( "Add Object: (%s)", type.GetTypeName() );
	}
	else
	{
		Sb::DebugLogInfo( "Failed to add Object: (%s)", type.GetTypeName() );
	}

	return obj->GetHandle();
}

/**
Remove the specified bank.
*/
Void CCtrlProject::RemoveBank( const CUuid& h )
{
    GetProject()->RemoveBank( h );
}

/**
Set a given bank as current.
If h is an invalid bank handle, the first bank available is set as the current bank
*/
Bool CCtrlProject::SetCurrentBank( const Dune::CBank* bank )
{
    if( bank )
        SetCurrentBank( bank->GetUuid() );
    else 
        SetCurrentBank( CUuid() );

    return TRUE;
}

Bool CCtrlProject::SetCurrentBank( const CUuid& uuid )
{
    m_CurrentBank = uuid;

    GetApp().GetAssetView().setEnabled( m_CurrentBank.IsValid()  );
    GetApp().GetSceneView().setEnabled( m_CurrentBank.IsValid()  );
    GetApp().GetUIView().setEnabled( m_CurrentBank.IsValid()  );

    SignalCurrentBankChanged( uuid );

    return TRUE;
}

Dune::CBank* CCtrlProject::GetCurrentBank() const
{
    if( GetProject() == NULL )
        return NULL;

    return GetProject()->GetBank( m_CurrentBank );
}
