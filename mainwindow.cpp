#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>

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

//    QUrl url("https://github.com/input-output-hk/cardano-sl/");
//    qDebug() << url.path();
//    qDebug() << url.path().section("/",0,0);
//    qDebug() << url.path().section("/",1,1); \\ OK!

//    qDebug() << convertGithubUrlToApiReq(QUrl("https://github.com/input-output-hk/cardano-sl/"));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    ParserResult resultParseCMC;
    //ParserResult result_2;
    //ParserResult result_3;

    // TEST PARSE MAIN PAGE!
    // https://api.coinmarketcap.com/v1/ticker/?limit=0
    int err;    
    QUrl cmcpApiUrl("https://api.coinmarketcap.com/v1/ticker/?limit=0");
    QByteArray page = Downloader::getHtmlPage(cmcpApiUrl, 15000, err);
    if(err == Downloader::ERR_OK) {
        qDebug() << "Parse: MAIN PAGE";
        Parser parser(page, Parser::TYPE_PARSE_MAIN_PAGE);
        ui->textBrowser->setHtml(page);
        //qDebug() << "page:" << page;
        resultParseCMC = parser.parse();
        qDebug() << "result_is_null:" << resultParseCMC.empty();
        qDebug() << "currencies_size:" << resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().size();
    } else {        
        qDebug() << "error download Main page:" << resultParseCMC.error;
        QMessageBox::critical(this, "error", QString("error download: %1\nerr number: %2").arg(cmcpApiUrl.toString()).arg(err));
        return;
    }


    // TEST PARSE SUB PAGE!
    // https://coinmarketcap.com/currencies/{bitcoin}/
    /*
    QHash<QString,QVariant> values = resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES_INFO_URLS).toHash();
    page = Downloader::getHtmlPage(values.value("bitcoin").toUrl(), 5000, err);
    if(err == Downloader::ERR_OK) {
        qDebug() << "Parse: SUB PAGE";
        Parser parser(page, Parser::TYPE_PARSE_INFO_PAGE);
        result_2 = parser.parse();        
        qDebug() << "result_is_null:" << result_2.empty();
        qDebug() << ":: Parse " << values.value("bitcoin").toUrl() << " : OK";
    } else {        
        qDebug() << "error download Sub page:" << result_2.error;
        return;
    }
    */
    // TEST PARSE GITHUB PAGE!
    // https://api.github.com/{users, orgs}/{:user, :org}/repos
    // https://api.github.com/orgs/valeksan/repos
    // https://github.com/{bitcoin}/ -> https://api.github.com/users/{bitcoin}/repos
    //...
    /*
    QString urlStr = result_2.values.value(Parser::KEY_INF_LIST_SOURCECODE_URLS).value<QList<QString> >().value(0, QString());
    if(!urlStr.isEmpty()) {
        QUrl reqUrl = convertGithubUrlToApiReq(QUrl(urlStr));
        page = Downloader::getHtmlPage(reqUrl, 5000, err);
        if(err == Downloader::ERR_OK) {
            qDebug() << "Parse: GITHUB PAGE";
            Parser parser(page, Parser::TYPE_PARSE_GITHUB_PAGE);
            result_3 = parser.parse();
            qDebug() << "result_is_null:" << result_3.empty();
            qDebug() << ":: Parse " << reqUrl << " : OK";
        } else {
            qDebug() << "error download Sub page:" << result_3.error;
            return;
        }
    } else {
        qDebug() << ":: error: github_url par-r is null!";
        return;
    }
    */
}
