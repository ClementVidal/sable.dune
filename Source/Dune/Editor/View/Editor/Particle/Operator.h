#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_OPERATOR_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_OPERATOR_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Widget/VectorRangeWidget.h>

#include <QtGui/QFormLayout>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSOperatorPanel : public QWidget
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSOperatorPanel( QWidget& parent, Sb::CParticleOperator& op );
    virtual ~CPSOperatorPanel();
    //@}

    /** @name Accessor*/
    //@{
	Sb::CParticleOperator& GetOperator() const;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

protected:

    // Methods
	Q_SLOT Bool OnUIStartTimeChanged( );
	Q_SLOT Bool OnUIDurationChanged( );
	Void		SetOperator( Sb::CParticleOperator& op );

    // Attributes
	Sb::CParticleOperator*			m_BasOperator;
	QFormLayout*				m_FormLayout;

	CFloat32RangeWidget*		m_StartTime;
	CFloat32RangeWidget*		m_Duration;
};

};

#endif