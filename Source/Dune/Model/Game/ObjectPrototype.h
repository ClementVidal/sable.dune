#ifndef _DUNE_GAME_OBJECTPROTOTYPE_
#define _DUNE_GAME_OBJECTPROTOTYPE_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/String.h>
#include <Sable/Core/Collection/Array.h>

#include <Sable/Game/Property/Property.h>

#include <Dune/Model/Bank/Object.h>
#include <Dune/Model/Property/Property.h>


namespace Dune
{

class CBank;
class CHandle;
class CBakeObjectSet;

/**
\ingroup ModelAsset

Base class for any kind of Asset in the project
*/
class CGameObjectPrototype : public CBankObject
{

    DEFINE_MANAGED_CLASS( CGameObjectPrototype )

public:

    /** @name Constructor/Destructor*/
    //@{
	CGameObjectPrototype( );
    ~CGameObjectPrototype();
    //@}

    /** @name Accessors*/
    //@{
    Bool    GetIsEdited() const;
    Void    SetIsEdited( Bool onOff );
	//Void	RemoveProperty( String name );
	//Void	AddProperty( String name, Sb::CGameProperty::EType type );
	//@}

    /** @name Manipulator*/
    //@{
	//@}

private:

    // Types

	// Methods 
	/*Void	AddProperty( String name, Float32 f );
	Void	AddProperty( String name, Int32 f );
	Void	AddProperty( String name, Bool f );
	Void	AddProperty( String name, const Sb::CVector2f& f );
	Void	AddProperty( String name, const Sb::CVector3f& f );
	Void	AddProperty( String name, const Sb::CVector4f& f );*/
	Void 	PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const;
	Void 	BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
	Void 	InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
	Sb::CGameProperty::EType ConvertType( const Sb::CTypeInfo& p ) const;

    // Attributes
    Bool m_IsEdited;
	CPropertyString*	m_ScriptCode;
};


};

#endif