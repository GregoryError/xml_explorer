#include "filesreader.h"

FilesReader::FilesReader(QObject *parent) : QObject(parent)
{
    dialog = new QFileDialog;
    qRegisterMetaType<COMMUTATOR>("COMMUTATOR");

    xml_worker = new xml_reader;
    connect(this, SIGNAL(selectionDone()), this, SLOT(makeProxyObject()));
    connect(this, SIGNAL(letsDo(QString)), xml_worker, SLOT(doWork(QString)));
    connect(this, SIGNAL(clearWorker()), xml_worker, SLOT(clear()));
    connect(this, SIGNAL(setFilesNumber(int)), xml_worker, SLOT(setFilesCountSlot(int)));
    connect(xml_worker, SIGNAL(resultReady(COMMUTATOR)), this, SLOT(addToProxyObj(COMMUTATOR)));
    connect(&worker_thread, &QThread::finished, xml_worker, &QObject::deleteLater);
    connect(xml_worker, SIGNAL(updateBar(int)), this, SIGNAL(newProgress(int)));
    xml_worker->moveToThread(&worker_thread);
    worker_thread.start();
}

FilesProxy FilesReader::getData()
{
    return px_obj;
}

void FilesReader::showDialog()
{
    fileList = dialog->getOpenFileNames(
                nullptr,
                "Select one or more files to open",
                "/home",
                "XML-files (*.xml)");
    emit selectionDone();
}

void FilesReader::addToProxyObj(const COMMUTATOR rslt)
{
    static int perc = 0;

    px_obj.addName(rslt.f_nm);
    px_obj.addValue(rslt.f_nm, rslt.getFileStrings());
    px_obj.addError(rslt.f_nm, rslt.getErrors());
    px_obj.addLinesCount(rslt.getLinesCount());

    if (perc == fileList.size() || *(fileList.begin()) == "default.xml")
    {
        perc = 0;
        emit objectDone();
    }
    ++perc;
}

void FilesReader::makeProxyObject()
{
    px_obj.clear();

    emit setFilesNumber(fileList.size());   // setting data for xml_worker for ProgressBar calc

    for (QString f_name : fileList)
    {
        emit letsDo(f_name);
        emit clearWorker();
    }
}

void FilesReader::openOneFile(const QString &nm)
{
    fileList.append(nm);
    makeProxyObject();
}


FilesReader::~FilesReader()
{
    worker_thread.quit();
    worker_thread.wait();
}
































