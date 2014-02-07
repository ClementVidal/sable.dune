#ifndef _DUNE_USERINTERFACE_WIDGET_
#define _DUNE_USERINTERFACE_WIDGET_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/String.h>
#include <Sable/Core/Collection/Array.h>
#include <Sable/Core/Geometry/Rect2Df.h>

namespace Dune
{

class CPropertyInt32;
class CPropertyVector2f;
class CPropertyHandle;

/**
*/
class CWidget : public CBankObject
{

    DEFINE_MANAGED_CLASS( CWidget )

public:

    /** @name Constructor/Destructor*/
    //@{
    CWidget();
    ~CWidget();
    //@}

    /** @name Accessors*/
    //@{
	const CPropertyInt32&      GetDepth() const;
    CPropertyVector2i&		    GetPosition() const;
    CPropertyVector2i&		    GetSize() const;
	CPropertyHandle&			GetCustomShader() const;
	CPropertyHandle&			GetLayout() const;
	CPropertyVector2i&			GetStretchFactor() const;
	CPropertyEnum&				GetHSizePolicy() const;
	CPropertyEnum&				GetVSizePolicy() const;
    //@}

    /** @name Baking*/
    //@{
    Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& out );
	Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const;
    //@}

    /** @name Manipulator*/
    //@{
    Bool						Serialize( Sb::CPersistentArchive& ar );
    //@}

    /** @name Attributes names*/
	//@{
	static CPropertyEnum::SEntry EnumSizePolicy[];
    static String PropPosition;
    static String PropSize;
    static String PropDepth;
	static String PropShader;
	static String PropLayout;
	static String PropStretchFactor;
	static String PropHSizePolicy;
	static String PropVSizePolicy;
    //@}

private:

    // Attributes

	// Methods

};

};

#endif