#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_TARGETSIZE_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_TARGETSIZE_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Graphics/Particle/Operator/TargetSize.h>

#include <QtGui/QWidget>
#include <QtGui/QFormLayout>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSTargetSizePanel : public CPSOperatorPanel
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSTargetSizePanel( QWidget& parent, Sb::CParticleOpTargetSize& op );
    virtual ~CPSTargetSizePanel();
    //@}

    /** @name Accessor*/
    //@{
	Void SetOperator( Sb::CParticleOpTargetSize& emitter );
    //@}

    /** @name Manipulator*/
    //@{
    //@}

protected:

    // Methods
	Q_SLOT Bool OnUIDeltaSizeChanged( );

    // Attributes
	Sb::CParticleOpTargetSize*	m_Operator;

	CFloat32RangeWidget*			m_DeltaSize;
};

};

#endif