#include <Dune\Editor\Widget\MatrixWidget.h> 

using namespace Dune;

CMatrix4x4Widget::CMatrix4x4Widget( QWidget& parent, const Sb::CMatrix4x4f& value  ) :
    QWidget( &parent )
{
	m_CanTrigerSignal = TRUE;

    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing( 0 );
    l->setContentsMargins( 0,0,0,0 );

    for( UInt32 x=0;x<4;x++ )
    {
        QHBoxLayout* layout = new QHBoxLayout();
        for( UInt32 y=0;y<4;y++ )
        {
            m_LineEdits[x][y] = new QLineEdit();
            m_LineEdits[x][y]->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
            connect( m_LineEdits[x][y], SIGNAL( editingFinished( ) ), this, SLOT( OnUIValueChanged( ) ) );

            m_LineEdits[x][y]->setText( QString::number( value.m[x][y] ) );

            layout->addWidget( m_LineEdits[x][y] );
        }
        l->addLayout( layout );
    }

	setLayout( l );

	SetValue( value );

    setMinimumSize( QSize( 128, 64 ) );
	
}

CMatrix4x4Widget::~CMatrix4x4Widget()
{

}

Void CMatrix4x4Widget::SetValue( const Sb::CMatrix4x4f& value, Bool trigerSignal )
{
	m_Value = value;
	m_CanTrigerSignal = FALSE;

    for( UInt32 x=0;x<4;x++ )
    {
        for( UInt32 y=0;y<4;y++ )
        {
            m_LineEdits[x][y]->setText( QString::number( value.m[x][y] ) );
        }
    }

	m_CanTrigerSignal = TRUE;

    if( trigerSignal )
	    emit OnValueChanged();
}

const Sb::CMatrix4x4f& CMatrix4x4Widget::GetValue() const
{
	return m_Value;
}

Bool CMatrix4x4Widget::OnUIValueChanged( )
{
	if( m_CanTrigerSignal == FALSE )
		return TRUE;

    Sb::CMatrix4x4f m;
    for( UInt32 x=0;x<4;x++ )
    {
        for( UInt32 y=0;y<4;y++ )
        {
            m.m[x][y] = (Float32) m_LineEdits[x][y]->text().toDouble();
        }
    }

	if( m_Value != m )
	{
		m_Value = m;
		emit OnValueChanged();
	}
	return TRUE;
}
