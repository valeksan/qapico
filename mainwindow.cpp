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
    QByteArray page = Downloader::getHtmlPage(QUrl("https://api.coinmarketcap.com/v1/ticker/?limit=0"), 1000, err);
    if(err == Downloader::ERR_OK) {
        Parser parser(page, Parser::TYPE_PARSE_MAIN_PAGE);
        ParserResult result = parser.parse();
        qDebug() << "error:" << result.error;
        qDebug() << "result_is_null:" << result.empty();
        qDebug() << "currencies_size:" << result.values.value("currencies").toHash().size();
    }
}
