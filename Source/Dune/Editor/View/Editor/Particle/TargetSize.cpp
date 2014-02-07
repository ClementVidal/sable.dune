#include <Dune/Editor/View/Editor/Particle/TargetSize.h>

#include <Dune/Editor/Widget/VectorWidget.h>

using namespace Dune;

CPSTargetSizePanel::CPSTargetSizePanel( QWidget& parent, Sb::CParticleOpTargetSize& op  ) :
    CPSOperatorPanel( parent, op )
{
	m_Operator = NULL;

	m_DeltaSize = new CFloat32RangeWidget( *this, 0.0f );
	connect( m_DeltaSize, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIDeltaSizeChanged() ) );
	m_FormLayout->addRow( "Increment", m_DeltaSize );

	SetOperator( op );

	setLayout( m_FormLayout );
}

CPSTargetSizePanel::~CPSTargetSizePanel()
{

}

Void CPSTargetSizePanel::SetOperator( Sb::CParticleOpTargetSize& emitter )
{
	CPSOperatorPanel::SetOperator( emitter );
	m_Operator = &emitter;
	m_DeltaSize->SetValue( emitter.GetIncrement() );
}

Bool CPSTargetSizePanel::OnUIDeltaSizeChanged( )
{
	m_Operator->SetIncrement( m_DeltaSize->GetValue() );
	return TRUE;
}