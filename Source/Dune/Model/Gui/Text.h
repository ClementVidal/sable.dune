#ifndef _DUNE_MODEl_ELEMENT_UI_TEXTFIELD_
#define _DUNE_MODEl_ELEMENT_UI_TEXTFIELD_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Model/Gui/Widget.h>
#include <Dune/Model/Property/Header.h>

namespace Dune
{

class CPropertyString;
class CPropertyHandle;
class CPropertyFloat32;
class CPropertyColor;

/**
*/
class CWidgetText : public CWidget
{

    DEFINE_MANAGED_CLASS( CWidgetText )

public:

    /** @name Constructor/Destructor*/
    //@{
    CWidgetText( );
    ~CWidgetText();
    //@}

    /** @name Accessors*/
    //@{
	//Sb::CRect2Df			 GetRect() const;
	CPropertyInt32&	GetCapacity();
	CPropertyString&	GetText();
    CPropertyHandle&	GetFont();
    CPropertyFloat32&	GetFontSize();
    CPropertyColor&		GetColor();
	//@}

    /** @name Manipulator*/
    //@{
	Bool Serialize( Sb::CPersistentArchive& ar );
	Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out ) const;
	Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& out );
	//@}
	
	/** @name Enum*/
	//@{
	static CPropertyEnum::SEntry EnumHAlign[];
	static CPropertyEnum::SEntry EnumVAlign[];
	//@}

private:

    // Methods

    // Attributes
	CPropertyString*	m_Text;
	CPropertyColor*		m_Color;
	CPropertyHandle*	m_Font;
	CPropertyFloat32*	m_FontSize;
	CPropertyEnum*		m_VAlign;
	CPropertyEnum*		m_HAlign;
	CPropertyInt32*	m_Capacity;

};

};

#endif