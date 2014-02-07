#include <Dune/Model/Asset/FBX/Interface.h>

#include <Dune/Model/Asset/FBX/Doc.h>

using namespace Dune;

String CFBXDocInterface::PropFBXPath = "FBXPath";
String CFBXDocInterface::PropFBXId = "FBXId";

IMPLEMENT_MANAGED_CLASS1( Dune, CFBXDocInterface, CAsset )

CFBXDocInterface::CFBXDocInterface() :
    CAsset( )
{
    m_PropertyFBXPath = new CPropertyFilePath( PropFBXPath );
	m_PropertyFBXPath->SetIsEnabled( FALSE );
    m_PropertyFBXPath->SetGroup( "FBX" );
	AddProperty( *m_PropertyFBXPath );

	m_PropertyFBXId = new CPropertyString( PropFBXId, CPropertyString::nType_SingleLine );
	m_PropertyFBXId->SetIsEnabled( FALSE );
    m_PropertyFBXId->SetGroup( "FBX" );
	AddProperty( *m_PropertyFBXId );
}

CFBXDocInterface::~CFBXDocInterface()
{
}

/**
Return the file path to the referenced collada document
*/
CPropertyFilePath& CFBXDocInterface::GetFBXDocFilePath() const
{
	return *m_PropertyFBXPath;
}

/**
Return the unique identifier of the object in the collada document
*/
CPropertyString& CFBXDocInterface::GetFBXId() const
{
	return *m_PropertyFBXId;
}

/**
This method is called every time the binding between the object and the reference collada document change.
This include when the collada document get's reloaded.

Overload this method to extract informations from the collada doc and to display them in the UI.
*/
Void CFBXDocInterface::OnSetFBXBinding()
{
}

Void CFBXDocInterface::OnSetFBXBindingInternal()
{
	OnSetFBXBinding();
	SignalFBXBindingChange( *this );
}

/**
Call this method to set the binding between this asset and an element within a FBX document.
\note If binding change, the signal SignalFBXBindingChange will be trigerred
*/
Void CFBXDocInterface::SetFBXBinding( CFBXObject* fbxObj )
{
	// Nothing change
	if( m_FBXObject == fbxObj )
		return;

	GetFBXDocFilePath().SetValue( fbxObj->GetDoc().GetPath() );
	GetFBXId().SetValue( fbxObj->GetUniqueId() );

	m_FBXObject = fbxObj;

	OnSetFBXBindingInternal();
}

Bool CFBXDocInterface::Serialize( Sb::CPersistentArchive& ar )
{
	Bool r = CAsset::Serialize( ar );

	if( ar.IsLoading() && GetFBXDocFilePath().GetValue().IsValid() )
	{
		if( GetFBXDocFilePath().GetValue().Exist() )
		{
			CFBXDoc* doc = GetProject()->GetFBXManager().GetOrAdd( GetFBXDocFilePath().GetValue() );

			m_FBXObject = doc->GetObject( GetFBXId().GetValue() );

			OnSetFBXBindingInternal();
		}
		else
		{
			if( GetFBXDocFilePath().GetValue().IsValid() )
				CoreManager.GetLog().LogError( "FBX File does not exist: %s", GetFBXDocFilePath().GetValue().GetPath() );
		}
	}

	return TRUE;
}

/**
Reload the FBX if necessary and reset the collada binding
*/
Void CFBXDocInterface::ReloadFBXIfNecessary() 
{
	if( m_FBXObject && m_FBXObject->GetDoc().ReloadIfNecessary() )
	{
		OnSetFBXBindingInternal();
	}
}

CFBXObject* CFBXDocInterface::GetFBXObject()
{
	return m_FBXObject;
}

/**
Return an access to the collada dae document,
Note that if the collada docuement is out of date, it will be automaticly reloaded from disk
*/
CFBXDoc* CFBXDocInterface::GetFBXDoc()
{
	if( m_FBXObject == NULL )
	{
		return NULL;
	}

	ReloadFBXIfNecessary();

	return &m_FBXObject->GetDoc();
}