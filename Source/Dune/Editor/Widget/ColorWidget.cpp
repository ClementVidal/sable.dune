#include <Dune/Editor/Widget/ColorWIdget.h> 

using namespace Dune;

CColorWidget::CColorWidget( QWidget& parent, const Sb::CColor& col ) :
    QWidget( &parent)
{
	QHBoxLayout* hLayout = new QHBoxLayout();

	m_LineEdit = new QLineEdit();
    m_LineEdit->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	hLayout->addWidget( m_LineEdit );
	connect( m_LineEdit, SIGNAL( editingFinished() ), this, SLOT( OnUILineEditChanged() ) );

	m_ColorFrame = new QFrame();    
	m_ColorFrame->setMaximumWidth( 30 );
	m_ColorFrame->setMinimumWidth( 30 );
	m_ColorFrame->setFrameShape( QFrame::Box );
	hLayout->addWidget( m_ColorFrame );

	m_ColorPickerBtn = new QPushButton();
	m_ColorPickerBtn->setText("...");
	m_ColorPickerBtn->setMaximumWidth( 15 );
	m_ColorPickerBtn->setMinimumWidth( 15 );
	connect( m_ColorPickerBtn, SIGNAL( clicked() ), this, SLOT( OnUIBtnClicked() ) );
	hLayout->addWidget( m_ColorPickerBtn );

	m_ColorFrame->setAutoFillBackground(true);
	
	hLayout->setSpacing( 0 );
	hLayout->setContentsMargins( 0,0,0,0 );

	setLayout( hLayout );

	SetValue( col );
}

CColorWidget::~CColorWidget()
{

}

Void CColorWidget::ShowLineEdit( Bool onOff )
{
	if( onOff )
		m_LineEdit->show();
	else
		m_LineEdit->hide();
}

Void CColorWidget::SetValue( const Sb::CColor& col )
{
	Sb::CString value = CLexicalCast::ValueToString( col );
	m_LineEdit->setText( value.GetBuffer() );
	OnUILineEditChanged();
}

const Sb::CColor& CColorWidget::GetValue() const
{
	return m_Value;
}

Bool CColorWidget::OnUIBtnClicked()
{
	QColor col = QColorDialog::getColor( QColor( m_Value.GetR()*255, m_Value.GetG()*255, m_Value.GetB()*255, m_Value.GetA()*255 ), this, "Pick color", QColorDialog::ShowAlphaChannel ); 
	
	if( col.isValid() )
	{
		SetValue( Sb::CColor( col.alphaF(), col.redF(), col.greenF(), col.blueF() ) );
	}
	
	return TRUE;
}

Bool CColorWidget::OnUILineEditChanged()
{
	QString text = m_LineEdit->text();
	if( ! CLexicalCast::StringToValue( text.toAscii().constData(), m_Value ) )
		return FALSE;

    Float32 r = MathClamp( m_Value.GetR()*255, 0, 255 );
    Float32 g = MathClamp( m_Value.GetG()*255, 0, 255 );
    Float32 b = MathClamp( m_Value.GetB()*255, 0, 255 );
    Float32 a = MathClamp( m_Value.GetA()*255, 0, 255 );

    m_ColorFrame->setStyleSheet( QString( "\
        QFrame \
        {   \
            border: 0px solid black; \
            background-color: rgba( %1, %2, %3, %4 ); \
            border-top: 1px solid black;   \
            border-bottom: 1px solid black;   \
        }\
        " ).arg(r).arg(g).arg(b).arg(a) );

	emit OnValueChanged();

	return TRUE;
}