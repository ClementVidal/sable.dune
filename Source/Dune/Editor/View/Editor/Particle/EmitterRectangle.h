#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_EMITTERRECTANGLE_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_EMITTERRECTANGLE_

#include <Sable/Core/Common/DataTypes.h>
#include <Dune/Editor/View/Editor/Particle/Emitter.h>

#include <Dune/Editor/Widget/VectorWidget.h>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSEmitterRectanglePanel : public CPSEmitterPanel
{
	
	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSEmitterRectanglePanel( QWidget& parent, Sb::CParticleOpSourceRectangle& op );
    virtual ~CPSEmitterRectanglePanel();
    //@}

    /** @name Accessor*/
    //@{
	Void SetEmitter( Sb::CParticleOpSourceRectangle& emitter );
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Methods
	Q_SLOT Bool	OnSizeXChanged( double v );
	Q_SLOT Bool	OnSizeYChanged( double v );
	Q_SLOT Bool	OnSizeZChanged( double v );
	Q_SLOT Bool	OnUIOriginChanged( );

    // Attributes
	Sb::CParticleOpSourceRectangle*	m_OpRectangle;
	CVector3Widget*		m_Origin;
	QDoubleSpinBox*		m_XSize;
	QDoubleSpinBox*		m_YSize;
	QDoubleSpinBox*		m_ZSize;
};

};

#endif