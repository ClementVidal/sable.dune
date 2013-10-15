#include <Dune\Editor\View\Editor\Particle\TargetColor.h>

using namespace Dune;


CPSTargetColorPanel::CPSTargetColorPanel( QWidget& parent, Sb::CParticleOpTargetColor& op  ) :
    CPSOperatorPanel( parent, op )
{
	m_Operator = &op;

	m_DeltaColor = new CVector4RangeWidget( *this, Sb::CVector4f( 0.0f, 0.0f, 0.0f, 0.0f ) );
	connect( m_DeltaColor, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIDeltaColorChanged() ) );
	m_FormLayout->addRow( "Increment", m_DeltaColor );

	setLayout( m_FormLayout );	
}

CPSTargetColorPanel::~CPSTargetColorPanel()
{
}

Void CPSTargetColorPanel::SetOperator( Sb::CParticleOpTargetColor& op )
{
	CPSOperatorPanel::SetOperator( op );
	m_Operator = &op;
	m_DeltaColor->SetValue( m_Operator->GetIncrement() );
}

Bool CPSTargetColorPanel::OnUIDeltaColorChanged( )
{
	m_Operator->SetIncrement( m_DeltaColor->GetValue() );
	return TRUE;
}

