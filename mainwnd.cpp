#include "mainwnd.h"
#include <QDebug>

mainwnd::mainwnd(QObject *parent) : QObject(parent)
{
    mainwgt = new QWidget;
    table = new QTableView;
    reader = new FilesReader;
    import_Btn = new QPushButton;
    clear_Btn = new QPushButton;

    progressWnd = new QProgressDialog; // ("Импорт файлов...", "Закрыть", 0, 100);
    lyt = new QVBoxLayout(progressWnd);
    txt_edit = new QTextEdit;
    lyt->addWidget(txt_edit);
    progressWnd->close();
    progressWnd->setLabelText("Импорт файлов...");
    progressWnd->setCancelButtonText("Закрыть");
    progressWnd->setMaximum(100);
    progressWnd->setMinimum(1);
    progressWnd->setAutoClose(false);
    progressWnd->setAutoReset(false);
    progressWnd->setFixedSize(mainwgt->width() * 0.8, mainwgt->width() / 2);
    txt_edit->setFontPointSize(10);
    txt_edit->setMaximumSize(progressWnd->width() - 30, progressWnd->height() / 2);

    import_Btn->setText("ИМПОРТ");
    import_Btn->setMinimumHeight(40);

    clear_Btn->setIcon(QIcon(":/TrashBin.png"));
    clear_Btn->setMinimumHeight(40);

    connect(import_Btn, SIGNAL(clicked()), reader, SLOT(showDialog()));
    connect(reader, SIGNAL(selectionDone()), this, SLOT(showProgress()));
    connect(reader, SIGNAL(objectDone()), this, SLOT(addProgressInfo()));
    connect(reader, SIGNAL(objectDone()), this, SLOT(setTableModel()));
    connect(clear_Btn, SIGNAL(clicked()), this, SLOT(clearTable()));
    connect(reader, SIGNAL(newProgress(int)), progressWnd, SLOT(setValue(int)));


    table->setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu();
    connect(table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slotActivated(QAction*)));

    menu->addAction(new QAction("Добавить", this));
    menu->addAction(new QAction("Удалить", this));
    menu->addAction(new QAction("Копировать", this));
    menu->addAction(new QAction("Изменить здесь", this));
    menu->addAction(new QAction("Очистить здесь", this));
    menu->addAction(new QAction("Очистить все", this));
    menu->addAction(new QAction("Выделить все", this));
    menu->addAction(new QAction("Экспортировать все", this));
    menu->setEnabled(false);

    // Lyout setup

    mainwgt->setMinimumSize(640, 480);

    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(clear_Btn);
    phbxLayout->addSpacing(table->width() / 3);
    phbxLayout->addWidget(import_Btn);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addLayout(phbxLayout);
    pvbxLayout->addWidget(table);
    mainwgt->setLayout(pvbxLayout);
    mainwgt->show();

    defaultFileSlot();

}

void mainwnd::setTableModel()
{
    //ShowProgressBar();

    p_tModel = new TABLE_MODEL();

    objForModel = reader->getData();

    p_tModel->setSize(objForModel.getSize(), 2);                         // size should`t be more than >setObjName(....
    p_tModel->setObjName(QStringList() << "Имя:" << "Значение:");

    QStringList rowsNums;

    int rows;
    for (rows = 0; rows < objForModel.getSize(); ++rows)
        rowsNums << QString::number(rows + 1);
    p_tModel->setRowsNames(rowsNums);
    rows = 0;
    for (QString& name : objForModel.getNamesLst())
    {
        for (const QString &str : objForModel.getFilePairs(name).first)
        {
            p_tModel->setValue(rows++, 0, str);
        }
    }
    rows = 0;
    for (QString& name : objForModel.getNamesLst())
    {
        for (const QString &str : objForModel.getFilePairs(name).second)
        {
            p_tModel->setValue(rows++, 1, str);
        }
    }
    table->setModel(p_tModel);
    table->setColumnWidth(0, mainwgt->width() / 2 - 45);
    table->setColumnWidth(1, mainwgt->width() / 2 - 45);
    menu->setEnabled(true);
    setTable = true;
}

void mainwnd::clearTable()
{
    delete p_tModel;
}

void mainwnd::addProgressInfo()
{

    int count = 1;
    QString err_str("Файлы:\n\n");
    for (const QString& e : reader->getData().getNamesLst())
    {
        err_str += QString::number(count++) + ". " + e + ": \n";
        err_str += ( reader->getData().getErrorMap()
                     .value(e).isEmpty() ? "Успешно!"
                                         : reader->getData().getErrorMap().value(e) ) + "\n\n";
    }

    txt_edit->setPlainText(err_str);
}

void mainwnd::showProgress()
{
    progressWnd->show();
}

void mainwnd::customMenuRequested(const QPoint &p)
{
    if (table->indexAt(p).isValid())
    {
        currentIndex = table->indexAt(p);
        menu->popup(table->viewport()->mapToGlobal(p));
        menuStr = p_tModel->data(currentIndex, Qt::EditRole).toString();
    }
}

void mainwnd::slotActivated(QAction *action)
{

    // TODO here

    if (action->text() == "Добавить")
        p_tModel->insertRows(p_tModel->getRowsCount(), 1);
    if (action->text() == "Удалить")
        p_tModel->removeRows(p_tModel->getRowsCount() - 1, 1);
    if (action->text() == "Копировать")
        copyToClipboard(menuStr);
    if (action->text() == "Изменить здесь")
        table->edit(currentIndex);
    if (action->text() == "Очистить здесь")
        p_tModel->setData(currentIndex, "", Qt::EditRole);
    if (action->text() == "Очистить все")
        p_tModel->clear();
    if (action->text() == "Выделить все")
        table->selectAll();
    if (action->text() == "Экспортировать все")
        startExport();

    qDebug() << action->text();
}

void mainwnd::copyToClipboard(const QString &str)
{
    buf->setText(str, QClipboard::Clipboard);
}

void mainwnd::startExport()
{
    if (QMessageBox::warning(nullptr,
                             ("Экспорт записей"),
                             ("Вы уверены, что хотите экспортировать все записи?\n"
                              "Файлы будут перезаписаны!"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        recordData();
    }
}

void mainwnd::recordData()
{
    QStringList f_names = objForModel.getNamesLst();

    QStringList column_0, column_1;

    for (int i = 0; i < p_tModel->getRowsCount(); ++i)
    {
        column_0.append(p_tModel->getCellContent(i, 0).toString());
        column_1.append(p_tModel->getCellContent(i, 1).toString());
    }

    QMessageBox msgBox;
    msgBox.setText(xmlWriter.writeToFile(f_names, column_0, column_1));
    msgBox.exec();

}

void mainwnd::defaultFileSlot()
{
    xmlWriter.writeToFile(QStringList() << "default.xml", QStringList()
                          << "tag", QStringList() << "data");
    reader->openOneFile("default.xml");
}































