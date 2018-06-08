#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    core(new Core),
    m_isBaseCurrenciesInit(false),
    m_isInfoCurrenciesInit(false),
    m_isDevCurrenciesInit(false)
{
    ui->setupUi(this);

    ui->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    db = new DataBase();
    db->connectToDataBase();

//    if(db->isInMemory())
//        db->sqliteDBMemFile(QString("%1.mem").arg(db->getDatabaseFilename()), false);

    connect(core, &Core::finishedTask, this, &MainWindow::slotFinishedTask);

    if(!isAppPathExists()) {
        qCritical() << "Missing application directory, and could not create it! (Access)";
        return;
    }

    //...
    ui->pushButtonUpdate_2->setEnabled(m_isBaseCurrenciesInit);
    ui->pushButtonUpdate_3->setEnabled(m_isInfoCurrenciesInit);


//    QUrl url("https://github.com/input-output-hk/cardano-sl/");
//    qDebug() << url.path();
//    qDebug() << url.path().section("/",0,0);
//    qDebug() << url.path().section("/",1,1); \\ OK!

//    qDebug() << convertGithubUrlToApiReq(QUrl("https://github.com/input-output-hk/cardano-sl/"));

    registerTasks();
}

MainWindow::~MainWindow()
{
//    qDebug() << "##1";
//    db->sqliteDBMemFile(QString("%1.mem").arg(db->getDatabaseFilename()), true);
//    qDebug() << "##2";
    core->deleteLater();
    db->deleteLater();
//    qDebug() << "##3";
    delete ui;
}

void MainWindow::registerTasks()
{
    // - задача - получение списка C-M-C
    try {
        core->registerTask(TASK_UPDATE_CURRENCIES_BASE, [this](QVariantList args = QVariantList()) -> TaskResult
        {
            Q_UNUSED(args)
            TaskResult result;
            DownloadError err;
            QUrl cmcpApiUrl("https://api.coinmarketcap.com/v1/ticker/?limit=0");
            QByteArray page = Downloader::getHtmlPage(cmcpApiUrl, 15000, err);
            if(err.error == DownloadError::ERR_OK) {
                qDebug() << "Parse: MAIN PAGE";
                Parser parser(page, Parser::TYPE_PARSE_MAIN_PAGE);
                ParserResult resultParseCMC = parser.parse();
                qDebug() << "currencies_size:" << resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().size();
                if(resultParseCMC.error == Parser::ERR_OK) {
                    // сохранение в БД ...
                    qDebug() << "save to DB ...";
                    int size = resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().size();
                    QStringList listId = resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().keys();

                    for(int i=0; i<size; i++) {
                        QHash<int, QVariant> hValues;
                        //hValues.insert(IDX_CURRENCIES_NAME, resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().value(listId.at(i)).value<QHash<int,QVariant> >().value(Parser::KEY_CYR_NAME_ATTR));
                        hValues.insert(IDX_CURRENCIES_ID, listId.at(i));
                        hValues.insert(IDX_CURRENCIES_NAME, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_NAME_ATTR));
                        hValues.insert(IDX_CURRENCIES_SYMBOL, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_CYMBOL_ATTR));
                        hValues.insert(IDX_CURRENCIES_RANK, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_RANK_ATTR));
                        hValues.insert(IDX_CURRENCIES_PRICE_USD, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_PRICE_USD_ATTR));
                        hValues.insert(IDX_CURRENCIES_PRICE_BTC, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_PRICE_BTC_ATTR));
                        hValues.insert(IDX_CURRENCIES_VOL24H_USD, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_24H_VOLUME_USD_ATTR));
                        hValues.insert(IDX_CURRENCIES_MARKETCAP_USD, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_MARKETCAP_USD_ATTR));
                        hValues.insert(IDX_CURRENCIES_AVAIBLE_SUPPLY, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_AVAIBLE_SUPPLY_ATTR));
                        hValues.insert(IDX_CURRENCIES_TOTAL_SUPPLY, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_TOTAL_SUPPLY_ATTR));
                        hValues.insert(IDX_CURRENCIES_MAX_SUPPLY, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_MAX_SUPPLY_ATTR));
                        hValues.insert(IDX_CURRENCIES_PERCENT_CH_1H, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_PERCENT_CH_1H_ATTR));
                        hValues.insert(IDX_CURRENCIES_PERCENT_CH_24H, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_PERCENT_CH_24H_ATTR));
                        hValues.insert(IDX_CURRENCIES_PERCENT_CH_7D, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_PERCENT_CH_7D_ATTR));
                        hValues.insert(IDX_CURRENCIES_LAST_UPDATE_DATE, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES, Parser::KEY_CYR_DATE_LAST_UPDATED_ATTR));
                        hValues.insert(IDX_CURRENCIES_CMC_PAGE_URL, Parser::getResultValue(resultParseCMC, listId.at(i), Parser::TYPE_PARSE_MAIN_PAGE, Parser::KEY_MAIN_TABLE_CURRENCIES_INFO_URLS));
                        //
                        if(!db->insertIntoCurrenciesTable(hValues, false)) {
                            qDebug() << "fail id: " << listId.at(i);
                        }
                        qDebug() << (i+1)*100.0/size << "%";
                    }
                    //db->insertIntoCurrenciesTable()
                } else {
                    result.error = ERR_TASK_PARSE_PAGE_FAIL;
                    result.errorExt1 = resultParseCMC.error;
                    result.errorExt2 = -1;
                    result.textOutputMsg = parser.textErr();
                    result.textOutputTitle = "Error parsing data";
                    result.toShowErrorOutputMsg = true;
                }
            } else {
                result.error = ERR_TASK_DOWNLOAD_PAGE_FOR_PARSE_FAIL;
                result.errorExt1 = err.error;
                result.errorExt2 = err.errorReply;
                result.textOutputMsg = err.errorReplyText;
                result.textOutputTitle = "Error download data";
                result.toShowErrorOutputMsg = true;
            }
            //...
            return result;
        }, 1);
    } catch (const CoreException &ex) {
        emit showWarrningMsgDialog("Core error register task", ex.message(), true);
    }
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    core->addTask(TASK_UPDATE_CURRENCIES_BASE, QVariantList());

