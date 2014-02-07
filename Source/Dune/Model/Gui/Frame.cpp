#include <Dune/Model/Gui/Frame.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CWidgetFrame, CWidget )

String CWidgetFrame::PropColor = "Color";
String CWidgetFrame::PropTexture = "Texture";
String CWidgetFrame::PropTextureSize = "TextureSize";
String CWidgetFrame::PropTextureOrigin = "TextureOrigin";

CWidgetFrame::CWidgetFrame( ) :
    CWidget( )
{
    SetName("UI WindowPane");

    {
        CPropertyHandle* p;
        p = new CPropertyHandle( PropTexture );
        p->AddFilter( CAssetTexture2D::GetStaticTypeInfo() );
		p->SetGroup( "Frame" );
        AddProperty( *p );
    }

    {
        CPropertyColor* p;
        p = new CPropertyColor( PropColor  );
		p->SetGroup( "Frame" );
        AddProperty( *p );
    }
	{
		CPropertyInterface* p;
		p = new CPropertyVector2i( PropTextureSize );
		((CPropertyVector2i*)p)->SetValue( Sb::CVector2i( 1, 1 ) );
		p->SetGroup( "Frame" );
		AddProperty( *p );
	}
	{
		CPropertyInterface* p;
		p = new CPropertyVector2i( PropTextureOrigin );
		p->SetGroup( "Frame" );
		AddProperty( *p );
	}
}

CWidgetFrame::~CWidgetFrame()
{
}

const CPropertyHandle& CWidgetFrame::GetTexture() const
{
    CPropertyHandle* p;
    p = (CPropertyHandle*)GetProperty( PropTexture );
    DebugAssert( p );
    return *p;
}

CPropertyVector2i& CWidgetFrame::GetTextureSize() const
{
	CPropertyVector2i* p;
	p = (CPropertyVector2i*)GetProperty( PropTextureSize );
	DebugAssert( p );
	return *p;
}

CPropertyVector2i& CWidgetFrame::GetTextureOrigin() const
{
	CPropertyVector2i* p;
	p = (CPropertyVector2i*)GetProperty( PropTextureOrigin );
	DebugAssert( p );
	return *p;
}

const CPropertyColor& CWidgetFrame::GetColor() const
{
    CPropertyColor* p;
    p = (CPropertyColor*)GetProperty( PropColor );
    DebugAssert( p );
    return *p;
}

Bool CWidgetFrame::Serialize( Sb::CPersistentArchive& ar )
{
    return CWidget::Serialize( ar );
}

#include <Dune/Model/Property/Property.h>
#include <Sable/Gui/Widget/Frame.h>

Void CWidgetFrame::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
    Sb::CWidgetFrame* pane = NEWOBJ( Sb::CWidgetFrame, ( ) );
    objSet.AddObject( *pane );
}

Void CWidgetFrame::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CWidgetFrame* widget = objSet.GetObj<Sb::CWidgetFrame>();

    if( widget )
    {
        // Color
        widget->SetColor( GetColor().GetValue() );

        // Size
        widget->SetSize( GetSize().GetValue() );

        // TextureSize
        widget->SetTextureSize( GetTextureSize().GetValue() );

        // TextureOrigin
        widget->SetTextureOrigin( GetTextureOrigin().GetValue() );

        const CBakeObjectSet* set = context.GetObjectSet( GetTexture().GetValue() );
        if( set )
            widget->SetTexture( set->GetObj<Sb::CTexture2D>( ) );
		else
			widget->SetTexture( NULL );
    
    }
    else
    {
        context.LogError( "Failled to export window pane %s ", GetName() );
    }

    CWidget::BakeObjectSet( context, objSet );
}
