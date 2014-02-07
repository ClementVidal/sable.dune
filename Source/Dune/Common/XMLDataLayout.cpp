#include <Dune/Common/XMLDataLayout.h>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtXml/QDomText>

Sb::CXMLDataLayout::CXMLDataLayout( const Sb::CFilePath& path )
{
    m_FilePath = path;
    m_CurrentNode = NULL;
    m_CurrentNode.SetCapacity( 256 );
    m_Archive = NULL;
}

Sb::CXMLDataLayout::~CXMLDataLayout()
{
}


Bool Sb::CXMLDataLayout::Open( Sb::CPersistentArchive& ar )
{
    m_Archive = &ar;
    if( ar.IsStoring() )
    {
        m_RootNode = m_Document.createElement( "Document" );
		m_Document.appendChild( m_RootNode );
    }
    else if( ar.IsLoading() )
    {
		QFile file( m_FilePath.GetPath() );
		if (!file.open(QIODevice::ReadOnly))
		{
			Sb::DebugLogInfo( "Sb::CXMLDataLayout::Open - Failed to open %s", m_FilePath.GetPath() );
			file.close();
			return FALSE;
		}

        if( !m_Document.setContent( &file ) )
        {
            Sb::DebugLogInfo( "Sb::CXMLDataLayout::Open - Failed to parse %s", m_FilePath.GetPath() );
            return FALSE;
        }
		file.close();

        m_RootNode = m_Document.documentElement();
    }

    m_CurrentNode.PushItem( SEntry( m_RootNode, 0 ) ) ;

    return TRUE;
}

Int16 Sb::CXMLDataLayout::BeginScope( WString name, Int16 version )
{
    if( m_Archive->IsStoring() )
    {
    	if( version < 0 )
    	{
        	return -1;
    	}

        // advance last child
        m_CurrentNode.GetTopItem().LastChild++;
        // Create effective child
		QDomElement node = m_Document.createElement( QString::fromWCharArray( name ) );
		m_CurrentNode.GetTopItem().Node.appendChild( node );
		
		m_CurrentNode.PushItem( SEntry( node, 0 ) ) ;

        // Save version info only if above 0
        if( version > 0 )
        {
            m_CurrentNode.GetTopItem().Node.setAttribute( QString("Version"), version );
        }
    }
    else
    {
        QDomNode n;
        UInt32 count = 0;

        // Find valid scope...
        // By pass any scope between the current one and the requested one
		UInt32 startIndex = m_CurrentNode.GetTopItem().LastChild;
        do
        {
			QDomNodeList list = m_CurrentNode.GetTopItem().Node.childNodes();
			Int32 id = m_CurrentNode.GetTopItem().LastChild ;
			if( id >= 0 )
			{
				n = list.item( id );
				if( !n.isNull() )
				{
                    m_CurrentNode.GetTopItem().LastChild++;
				}
			}
			else
			{
				n = QDomNode();
			}

        }
		while( !n.isNull() && ( n.isElement() && ( n.toElement().tagName() != QString::fromWCharArray( name ) ) ) );

        // if scope is not found abort
        if( n.isNull() )
        {
            m_CurrentNode.GetTopItem().LastChild = startIndex;
            return -1;
        }

        if( n.isElement() && ( n.toElement().tagName() != QString::fromWCharArray( name ) ) )
        {
            return -1;
        }

        m_CurrentNode.PushItem( SEntry( n.toElement(), 0 ) ) ;

        QString string, def( "0" );
        string = m_CurrentNode.GetTopItem().Node.attribute( QString("Version"), def );

        long temp = string.toLong( );
        version = temp;
    }
    return version;
}

Bool Sb::CXMLDataLayout::EndScope( )
{
    m_CurrentNode.PopItem();
    return TRUE;
}

Void Sb::CXMLDataLayout::FlushData( const Bool IsStoring )
{
    if( IsStoring )
    {
		QFile file( m_FilePath.GetPath() );
		if( !file.open( QIODevice::WriteOnly ) )
			return;

		QTextStream ts( &file );
		ts << m_Document.toString( 4 );

		file.close();
    }
}

