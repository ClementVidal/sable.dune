#include <Dune\Model\Game\ObjectPrototype.h>

#include <Dune\Model\Property\Property.h>
#include <Dune\Model\Property\PropertyArray.h>

#include <Sable\Game\Object\Header.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CGameObjectPrototype, CBankObject )

CGameObjectPrototype::CGameObjectPrototype() :
    CBankObject( )
{
	SetName( "GameObjectPrototype" );

    m_IsEdited = FALSE;

	m_ScriptCode = NEWOBJ( CPropertyString, ("Script", CPropertyString::nType_Script) );
	m_ScriptCode->SetGroup( "GameObjectPrototype" );
	AddProperty( *m_ScriptCode );

}

CGameObjectPrototype::~CGameObjectPrototype()
{

}

Bool CGameObjectPrototype::GetIsEdited() const
{
    return m_IsEdited;
}

Void CGameObjectPrototype::SetIsEdited( Bool onOff )
{
    m_IsEdited = onOff;
}

Void CGameObjectPrototype::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CGameObjectPrototype* objPrototype = NEWOBJ( Sb::CGameObjectPrototype, () );
	objSet.AddObject( *objPrototype );
}

Sb::CGameProperty::EType CGameObjectPrototype::ConvertType( const Sb::CTypeInfo& p ) const
{
	if( p == CPropertyBool::GetStaticTypeInfo() )
		return Sb::CGameProperty::nType_Bool;
	else if( p == CPropertyInt32::GetStaticTypeInfo() )
		return Sb::CGameProperty::nType_Int;
	else if( p == CPropertyFloat32::GetStaticTypeInfo() )
		return Sb::CGameProperty::nType_Float;
	else if( p == CPropertyVector2f::GetStaticTypeInfo() )
		return Sb::CGameProperty::nType_Tuple2f;
	else if( p == CPropertyVector3f::GetStaticTypeInfo() )
		return Sb::CGameProperty::nType_Tuple3f;
	/*else if( p == CPropertyVector4f::GetStaticTypeInfo() )
		return Sb::CGameProperty::nType_Tuple4f;*/

	return Sb::CGameProperty::nType_None;
}
/*

Void CGameObjectPrototype::AddProperty( String name, Sb::CGameProperty::EType type )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
	{
		if( ConvertType( p->GetTypeInfo() ) != type )
		{
			RemoveProperty( *p );
		}
		else
		{
			return;
		}
	}

	if( type == Sb::CGameProperty::nType_Bool )
		AddProperty( name, (Bool) TRUE );
	else if( type == Sb::CGameProperty::nType_Int )
		AddProperty( name, (Int32)0 );
	else if( type == Sb::CGameProperty::nType_Float )
		AddProperty( name, (Float32).0f );
	else if( type == Sb::CGameProperty::nType_Tuple2f )
		AddProperty( name, Sb::CVector2f::Zero );
	else if( type == Sb::CGameProperty::nType_Tuple3f )
		AddProperty( name, Sb::CVector3f::Zero );
	else if( type == Sb::CGameProperty::nType_Tuple4f )
		AddProperty( name, Sb::CVector4f::Zero );
}

Void CGameObjectPrototype::AddProperty( String name, Float32 f )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
		return;

	CPropertyFloat32* pf = NEWOBJ( CPropertyFloat32, ( name ) );
	pf->SetValue( f );
	AddProperty( *pf );
}

Void CGameObjectPrototype::AddProperty( String name, Int32 f )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
		return;

	CPropertyInt32* pf = NEWOBJ( CPropertyInt32, ( name ) );
	pf->SetValue( f );
	AddProperty( *pf );
}


Void CGameObjectPrototype::AddProperty( String name, Bool f )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
		return;

	CPropertyBool* pf = NEWOBJ( CPropertyBool, ( name ) );
	pf->SetValue( f );
	AddProperty( *pf );
}

Void CGameObjectPrototype::AddProperty( String name, const Sb::CVector2f& f )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
		return;

	CPropertyVector2f* pf = NEWOBJ( CPropertyVector2f, ( name ) );
	pf->SetValue( f );
	AddProperty( *pf );
}

Void CGameObjectPrototype::AddProperty( String name, const Sb::CVector3f& f )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
		return;

	CPropertyVector3f* pf = NEWOBJ( CPropertyVector3f, ( name ) );
	pf->SetValue( f );
	AddProperty( *pf );
}

Void CGameObjectPrototype::AddProperty( String name, const Sb::CVector4f& f )
{
	CPropertyInterface* p = GetProperty( name );
	if( p )
		return;

	CPropertyVector4f* pf = NEWOBJ( CPropertyVector4f, ( name ) );
	pf->SetValue( f );
	AddProperty( *pf );
}*/

Void CGameObjectPrototype::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CGameObjectPrototype* objPrototype = objSet.GetObj<Sb::CGameObjectPrototype>( );
	PropertyTable::Iterator it;
	const PropertyTable& pTable = GetPropertyTable();
	Sb::CGameProperty *param = NULL;
/*
	ForEachItem( it, pTable )
	{
		CPropertyInterface& p = *(*it);
		if( !Sb::StringCompare( p.GetGroup(), "Property" ) )
		{
			if( p.GetTypeInfo().IsKindOf( CPropertyFloat32::GetStaticTypeInfo() ) )
			{
				param = objPrototype->AddProperty( p.GetName() );
				param->SetAsFloat( ((CPropertyFloat32&)(p)).GetValue() );
			}
			else if( p.GetTypeInfo().IsKindOf( CPropertyInt32::GetStaticTypeInfo() ) )
			{
				objPrototype->AddProperty( p.GetName() );
				param->SetAsInt( ((CPropertyInt32&)(p)).GetValue() );
			}
			else if( p.GetTypeInfo().IsKindOf( CPropertyVector2f::GetStaticTypeInfo() ) )
			{
				objPrototype->AddProperty( p.GetName() );
				param->SetAsVector2( ((CPropertyVector2f&)(p)).GetValue() );
			}
			else if( p.GetTypeInfo().IsKindOf( CPropertyVector3f::GetStaticTypeInfo() ) )
			{
				objPrototype->AddProperty( p.GetName() );
				param->SetAsVector3( ((CPropertyVector3f&)(p)).GetValue() );
			}
		}
	}

	CBankObject::BakeObjectSet( context, objSet );*/
}

Void CGameObjectPrototype::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{

}