#include "CConnection.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "CSessionCommand.h"
#include <QDebug>

#include "CCollectionModel.h"
#include "CPlaylistModel.h"

CConnection::CConnection() : m_sm(this), m_xml_reader(0)
{

	connect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    connect(&m_sm, SIGNAL(progress(int,int)), this, SLOT(progress(int, int)));
}

CConnection::~CConnection()
{
    disconnect(&m_socket, SIGNAL(connected()), this, SLOT(connected()));
    disconnect(&m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(&m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void CConnection::open(QString host, int port)
{
    m_socket.connectToHost(host, port);
//    emit connectionStatusChanged( e_connected );
}

void CConnection::close()
{
    m_xml_writer->writeEndElement();
    m_xml_writer->writeEndDocument();
    if(m_xml_reader)
    {
        delete m_xml_reader;
        m_xml_reader = 0;
    }
    m_socket.disconnectFromHost();
//
}


void CConnection::addSong(QString artist, QString album, QString title)
{
    qDebug() << QString("addSong");
    m_xml_writer->writeStartElement("addSong");
    m_xml_writer->writeAttribute("artist", artist.toUtf8());
    m_xml_writer->writeAttribute("album", album.toUtf8());
    m_xml_writer->writeAttribute("title", title.toUtf8());
    m_xml_writer->writeEndElement();
}

//void CConnection::getCollection()
//{
//    qDebug() << QString("getCollection");
//    m_xml_writer->writeStartElement("getCollection");
//    m_xml_writer->writeEndElement();
//}

void CConnection::getCollection(int knownRevision)
{
    qDebug() << QString("getCollection");
    m_xml_writer->writeStartElement("getCollection");
    if( knownRevision != -1) {
    	m_xml_writer->writeAttribute("knownRev", QString().setNum(knownRevision));
    }
    m_xml_writer->writeEndElement();
}

//
//void CConnection::getPlaylist()
//{
//    qDebug() << QString("getPlaylist");
//    m_xml_writer->writeStartElement("getPlaylist");
//    m_xml_writer->writeEndElement();
//}

void CConnection::getPlaylist(int knownRevision)
{
    qDebug() << QString("getPlaylist");
    m_xml_writer->writeStartElement("getPlaylist");
    if( knownRevision != -1) {
    	m_xml_writer->writeAttribute("knownRev", QString().setNum(knownRevision));
    }
    m_xml_writer->writeEndElement();
}

//void CConnection::getNextlist()
//{
//    qDebug() << QString("getNextlist");
//    m_xml_writer->writeStartElement("getNextlist");
//    m_xml_writer->writeEndElement();
//}

void CConnection::getNextlist(int knownRevision)
{
    qDebug() << QString("getNextlist");
    m_xml_writer->writeStartElement("getNextlist");
    if( knownRevision != -1) {
    	m_xml_writer->writeAttribute("knownRev", QString().setNum(knownRevision));
    }
    m_xml_writer->writeEndElement();
}




void CConnection::connected()
{
    qDebug() << QString("CStateMachine::connected");
    m_xml_reader = new QXmlStreamReader();
    // m_xml_reader->setContentHandler(&m_contentHandler);
    m_xml_writer = new QXmlStreamWriter(&m_socket);
    m_xml_writer->setAutoFormatting(true);
    m_xml_writer->writeStartDocument(QString("1.0"), true);
    m_xml_writer->writeStartElement("session");


    getCollection(m_sm.getCollectionModelPtr()->getRevision());
    getPlaylist(m_sm.getPlaylistModelPtr()->getRevision());
    getNextlist(m_sm.getNextlistModelPtr()->getRevision());

    emit connectionStatusChanged( e_connected );
}

void CConnection::disconnected()
{
    qDebug() << QString("CStateMachine::disconnected");
    if(m_xml_reader)
    {
        delete m_xml_reader;
        m_xml_reader = 0;
    }
    if(m_xml_writer)
    {
        delete m_xml_writer;
        m_xml_writer = 0;
    }
    emit connectionStatusChanged( e_disconnected );
}

void CConnection::error()
{

}

void CConnection::readyRead()
{
    int avail = m_socket.bytesAvailable();
    // qDebug() << QString("readyRead(): %1 bytes available").arg(avail);
    QByteArray data = m_socket.readAll();
    // qDebug() << QString("readyRead(): %1 bytes read").arg(data.size());
    m_xml_reader->addData(data);
    enum QXmlStreamReader::TokenType tokenType;
    do
    {
        tokenType = m_xml_reader->readNext();
        switch(tokenType)
        {
            case QXmlStreamReader::NoToken:
                // nothing yet
                break;
            case QXmlStreamReader::Invalid:
                // check if we have to wait for new data
                if( m_xml_reader->error() != QXmlStreamReader::PrematureEndOfDocumentError )
                {
                    //there is a real error
                    qDebug() << QString("Parse Error: %1   line %2 col %3  offset %4").arg(m_xml_reader->errorString()).arg(m_xml_reader->lineNumber()).arg(m_xml_reader->columnNumber()).arg(m_xml_reader->characterOffset());
                    return;
                }
                break;
            case QXmlStreamReader::StartDocument:
                m_sm.startDocument(m_xml_reader);
                break;
            case QXmlStreamReader::EndDocument:
                m_sm.endDocument(m_xml_reader);
                break;
            case QXmlStreamReader::StartElement:
                m_sm.startElement(m_xml_reader);
                break;
            case QXmlStreamReader::EndElement:
                m_sm.endElement(m_xml_reader);
                break;
            case QXmlStreamReader::Characters:
                m_sm.characters(m_xml_reader);
                break;
            case QXmlStreamReader::Comment:
                // nothing yet
                break;
            case QXmlStreamReader::DTD:
                // nothing yet
                break;
            case QXmlStreamReader::EntityReference:
                // nothing yet
                break;
            case QXmlStreamReader::ProcessingInstruction:
                // nothing yet
                break;

            default:
                // nothing yet
                break;
        }
    } while(tokenType != QXmlStreamReader::Invalid );
}
//  m_xml_reader->parse(&m_xml_src, true);
//}

void CConnection::sendCommand(CCommandBase* cmd)
{
    qDebug() << QString("sendCommand %1").arg(cmd->commandName());
    m_xml_writer->writeStartElement(cmd->commandName());
    m_xml_writer->writeAttribute("fromRev", QString().setNum(cmd->knownRev()));
    m_xml_writer->writeCharacters(cmd->commandData());
    m_xml_writer->writeEndElement();

    delete cmd;
}


void CConnection::play()
{
	CSessionCommand *cmd = new CSessionCommand(0, "play");
	sendCommand(cmd);
}

void CConnection::stop()
{
	CSessionCommand *cmd = new CSessionCommand(0, "stop");
	sendCommand(cmd);
}

void CConnection::next() {
	CSessionCommand *cmd = new CSessionCommand(0, "next");
	sendCommand(cmd);
}

void CConnection::prev() {
	CSessionCommand *cmd = new CSessionCommand(0, "prev");
	sendCommand(cmd);
}


void CConnection::test()
{
    getCollection();
}


void CConnection::progress(int done, int total)
{
	emit progressSig(done, total);
}
