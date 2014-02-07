#include <Dune/Editor/View/Editor/Particle/EmitterRectangle.h>

//#include <Dune/Editor/Widget/VectorWidget.h>
//#include <Dune/Editor/Widget/WxFloatInputCtrl.h>

using namespace Dune;


CPSEmitterRectanglePanel::CPSEmitterRectanglePanel( QWidget& parent, Sb::CParticleOpSourceRectangle& op ) :
    CPSEmitterPanel( parent, op )
{
	m_OpRectangle = NULL;

	m_Origin = new CVector3Widget( *this, Sb::CVector3f::Zero );
	connect( m_Origin, SIGNAL( OnValueChanged() ), this, SLOT( OnUIOriginChanged() ) );
	m_FormLayout->addRow( "Origin", m_Origin );

	m_XSize = new QDoubleSpinBox();
	connect( m_XSize, SIGNAL( valueChanged( double ) ), this, SLOT( OnSizeXChanged( double ) ) );
	m_FormLayout->addRow( "Size X", m_XSize );

	m_YSize = new QDoubleSpinBox();
	connect( m_YSize, SIGNAL( valueChanged( double ) ), this, SLOT( OnSizeYChanged( double ) ) );
	m_FormLayout->addRow( "Size Y", m_YSize );

	m_ZSize = new QDoubleSpinBox();
	connect( m_ZSize, SIGNAL( valueChanged( double ) ), this, SLOT( OnSizeZChanged( double ) ) );
	m_FormLayout->addRow( "Size Z", m_ZSize );

	SetEmitter( op );
}

CPSEmitterRectanglePanel::~CPSEmitterRectanglePanel()
{
}

Void CPSEmitterRectanglePanel::SetEmitter( Sb::CParticleOpSourceRectangle& emitter )
{
	m_OpRectangle = & emitter;

	CPSEmitterPanel::SetEmitter( emitter );
	m_Origin->SetValue( emitter.GetOrigin() );
	m_XSize->setValue( emitter.GetXSize() );
	m_YSize->setValue( emitter.GetYSize() );
	m_ZSize->setValue( emitter.GetZSize() );
}

Bool CPSEmitterRectanglePanel::OnSizeXChanged( double v )
{
	m_OpRectangle->SetXSize( m_XSize->value() );
	return TRUE;
}

Bool CPSEmitterRectanglePanel::OnSizeYChanged( double v )
{
	m_OpRectangle->SetYSize( m_YSize->value() );
	return TRUE;
}

Bool CPSEmitterRectanglePanel::OnSizeZChanged( double v )
{
	m_OpRectangle->SetZSize( m_ZSize->value() );
	return TRUE;
}

Bool CPSEmitterRectanglePanel::OnUIOriginChanged( )
{
	m_OpRectangle->SetOrigin( m_Origin->GetValue() );
	return TRUE;
}

