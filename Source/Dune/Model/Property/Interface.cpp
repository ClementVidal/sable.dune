#include <Dune/Model/Property/Interface.h> 

IMPLEMENT_MANAGED_CLASS1( Dune, CPropertyInterface, Sb::CManagedObject );

CPropertyInterface::CPropertyInterface( )
{
    m_Object = NULL;
    m_Name = "";
    m_IsEnabled = TRUE;
    m_IsVisible = TRUE;
	m_IsAutoSerialized = TRUE;
}

CPropertyInterface::CPropertyInterface( String name )
{
	m_Object = NULL;
	m_Name = "";
	m_IsEnabled = TRUE;
	m_IsVisible = TRUE;
	m_IsAutoSerialized = TRUE;
	Initialize( name );
}

CPropertyInterface::~CPropertyInterface()
{
    
}

Void CPropertyInterface::Initialize( String name )
{
	DebugAssert( m_Object == NULL );
	m_Object = NULL;
	m_Name = name;
	m_IsEnabled = TRUE;
	m_IsVisible = TRUE;
	m_IsAutoSerialized = TRUE;
}

/**
Only called by CBankObject::AddProperty & CPropertyArray::AddItem
*/
Void CPropertyInterface::BindToObject( CBankObject* obj )
{
	m_Object = obj;
}

CBankObject* CPropertyInterface::GetObj() const
{
    return m_Object;
}

CBankObjectAccessor* CPropertyInterface::GetObjAccessor() const
{
	if( m_Object )
		return m_Object->GetObjAccessor();

	return NULL;
}

Bool CPropertyInterface::GetIsAutoSerialized( ) const
{
	return m_IsAutoSerialized;
}

/**
If set to TRUE this property is automatically serialized when the object that own it is serialized.
Otherwise the object have to implement custom serialization functions
*/
Void CPropertyInterface::SetIsAutoSerialized( Bool onOff ) 
{
	m_IsAutoSerialized = onOff;
}

Bool CPropertyInterface::GetIsVisible() const
{
    return m_IsVisible;
}

Bool CPropertyInterface::GetIsEnabled() const
{
    return m_IsEnabled;
}

String CPropertyInterface::GetGroup() const
{
    return m_GroupName;
}

Void CPropertyInterface::SetIsVisible( Bool onOff )
{
    m_IsVisible = onOff;
}

Void CPropertyInterface::SetIsEnabled( Bool onOff )
{
    m_IsEnabled = onOff;
}

Void CPropertyInterface::SetGroup( String group )
{
    m_GroupName = group;
}

String CPropertyInterface::GetName() const
{
    return m_Name;
}

/**
Serialize a property.
*/
Bool CPropertyInterface::Serialize( Sb::CPersistentArchive& ar )
{
	return TRUE;
}

