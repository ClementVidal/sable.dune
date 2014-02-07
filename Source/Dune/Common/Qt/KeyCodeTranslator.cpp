#include <Dune/Common/Qt/KeyCodeTranslator.h>

using namespace Dune;

EKeyboardEventData CKeyCodeTranslator::Translate( Qt::Key key )
{
    switch( key )
    {
    case Qt::Key_Escape:
        return nKeyboardEventData_Escape;
    case Qt::Key_Left:
        return nKeyboardEventData_LEFT;
    case Qt::Key_Right:
        return nKeyboardEventData_RIGHT;
    case Qt::Key_Up:
        return nKeyboardEventData_UP;
    case Qt::Key_Down:
        return nKeyboardEventData_DOWN;
    case Qt::Key_Control:
        return nKeyboardEventData_LeftControl;
    case Qt::Key_Shift:
        return nKeyboardEventData_LeftShift;
    case Qt::Key_Alt:
        return nKeyboardEventData_LeftAlt;
    }
    return nKeyboardEventData_None;
}