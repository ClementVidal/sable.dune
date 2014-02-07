#ifndef _DUNE_MODEl_ELEMENT_UI_LAYOUT_
#define _DUNE_MODEl_ELEMENT_UI_LAYOUT_

#include <Dune/Model/Bank/Object.h>

#include <Sable/Core/Common/DataTypes.h>

namespace Dune
{

/**
*/
class CLayout : public CBankObject
{

    DEFINE_MANAGED_CLASS( CLayout )

public:

    /** @name Constructor/Destructor*/
    //@{
    CLayout();
    ~CLayout();
    //@}

    /** @name Accessors*/
	//@{
	CPropertyVector2i&			GetStretchFactor() const;
	CPropertyEnum&				GetHSizePolicy() const;
	CPropertyEnum&				GetVSizePolicy() const;
	CPropertyVector2i&			GetSize() const;
    //@}

    /** @name Manipulator*/
    //@{
    Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& out ) ;
    //@}

    /** @name Attributes names*/
	//@{
	static CPropertyEnum::SEntry EnumSizePolicy[];
	static String PropStretchFactor;
	static String PropHSizePolicy;
	static String PropVSizePolicy;
	static String PropSize;
    //@}


private:

    // Attributes
};

};

#endif