#ifndef _DUNE_MODEl_ELEMENT_UI_LAYOUTVBOX_
#define _DUNE_MODEl_ELEMENT_UI_LAYOUTVBOX_

#include <Dune/Model/Gui/Layout.h>

#include <Sable/Core/Common/DataTypes.h>



namespace Dune
{

/**
*/
class CLayoutVBox : public CLayout
{

    DEFINE_MANAGED_CLASS( CLayoutVBox )

public:

    /** @name Constructor/Destructor*/
    //@{
    CLayoutVBox();
    ~CLayoutVBox();
    //@}

    /** @name Accessors*/
	//@{
    //@}

    /** @name Manipulator*/
    //@{
    Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out ) const;
    Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& out ) ;
    //@}

    /** @name Attributes names*/
	//@{
    //@}


private:

    // Attributes
};

};

#endif