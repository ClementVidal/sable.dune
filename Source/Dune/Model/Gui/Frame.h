#ifndef _DUNE_MODEl_ELEMENT_UI_WINDOWPANE_
#define _DUNE_MODEl_ELEMENT_UI_WINDOWPANE_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Model/Gui/Widget.h>

namespace Dune
{

class CPropertyColor;
class CPropertyHandle;

/**
*/
class CWidgetFrame : public CWidget
{

    DEFINE_MANAGED_CLASS( CWidgetFrame )

public:

    /** @name Constructor/Destructor*/
    //@{
    CWidgetFrame();
    ~CWidgetFrame();
    //@}

    /** @name Accessors*/
    //@{
    const CPropertyHandle&  GetTexture() const;
    CPropertyVector2i&		GetTextureSize() const;
    CPropertyVector2i&		GetTextureOrigin() const;
    const CPropertyColor&	GetColor() const;
    //@}

    /** @name Manipulator*/
    //@{
    Bool Serialize( Sb::CPersistentArchive& ar );
    Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out ) const;
    Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& out ) ;
    //@}

    /** @name Attributes names*/
    //@{
    static String PropTexture;
    static String PropColor;
	static String PropTextureSize;
	static String PropTextureOrigin;
    //@}


private:

    // Attributes

};

};

#endif