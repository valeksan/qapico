#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    db = new DataBase();
    db->connectToDataBase();

    if(!isAppPathExists()) {
        qCritical() << "Missing application directory, and could not create it! (Access)";
        return;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    // TEST PARSE MAIN PAGE!
    int err;
    ParserResult result;
    ParserResult result_2;
    QByteArray page = Downloader::getHtmlPage(QUrl("https://api.coinmarketcap.com/v1/ticker/?limit=0"), 2000, err);
    if(err == Downloader::ERR_OK) {
        Parser parser(page, Parser::TYPE_PARSE_MAIN_PAGE);
        result = parser.parse();
        qDebug() << "currencies_size:" << result.values.value("currencies").toHash().size();
    } else {
        qDebug() << "result_is_null:" << result.empty();
        qDebug() << "error download Main page:" << result.error;
        return;
    }
    // TEST PARSE SUB PAGE!
    QHash<QString,QVariant> values = result.values.value("currencies_sub_urls").toHash();
    page = Downloader::getHtmlPage(values.value("bitcoin").toUrl(), 5000, err);

    if(err == Downloader::ERR_OK) {
        QFile file("1.html");
        if( file.open(QIODevice::WriteOnly) ) {
            file.write(page); // пишем в файл
        }
        Parser parser(page, Parser::TYPE_PARSE_SUB_PAGE);
        result_2 = parser.parse();
        qDebug() << ":: Parse " << values.value("bitcoin").toUrl() << " : OK";
    } else {
        qDebug() << "result_is_null:" << result_2.empty();
        qDebug() << "error download Sub page:" << result_2.error;
        return;
    }
}