//    ParserResult resultParseCMC;
//    //ParserResult result_2;
//    //ParserResult result_3;

//    // TEST PARSE MAIN PAGE!
//    // https://api.coinmarketcap.com/v1/ticker/?limit=0
//    DownloadError err;
//    QUrl cmcpApiUrl("https://api.coinmarketcap.com/v1/ticker/?limit=0");
//    QByteArray page = Downloader::getHtmlPage(cmcpApiUrl, 15000, err);
//    if(err.error == DownloadError::ERR_OK) {
//        qDebug() << "Parse: MAIN PAGE";
//        Parser parser(page, Parser::TYPE_PARSE_MAIN_PAGE);
//        ui->textBrowser->setHtml(page);
//        //qDebug() << "page:" << page;
//        resultParseCMC = parser.parse();
//        qDebug() << "result_is_null:" << resultParseCMC.empty();
//        qDebug() << "currencies_size:" << resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().size();
//    } else {
//        qDebug() << "error download Main page:" << resultParseCMC.error;
//        QMessageBox::critical(this, "error", QString("error download: %1\nerr number: %2-%3").arg(cmcpApiUrl.toString()).arg(err.error).arg(err.errorReply));
//        return;
//    }


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

void MainWindow::slotFinishedTask(long id, int type, QVariantList argsList, QVariant result)
{
    Q_UNUSED(id)
    Q_UNUSED(argsList)
    if(type == TASK_UPDATE_CURRENCIES_BASE && result.value<TaskResult>().error == ERR_TASK_OK) {
        m_isBaseCurrenciesInit = true;
    }
}
