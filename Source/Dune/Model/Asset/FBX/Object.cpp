#include <Dune/Model/Asset/FBX/Object.h>

IMPLEMENT_MANAGED_CLASS( Dune, CFBXObject );

using namespace Dune;

CFBXObject::CFBXObject( CFBXDoc& doc, FbxObject* obj  )
{
	m_Doc = &doc;
	m_FBXObject = obj;
	if( obj )
	{
		m_Id.WriteFormattedText( "%d", (UInt32) obj->GetUniqueID() );
	}
}

CFBXObject::~CFBXObject()
{
}

String CFBXObject::GetUniqueId() const
{
	return m_Id;
}

String	CFBXObject::GetName() const
{
	if( m_FBXObject )
		return m_FBXObject->GetName();

	return NULL;
}

FbxObject* CFBXObject::GetFBXObject() const
{
	return m_FBXObject;
}

CFBXDoc& CFBXObject::GetDoc() const
{
	return *m_Doc;
}