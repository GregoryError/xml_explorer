#ifndef FILESREADER_H
#define FILESREADER_H

#include <QObject>
#include <QFileDialog>
#include <QString>
#include "xmlreader.h"
#include "filesproxy.h"
#include <QThread>

class FilesReader : public QObject
{
    Q_OBJECT
private:
    QFileDialog* dialog;
    QStringList fileList;
    xml_reader* xml_worker;
    QThread worker_thread;
    FilesProxy px_obj;

public:
    explicit FilesReader(QObject *parent = nullptr);
    void openOneFile(const QString& nm);
    ~FilesReader();
public slots:

    void makeProxyObject();
    FilesProxy getData();
    void showDialog();
    int getUnitsRead() const { return xml_worker->getLinesCount(); }
    void addToProxyObj(const COMMUTATOR rslt);

signals:
    void selectionDone();
    void objectDone();
    void newProgress(int);
    void letsDo(const QString nm);
    void clearWorker();
    void setFilesNumber(const int);
};

#endif // FILESREADER_H