Void Sb::CXMLDataLayout::Store( Int64 data, WString type, WString name )
{
	QString s = QString("%1").arg( (qlonglong) data );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( UInt64 data, WString type, WString name )
{
	QString s = QString("%1").arg( (qulonglong) data );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( UInt32 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( UInt16 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( UInt8 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( Int32 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
    m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( Int16 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
    m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( Int8 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
    m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( Float32 data, WString type, WString name )
{
    QString s = QString("%1").arg( data );
    m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( String data, UInt32 length, WString type, WString name )
{
	QString s = QString("%1").arg( QString( data ) );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( const CString& data, WString type, WString name )
{
	QString s = QString("%1").arg( QString( data.GetBuffer() ) );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Void Sb::CXMLDataLayout::Store( WString data, UInt32 length, WString type, WString name )
{
	QString s = QString("%1").arg( QString::fromWCharArray( data ) );
	m_CurrentNode.GetTopItem().Node.setAttribute( QString::fromWCharArray( name ), s );
}

Bool Sb::CXMLDataLayout::Load( UInt64& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	qulonglong temp;
	bool ok = FALSE;
	temp = stringVal.toULongLong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( Int64& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	qlonglong temp;
	bool ok = FALSE;
	temp = stringVal.toLongLong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( UInt32& data, WString type, WString name )
{
    QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
        return FALSE;

    unsigned long temp;
	bool ok = FALSE;
	temp = stringVal.toULong( &ok );
    if( ok )
        data = temp;
    else
        data = 0;

    return TRUE;
}

Bool Sb::CXMLDataLayout::Load( UInt16& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	unsigned long temp;
	bool ok = FALSE;
	temp = stringVal.toULong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( UInt8& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	unsigned long temp;
	bool ok = FALSE;
	temp = stringVal.toULong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( Int32& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	unsigned long temp;
	bool ok = FALSE;
	temp = stringVal.toLong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( Int16& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	unsigned long temp;
	bool ok = FALSE;
	temp = stringVal.toLong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( Int8& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	unsigned long temp;
	bool ok = FALSE;
	temp = stringVal.toLong( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( Float32& data, WString type, WString name )
{
	QString stringVal;
	stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), stringVal );
	if( stringVal.isEmpty() )
		return FALSE;

	Float32 temp;
	bool ok = FALSE;
	temp = stringVal.toFloat( &ok );
	if( ok )
		data = temp;
	else
		data = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( WChar* data, UInt32 s, WString type, WString name )
{
	QString string = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), "" );

	if( m_CurrentNode.GetTopItem().Node.hasAttribute( QString::fromWCharArray( name ) ) == FALSE )
		return FALSE;

	string.toWCharArray( data );
	data[string.length()] = 0;

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( CString& data, WString type, WString name )
{
	QByteArray stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), "" ).toLocal8Bit();

	if( m_CurrentNode.GetTopItem().Node.hasAttribute( QString::fromWCharArray( name ) ) == FALSE )
		return FALSE;

	data = stringVal.data();

	return TRUE;
}

Bool Sb::CXMLDataLayout::Load( Char* data, UInt32 s, WString type, WString name )
{
	QByteArray stringVal = m_CurrentNode.GetTopItem().Node.attribute( QString::fromWCharArray( name ), "" ).toLocal8Bit();
        
	if( m_CurrentNode.GetTopItem().Node.hasAttribute( QString::fromWCharArray( name ) ) == FALSE )
		return FALSE;

    Sb::StringCopy( stringVal.data(), data, s );

    return TRUE;
}

Void Sb::CXMLDataLayout::Store( Byte* data, UInt32 s, WString type, WString name )
{
	QByteArray byteArray( (const char*)data, s );
	QByteArray byteArrayBase64 = byteArray.toBase64();

	QDomElement elementNode = m_Document.createElement( QString::fromWCharArray( name ) );
	QDomText textNode = m_Document.createTextNode( byteArrayBase64.constData() );

	elementNode.appendChild( textNode );
	m_CurrentNode.GetTopItem().Node.appendChild( elementNode );
}

Bool Sb::CXMLDataLayout::Load( Byte* data, UInt32 s, WString type, WString name )
{
	QDomElement elementNode = m_CurrentNode.GetTopItem().Node.firstChildElement( QString::fromWCharArray( name ) );
	if( elementNode.isNull() )
		return FALSE;

	QDomText textNode = elementNode.firstChild().toText();
	if( textNode.isNull() )
		return FALSE;

	QByteArray byteArray = QByteArray::fromBase64( QByteArray( textNode.data().toLocal8Bit().constData() ) );

	DebugAssert( byteArray.length() == s );

	Sb::MemoryCopy( byteArray.constData(), data, s );

	return FALSE;
}