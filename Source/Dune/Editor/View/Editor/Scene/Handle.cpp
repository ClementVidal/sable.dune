#include <Dune\Editor\View\Editor\Scene\Handle.h>

using namespace Dune;

CManipulatorHandle::CManipulatorHandle( EAxis supportAxis )
{
    m_IsVisible = FALSE;
    m_SupportAxis = supportAxis;
    m_IsHighlighted = FALSE;
}

CManipulatorHandle::~CManipulatorHandle()
{
}

Void CManipulatorHandle::SetIsVisible( Bool onOff )
{
    m_IsVisible = onOff;
}

Void CManipulatorHandle::SetIsHighlighted( Bool onOff )
{
    m_IsHighlighted = onOff;
}

EAxis CManipulatorHandle::GetSupportAxis() const
{
    return m_SupportAxis;
}

Bool CManipulatorHandle::GetIsHighlighted() const
{
    return m_IsHighlighted;
}

Void CManipulatorHandle::Render( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m )
{
    if( m_IsVisible )
    {
        InternalDraw( renderer, m );
    }
}

Bool CManipulatorHandle::GetIsVisible() const
{
    return m_IsVisible;
}