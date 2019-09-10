#ifndef MAINWND_H
#define MAINWND_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressDialog>
#include <QString>
#include <QLabel>
#include <QTextEdit>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QPair>
#include <QStringList>

#include "filesreader.h"
#include "filesproxy.h"
#include "table_model.h"
#include "xmlwriter.h"


typedef  QMap<QString, QString> MAP_s_s_;

typedef  QPair<QStringList, QStringList> MAP_lsts;

class mainwnd : public QObject
{
    Q_OBJECT
private:
    QWidget* mainwgt;
    QProgressDialog* progressWnd;
    QTextEdit* txt_edit;
    TABLE_MODEL* p_tModel = nullptr;
    QTableView* table;
    FilesReader* reader;
    QPushButton* import_Btn;
    QPushButton* clear_Btn;
    QMenu *menu;
    QString menuStr;
    QClipboard *buf = QApplication::clipboard();
    bool setTable = false;
    QModelIndex currentIndex;
    FilesProxy objForModel;
    xml_writer xmlWriter;
    QVBoxLayout* lyt;



public:
    explicit mainwnd(QObject *parent = nullptr);

    void setWidgetContent();
    void copyToClipboard(const QString& str);
    bool isTableFill() { return setTable; }
    void startExport();
    void recordData();

//signals:

public slots:
    void setTableModel();
    void clearTable();
    void addProgressInfo();
    void showProgress();
    void customMenuRequested(const QPoint& p);
    void slotActivated(QAction* action);
    void defaultFileSlot();


};

#endif // MAINWND_H
