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
    core->deleteLater();
    db->deleteLater();
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
                    QHash<int, QVariant> hValues;
                    for(int i=0; i<size; i++) {
                        hValues.insert(IDX_CURRENCIES_CMC_PAGE_URL, resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES_INFO_URLS).toHash().value(listId.at(i)));
                        hValues.insert(IDX_CURRENCIES_SYMBOL, listId.at(i));
                        //
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
