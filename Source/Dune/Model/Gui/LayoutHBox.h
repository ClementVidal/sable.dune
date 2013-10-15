#ifndef _DUNE_MODEL_GUI_LAYOUTHBOX_
#define _DUNE_MODEL_GUI_LAYOUTHBOX_

#include <Dune\Model\Gui\Layout.h>

#include <Sable\Core\Common\DataTypes.h>

namespace Dune
{

/**
*/
class CLayoutHBox : public CLayout
{

    DEFINE_MANAGED_CLASS( CLayoutHBox )

public:

    /** @name Constructor/Destructor*/
    //@{
    CLayoutHBox();
    ~CLayoutHBox();
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
	static String PropStretchFactor;
    //@}


private:

    // Attributes

};

};

#endif