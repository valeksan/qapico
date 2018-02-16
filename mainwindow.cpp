#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_pCore(new Core())
{
    ui->setupUi(this);

//    ui->tableWidgetCurrencies->setColumnWidth(0, 200);
//    ui->tableWidgetCurrencies->setColumnWidth(1, 70);
//    ui->tableWidgetCurrencies->setColumnWidth(2, 130);
    ui->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_pCore->registerTask(Tasks::TASK_UPDATE, [this]() -> TaskResult
    {
        TaskResult result;
        // Скачивание страницы в текст
        Downloader html_downloader;
        connect(&html_downloader, &Downloader::onComplete, [=](QVariant data, int downloadType, int error) {
            // формирование списка (парсинг)
            auto doc = QGumboDocument::parse(data.toString().toLatin1().data());
            auto root = doc.rootNode();
            auto tbody = root.getElementsByTagName(HtmlTag::TBODY).front();
            auto currency_nodes = tbody.getElementsByTagName(HtmlTag::TR);
            for(unsigned int index_node = 0; index_node < currency_nodes.size(); index_node++) {
                //
            }
            qDebug() << "SIZE_TR: " << currency_nodes.size();
            //qDebug() << data.toString();
        });
        html_downloader.getData(QUrl("https://coinmarketcap.com/all/views/all/"));
        waitSignal(&html_downloader, &Downloader::onComplete, 10000);
        qDebug() << "UPDATE COMPLETE";                              
        return result;
    }, 0);    

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    //...
    m_pCore->addTask(Tasks::TASK_UPDATE);
}
