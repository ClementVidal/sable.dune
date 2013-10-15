#include <Dune\Model\Gui\Widget.h>

#include <Dune\Model\Asset\Asset.h>
#include <Dune\Editor\Ctrl\Editor\UI\UI.h>

#include <Sable\Gui\Widget\Header.h>

using namespace Dune;

CPropertyEnum::SEntry CWidget::EnumSizePolicy[] = { 
	{ "Stretchable", nSizePolicy_Stretchable}, 
	{ "Fixed", nSizePolicy_Fixed }, 
	{ NULL, NULL } };

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CWidget, CBankObject )

String CWidget::PropPosition = "Position";
String CWidget::PropSize = "Size";
String CWidget::PropDepth = "Depth";
String CWidget::PropShader = "Shader";
String CWidget::PropLayout = "Layout";
String CWidget::PropStretchFactor = "StretchFactor";
String CWidget::PropHSizePolicy = "HSizePolicy";
String CWidget::PropVSizePolicy = "VSizePolicy";

CWidget::CWidget( ) :
    CBankObject( )
{
    SetName("UI Widget");

	CPropertyInterface* p;
	
    p = new CPropertyVector2i( PropPosition );
	p->SetGroup( "Widget" );
    AddProperty( *p );

	p = new CPropertyVector2i( PropSize );
	p->SetGroup( "Widget" );
	((CPropertyVector2i*)p)->SetValue( Sb::CVector2i( 128, 128 ) );
	AddProperty( *p );

    p = new CPropertyInt32( PropDepth );
	p->SetGroup( "Widget" );
    AddProperty( *p );

	CPropertyHandle* ph = new CPropertyHandle( PropShader );
	ph->SetGroup( "Widget" );
	ph->AddFilter( CAssetShader::GetStaticTypeInfo() );
	AddProperty( *ph );

    CPropertyHandle* pL = new CPropertyHandle( PropLayout );
	pL->SetGroup( "Widget" );
	pL->AddFilter( CLayoutHBox::GetStaticTypeInfo() );
	pL->AddFilter( CLayoutVBox::GetStaticTypeInfo() );
	AddProperty( *pL );

	p = new CPropertyVector2i( PropStretchFactor );
	p->SetGroup( "Widget" );
	((CPropertyVector2i*)p)->SetValue( Sb::CVector2i( 1, 1 ) );
	AddProperty( *p );

	p = new CPropertyEnum( PropVSizePolicy, CPropertyEnum::CDef( EnumSizePolicy )  );
	p->SetGroup( "Widget" );
	AddProperty( *p );

	p = new CPropertyEnum( PropHSizePolicy, CPropertyEnum::CDef( EnumSizePolicy )  );
	p->SetGroup( "Widget" );
	AddProperty( *p );

}

CWidget::~CWidget()
{
}

Bool CWidget::Serialize( Sb::CPersistentArchive& ar )
{
    return CBankObject::Serialize( ar );
}


CPropertyEnum& CWidget::GetHSizePolicy() const
{
	CPropertyEnum* p;
	p = (CPropertyEnum*)GetProperty( PropHSizePolicy );
	DebugAssert( p );
	return *p;
}

CPropertyEnum& CWidget::GetVSizePolicy() const
{
	CPropertyEnum* p;
	p = (CPropertyEnum*)GetProperty( PropVSizePolicy );
	DebugAssert( p );
	return *p;
}

CPropertyHandle& CWidget::GetCustomShader() const
{
	CPropertyHandle* p;
	p = (CPropertyHandle*)GetProperty( PropShader );
	DebugAssert( p );
	return *p;
}

CPropertyHandle& CWidget::GetLayout() const
{
    CPropertyHandle* p;
    p = (CPropertyHandle*)GetProperty( PropLayout );
    DebugAssert( p );
    return *p;
}

CPropertyVector2i& CWidget::GetPosition() const
{
    CPropertyVector2i* p;
    p = (CPropertyVector2i*)GetProperty( PropPosition );
    DebugAssert( p );
    return *p;
}

CPropertyVector2i& CWidget::GetSize() const
{
	CPropertyVector2i* p;
	p = (CPropertyVector2i*)GetProperty( PropSize );
	DebugAssert( p );
	return *p;
}

const CPropertyInt32& CWidget::GetDepth() const
{
    CPropertyInt32* p;
    p = (CPropertyInt32*)GetProperty( PropDepth );
    DebugAssert( p );
    return *p;
}

CPropertyVector2i& CWidget::GetStretchFactor() const
{
	CPropertyVector2i* p;
	p = (CPropertyVector2i*)GetProperty( PropStretchFactor );
	DebugAssert( p );
	return *p;
}

Void CWidget::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
	Sb::CWidget* pane = NEWOBJ( Sb::CWidget, ( ) );
	objSet.AddObject( *pane );
}

Void CWidget::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CWidget* widget = objSet.GetObj<Sb::CWidget>();
    DebugAssert( widget );

    widget->SetPosition( GetPosition().GetValue() );
    widget->SetSize( GetSize().GetValue() );
    widget->SetDepth( GetDepth().GetValue(), FALSE );

	// Set Parent
	{
		CBakeObjectSet* parent = context.GetObjectSet( GetParent() );
		if( parent )
		{
			Sb::CWidget* parentWdg = parent->GetObj<Sb::CWidget>();
            if( parentWdg )
            {
			    widget->SetParent( parent->GetObj<Sb::CWidget>() );
            }
			else
			{
				widget->SetParent( NULL );
			}
		}
	}

	// Set Custom shader
	if( GetCustomShader().GetValue().IsValid() )
	{
		CBakeObjectSet* shader = context.GetObjectSet( GetCustomShader().GetValue() );
		DebugAssert( shader )
		widget->SetCustomShader( shader->GetObj<Sb::CShader>() );
	}
	
    // Layout
    {
        CBakeObjectSet* layoutobjSet = context.GetObjectSet( GetLayout().GetValue() );
        if( layoutobjSet )
        {
            Sb::CLayout* layout = layoutobjSet->GetObj<Sb::CLayout>();
            if( layout )
            {
                widget->SetLayout( layout );
            }
        }

		widget->SetStretchFactor( GetStretchFactor().GetValue() );
		widget->SetHSizePolicy( (ESizePolicy) GetHSizePolicy().GetValue() );
		widget->SetVSizePolicy( (ESizePolicy) GetVSizePolicy().GetValue() );
    }
}
