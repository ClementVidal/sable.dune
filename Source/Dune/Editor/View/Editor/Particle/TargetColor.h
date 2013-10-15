#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_TARGETCOLOR_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_TARGETCOLOR_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Graphics\Particle\Operator\TargetColor.h>

#include <Dune\Editor\View\Editor\Particle\Operator.h>
#include <Dune\Editor\Widget\VectorRangeWidget.h>

#include <QtGui/QFormLayout>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSTargetColorPanel : public CPSOperatorPanel
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSTargetColorPanel( QWidget& parent, Sb::CParticleOpTargetColor& op );
    virtual ~CPSTargetColorPanel();
    //@}

    /** @name Accessor*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    //@}

protected:

    // Methods
	Q_SLOT Bool OnUIDeltaColorChanged( );
	Void SetOperator(  Sb::CParticleOpTargetColor& emitter );

    // Attributes
	Sb::CParticleOpTargetColor*		m_Operator;

	CVector4RangeWidget*		m_DeltaColor;
};

};

#endif