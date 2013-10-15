#include <Dune\Model\Gui\Text.h>

#include <Sable\Gui\Widget\Text.h>

using namespace Dune;

CPropertyEnum::SEntry CWidgetText::EnumHAlign[] = { 
	{ "Left", Sb::CWidgetText::nHAlign_Left }, 
	{ "Center", Sb::CWidgetText::nHAlign_Center }, 
	{ "Right", Sb::CWidgetText::nHAlign_Right }, 
	{ NULL, NULL } };

CPropertyEnum::SEntry CWidgetText::EnumVAlign[] = { 
	{ "Top", Sb::CWidgetText::nVAlign_Top }, 
	{ "Center", Sb::CWidgetText::nVAlign_Center }, 
	{ "Bottom", Sb::CWidgetText::nVAlign_Bottom }, 
	{ NULL, NULL } };


IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CWidgetText, CWidget )

CWidgetText::CWidgetText() :
    CWidget( )
{
    SetName("UI TextField");

    m_Text = new CPropertyString( "Text", CPropertyString::nType_MultiLine );
	m_Text->SetValue( "Text" );
    AddProperty( *m_Text );

    m_Color = new CPropertyColor( "Color" );
    AddProperty( *m_Color );

    m_Font = new CPropertyHandle( "Font" );
	m_Font->AddFilter( CAssetFont::GetStaticTypeInfo() );
    AddProperty( *m_Font );

    m_FontSize = new CPropertyFloat32( "FontSize" );
	m_FontSize->SetValue( 1.0f );
    AddProperty( *m_FontSize );

	m_Capacity = new CPropertyInt32( "Capacity" );
	m_Capacity->SetValue( 128 );
	AddProperty( *m_Capacity );

	m_VAlign = new CPropertyEnum( "VAlign", CPropertyEnum::CDef( EnumVAlign )  );
	m_VAlign->SetGroup( "Align" );
	AddProperty( *m_VAlign );

	m_HAlign = new CPropertyEnum( "HAlign", CPropertyEnum::CDef( EnumHAlign )  );
	m_HAlign->SetGroup( "Align" );
	AddProperty( *m_HAlign );


    GetProperty( CWidget::PropSize )->SetIsVisible( FALSE );
}

CWidgetText::~CWidgetText()
{
}

CPropertyInt32& CWidgetText::GetCapacity()
{
	return *m_Capacity;
}

CPropertyString& CWidgetText::GetText()
{
	return *m_Text;
}

CPropertyHandle& CWidgetText::GetFont()
{
	return *m_Font;
}

CPropertyFloat32& CWidgetText::GetFontSize()
{
	return *m_FontSize;
}

CPropertyColor& CWidgetText::GetColor()
{
	return *m_Color;
}

Bool CWidgetText::Serialize( Sb::CPersistentArchive& ar )
{
	return CWidget::Serialize( ar );
}

Void CWidgetText::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
	Sb::CWidgetText* text = NEWOBJ( Sb::CWidgetText, ( ) );
	objSet.AddObject( *text );

	CWidget::PrepareBakeObjectSet( context, objSet );
}

Void CWidgetText::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CWidgetText* widget = objSet.GetObj<Sb::CWidgetText>();

	if( widget )
	{
		const CBakeObjectSet* fontSet = context.GetObjectSet( GetFont().GetValue() );
		if( fontSet )
		{
			Sb::CTextFont* fnt = fontSet->GetObj<Sb::CTextFont>();
			if( fnt )
				widget->SetFont( *fnt );
		}

		CHandle cs;
		widget->SetText( GetText().GetValue() );
		widget->SetFontSize( GetFontSize().GetValue() );
		widget->SetColor( GetColor().GetValue() );
		widget->SetHAlign( (Sb::CWidgetText::EHAlign) m_HAlign->GetValue() );
		widget->SetVAlign((Sb::CWidgetText::EVAlign)  m_VAlign->GetValue() );
		widget->SetCapacity( m_Capacity->GetValue() );
	}
	else
	{
		context.LogError( "Failled to export window pane %s ", GetName() );
	}

	CWidget::BakeObjectSet( context, objSet );
}
