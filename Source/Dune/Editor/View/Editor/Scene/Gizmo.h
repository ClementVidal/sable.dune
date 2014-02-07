#ifndef _DUNE_EDITOR_VIEW_EDITOR_SCENE_GIZMO_
#define _DUNE_EDITOR_VIEW_EDITOR_SCENE_GIZMO_

#include <Sable/Core/ManagedObject/ManagedObject.h>
#include <Sable/Core/Event/Slot.h>


namespace Dune
{

/**
*/
class CGizmo : public Sb::CManagedObject
{

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CGizmo( CApplication& app, const CHandle& object );
    ~CGizmo();
	//@}

    /** @name Accessors*/
    //@{
	CApplication&		GetApp() const;
	CProject*			GetProject() const;
	const CHandle&		GetObjectHandle() const;
	Void				SetIsVisible( Bool onOff );
	Bool				GetIsVisible() const;
	Void				SetIsHighlighted( Bool onOff );
	Bool				GetIsHighlighted() const;
    //@}

    /** @name Manipulator*/
    //@{
    virtual Void    Render( const Sb::CRenderer& renderer ) = 0;
    //@}

protected:

	// Members
	virtual Void	OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  );

private:

    // Attributes
	Bool			m_IsVisible;
	Bool			m_IsHighlighted;
	CApplication*	m_App;
	CHandle			m_ObjectHandle;
	Sb::CEventSlot< CGizmo, const CPropertyInterface&, const Sb::CString& >	m_SlotPropertyChange;
};

};

#endif