#ifndef XMLREADER_H
#define XMLREADER_H
#include <QFile>
#include <QXmlStreamReader>
#include <QMap>
#include <QString>
#include <QObject>
#include "filesproxy.h"
Q_DECLARE_METATYPE(COMMUTATOR)


typedef  QPair<QStringList, QStringList> MAP_lsts; 

class xml_reader : public QObject
{
    Q_OBJECT
private:
    QFile f_in;
    QXmlStreamReader xml;
    MAP_lsts fileLsts;
    QString errors;
    int linesCount = 0;
    int filesCount = 0;
    double scale;
    int calls = 0;
    QString f_nm;
    COMMUTATOR objToSend;

public:
   // xml_reader() = default;
    xml_reader(const QString& f_name = "", QObject* parent = nullptr);

    MAP_lsts getFileStrings() const;
    QString getErrors() const;
    int getLinesCount() const;
    void resetObj();
    QString getFNm() { return f_nm; }
    void setName(const QString& nm) { f_nm = nm; }

public slots:
    void doWork(const QString nm);
    void clear();
    void setFilesCountSlot(const int cnt);

signals:
    void resultReady(const COMMUTATOR result);
    void updateBar(const int);



};

#endif // XMLREADER_H


