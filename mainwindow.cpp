#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pUi(new Ui::MainWindow),
    m_pCore(new Core),
    m_isBaseCurrenciesInit(false),
    m_isInfoCurrenciesInit(false),
    m_isDevCurrenciesInit(false),
    m_tableview_variant(VIEW_TABLE_ACTUAL)
{
    m_pUi->setupUi(this);

    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(m_pUi->horizontalLayoutWidget);
    pMainLayout->addWidget(m_pUi->verticalLayoutWidget/*tableWidgetCurrencies*/, 1);
    pMainLayout->addWidget(m_pUi->widgetControlsBottom);
    pMainLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    pMainLayout->addWidget(m_pUi->progressBar);

    m_pUi->centralWidget->setLayout(pMainLayout);

//    m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_pDb = new DataBase();
    m_pDb->connectToDataBase();

//    if(db->isInMemory())
//        db->sqliteDBMemFile(QString("%1.mem").arg(db->getDatabaseFilename()), false);

    connect(m_pCore, &Core::finishedTask, this, &MainWindow::slotFinishedTask);

    if(!isAppPathExists()) {
        qCritical() << "Missing application directory, and could not create it! (Access)";
        return;
    }

    //...
    m_pUi->pushButtonUpdate_2->setEnabled(m_isBaseCurrenciesInit);
    m_pUi->pushButtonUpdate_3->setEnabled(m_isInfoCurrenciesInit);


//    QUrl url("https://github.com/input-output-hk/cardano-sl/");
//    qDebug() << url.path();
//    qDebug() << url.path().section("/",0,0);
//    qDebug() << url.path().section("/",1,1); \\ OK!

//    qDebug() << convertGithubUrlToApiReq(QUrl("https://github.com/input-output-hk/cardano-sl/"));
    updateColorCheckTableButton();

    registerTasks();

    displayCurrenciesUpdate();

//    m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    //m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    for (int c = 1; c < m_pUi->tableWidgetCurrencies->horizontalHeader()->count(); ++c)
//    {
//        m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
//    }

//    m_pUi->tableWidgetCurrencies->horizontalHeader()->sortIndicatorChanged

//    connect(m_pUi->tableWidgetCurrencies->horizontalHeader(), &QHeaderView::, [&](const QModelIndex &logicalIndex) {
//        Q_UNUSED(logicalIndex)
//        m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//        //m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//        for (int c = 1; c < m_pUi->tableWidgetCurrencies->horizontalHeader()->count(); ++c)
//        {
//            m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
//        }
//    });

    connect(m_pUi->spinBoxPrecisionPrices, SELECT<int>::OVERLOAD_OF(&QSpinBox::valueChanged), [&](int i) {
        Q_UNUSED(i)
        displayCurrenciesUpdate();
    });
    connect(m_pUi->spinBoxPrecisionIncrease, SELECT<int>::OVERLOAD_OF(&QSpinBox::valueChanged), [&](int i) {
        Q_UNUSED(i)
        displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonUSD, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonBTC, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonPR, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonDP, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonSupplyAv, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonSupplyTotal, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
    connect(m_pUi->radioButtonSupplyMax, &QRadioButton::toggled, [&](bool checked) {
        if(checked) displayCurrenciesUpdate();
    });
}

MainWindow::~MainWindow()
{
//    qDebug() << "##1";
//    db->sqliteDBMemFile(QString("%1.mem").arg(db->getDatabaseFilename()), true);
//    qDebug() << "##2";
    m_pCore->deleteLater();
    m_pDb->deleteLater();
//    qDebug() << "##3";
    delete m_pUi;
}

void MainWindow::registerTasks()
{
    // - задача - получение списка C-M-C
    try {
        m_pCore->registerTask(TASK_UPDATE_CURRENCIES_BASE, [this](QVariantList args = QVariantList()) -> TaskResult
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
                    // подготовка таблиц - очистка старых записей в PREV-версии
                    m_pDb->clearTableByIdx(IDX_TABLE_CURRENCIES_PREV);
                    // подготовка таблиц - копирование текущих старых записей в PREV
                    m_pDb->copyTable(IDX_TABLE_CURRENCIES, IDX_TABLE_CURRENCIES_PREV);
                    // подготовка таблиц - очистка старых записей в актуальной таблице
                    m_pDb->clearTableByIdx(IDX_TABLE_CURRENCIES);
                    // сохранение в БД ...
                    qDebug() << "save to DB ...";
                    int size = resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().size();
                    QStringList listId = resultParseCMC.values.value(Parser::KEY_MAIN_TABLE_CURRENCIES).toHash().keys();

                    QMetaObject::invokeMethod(m_pUi->statusBar, "showMessage", Q_ARG(QString, "Обновление списка проектов ..."));
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
                        // вставка записей в актуальную версию таблицы
                        if(!m_pDb->insertIntoCurrenciesTable(hValues, IDX_TABLE_CURRENCIES)) {
                            qDebug() << "fail id: " << listId.at(i);
                        }
                        //qDebug() << (i+1)*100.0/size << "%";
                        QMetaObject::invokeMethod(m_pUi->progressBar, "setValue", Q_ARG(int, (i+1)*100/size));
                    }
                    // выявление недействительных (DEAD) записей - умерших проектов, и запись их в соотв. таблицу
                    m_pDb->copyCurrenciesBetweenTablesByNotExist(IDX_TABLE_CURRENCIES_PREV, IDX_TABLE_CURRENCIES, IDX_TABLE_CURRENCIES_DEAD);
                    // выявление новых (BORN) записей - новых проектов, и запись их в соотв. таблицу
                    m_pDb->copyCurrenciesBetweenTablesByNotExist(IDX_TABLE_CURRENCIES, IDX_TABLE_CURRENCIES_PREV, IDX_TABLE_CURRENCIES_BORN);
                    //db->insertIntoCurrenciesTable()
                    QMetaObject::invokeMethod(m_pUi->statusBar, "showMessage",
                                              Q_ARG(QString, "Обновление списка проектов завершено"),
                                              Q_ARG(int, 5000));
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

    // - задача - получение подробной информации по каждому проекту из C-M-C
    try {
        m_pCore->registerTask(TASK_UPDATE_CURRENCIES_INFO, [this](QVariantList args = QVariantList()) -> TaskResult
        {
            Q_UNUSED(args)
            TaskResult result;
            DownloadError err;
            //...
            return result;
        }, 1);
    } catch (const CoreException &ex) {
        emit showWarrningMsgDialog("Core error register task", ex.message(), true);
    }
}

void MainWindow::displayCurrenciesFromBase(int table_currencies_idx)
{
    QList<int> list_cell_idx;
    double price_usd;
    double price_btc;
    double tmpValue;
    list_cell_idx.append(IDX_CURRENCIES_ID);
    list_cell_idx.append(IDX_CURRENCIES_NAME);
    list_cell_idx.append(IDX_CURRENCIES_SYMBOL);
    list_cell_idx.append(IDX_CURRENCIES_RANK);
    list_cell_idx.append(IDX_CURRENCIES_PRICE_USD);
    list_cell_idx.append(IDX_CURRENCIES_PRICE_BTC);
    list_cell_idx.append(IDX_CURRENCIES_VOL24H_USD);
    list_cell_idx.append(IDX_CURRENCIES_MARKETCAP_USD);
    list_cell_idx.append(IDX_CURRENCIES_AVAIBLE_SUPPLY);
    list_cell_idx.append(IDX_CURRENCIES_TOTAL_SUPPLY);
    list_cell_idx.append(IDX_CURRENCIES_MAX_SUPPLY);
    list_cell_idx.append(IDX_CURRENCIES_PERCENT_CH_1H);
    list_cell_idx.append(IDX_CURRENCIES_PERCENT_CH_24H);
    list_cell_idx.append(IDX_CURRENCIES_PERCENT_CH_7D);
    list_cell_idx.append(IDX_CURRENCIES_LAST_UPDATE_DATE);
    list_cell_idx.append(IDX_CURRENCIES_CMC_PAGE_URL);
    QList<QHash<int,QVariant> > resultList = m_pDb->selectFromCurrenciesTable(list_cell_idx, table_currencies_idx);

//    if(resultList.empty()) {
//        return;
//    }
    m_pUi->tableWidgetCurrencies->setSortingEnabled(false);
    while (m_pUi->tableWidgetCurrencies->rowCount() != 0) m_pUi->tableWidgetCurrencies->removeRow(0);



    //for(int i=0; i<resultList.size(); i++) m_pUi->tableWidgetCurrencies->insertRow(i);
    for(int i=0; i<resultList.size(); i++) {
        m_pUi->tableWidgetCurrencies->insertRow(i);
        //int row_index = resultList.value(i).value(IDX_CURRENCIES_RANK).toInt()-1;
        m_pUi->tableWidgetCurrencies->setItem(i, 0, new TableNumberItem(resultList.value(i).value(IDX_CURRENCIES_RANK).toString()));
        m_pUi->tableWidgetCurrencies->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 0), resultList.value(i).value(IDX_CURRENCIES_RANK), Qt::DisplayRole);
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
        m_pUi->tableWidgetCurrencies->setItem(i, 1, new TableSimpleItem(resultList.value(i).value(IDX_CURRENCIES_NAME).toString()));
        m_pUi->tableWidgetCurrencies->setItem(i, 2, new TableSimpleItem(resultList.value(i).value(IDX_CURRENCIES_SYMBOL).toString()));
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 2), resultList.value(i).value(IDX_CURRENCIES_SYMBOL), Qt::DisplayRole);
        m_pUi->tableWidgetCurrencies->setItem(i, 3, new TableSimpleItem(resultList.value(i).value(IDX_CURRENCIES_MARKETCAP_USD).toString()));
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 3), QString("$")+resultList.value(i).value(IDX_CURRENCIES_MARKETCAP_USD).toString(), Qt::DisplayRole);
        if(m_pUi->radioButtonUSD->isChecked()) {
            price_usd = resultList.value(i).value(IDX_CURRENCIES_PRICE_USD).toDouble();
            m_pUi->tableWidgetCurrencies->setItem(i, 4, new TableNumberItem(QString("$")+QString::number(price_usd,'f',m_pUi->spinBoxPrecisionPrices->value())));
//            m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 4), QString("$")+QString::number(price_usd,'f',m_pUi->spinBoxPrecisionPrices->value()), Qt::DisplayRole);
        } else {
            price_btc = resultList.value(i).value(IDX_CURRENCIES_PRICE_BTC).toDouble();
            m_pUi->tableWidgetCurrencies->setItem(i, 4, new TableNumberItem(QString::number(price_btc,'f',m_pUi->spinBoxPrecisionPrices->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 4), QString::number(price_btc,'f',m_pUi->spinBoxPrecisionPrices->value()), Qt::DisplayRole);
        }
        if(m_pUi->radioButtonSupplyAv->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 5, new TableNumberItem(resultList.value(i).value(IDX_CURRENCIES_AVAIBLE_SUPPLY).toString()));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 5), resultList.value(i).value(IDX_CURRENCIES_AVAIBLE_SUPPLY), Qt::DisplayRole);
        } else if(m_pUi->radioButtonSupplyTotal->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 5, new TableNumberItem(resultList.value(i).value(IDX_CURRENCIES_TOTAL_SUPPLY).toString()));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 5), resultList.value(i).value(IDX_CURRENCIES_TOTAL_SUPPLY), Qt::DisplayRole);
        } else {
            m_pUi->tableWidgetCurrencies->setItem(i, 5, new TableNumberItem(resultList.value(i).value(IDX_CURRENCIES_MAX_SUPPLY).toString()));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 5), resultList.value(i).value(IDX_CURRENCIES_MAX_SUPPLY), Qt::DisplayRole);
        }

        m_pUi->tableWidgetCurrencies->setItem(i, 6, new TableNumberItem(QString("$")+resultList.value(i).value(IDX_CURRENCIES_VOL24H_USD).toString()));
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 6), QString("$")+resultList.value(i).value(IDX_CURRENCIES_VOL24H_USD).toString(), Qt::DisplayRole);

        tmpValue = resultList.value(i).value(IDX_CURRENCIES_PERCENT_CH_1H).toDouble();
        if(m_pUi->radioButtonPR->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 7, new TableNumberItem(getStrCalcPercentIncreaseValue(tmpValue, m_pUi->spinBoxPrecisionIncrease->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 7), getStrCalcPercentIncreaseValue(tmpValue, m_pUi->spinBoxPrecisionIncrease->value()), Qt::DisplayRole);
        } else if(m_pUi->radioButtonUSD->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 7, new TableNumberItem(getStrCalcDeltaIncreaseValue(price_usd, tmpValue, m_pUi->spinBoxPrecisionIncrease->value(),"$")));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 7), getStrCalcDeltaIncreaseValue(price_usd, tmpValue, m_pUi->spinBoxPrecisionIncrease->value(),"$"), Qt::DisplayRole);
        } else {
            m_pUi->tableWidgetCurrencies->setItem(i, 7, new TableNumberItem(getStrCalcDeltaIncreaseValue(price_btc, tmpValue, m_pUi->spinBoxPrecisionIncrease->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 7), getStrCalcDeltaIncreaseValue(price_btc, tmpValue, m_pUi->spinBoxPrecisionIncrease->value()), Qt::DisplayRole);
        }
        m_pUi->tableWidgetCurrencies->item(i, 7)->setTextColor(QColor((tmpValue < 0) ? Qt::red : ((tmpValue > 0) ? Qt::darkGreen : Qt::black)));
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 7), QColor((tmpValue < 0) ? Qt::red : ((tmpValue > 0) ? Qt::darkGreen : Qt::black)), Qt::TextColorRole);

        tmpValue = resultList.value(i).value(IDX_CURRENCIES_PERCENT_CH_24H).toDouble();
        if(m_pUi->radioButtonPR->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 8, new TableNumberItem(getStrCalcPercentIncreaseValue(tmpValue, m_pUi->spinBoxPrecisionIncrease->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 8), getStrCalcPercentIncreaseValue(tmpValue, m_pUi->spinBoxPrecisionIncrease->value()), Qt::DisplayRole);
        } else if(m_pUi->radioButtonUSD->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 8, new TableNumberItem(getStrCalcDeltaIncreaseValue(price_usd, tmpValue, m_pUi->spinBoxPrecisionIncrease->value(),"$")));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 8), getStrCalcDeltaIncreaseValue(price_usd, tmpValue, m_pUi->spinBoxPrecisionIncrease->value(),"$"), Qt::DisplayRole);
        } else {
            m_pUi->tableWidgetCurrencies->setItem(i, 8, new TableNumberItem(getStrCalcDeltaIncreaseValue(price_btc, tmpValue, m_pUi->spinBoxPrecisionIncrease->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 8), getStrCalcDeltaIncreaseValue(price_btc, tmpValue, m_pUi->spinBoxPrecisionIncrease->value()), Qt::DisplayRole);
        }
        m_pUi->tableWidgetCurrencies->item(i, 8)->setTextColor(QColor((tmpValue < 0) ? Qt::red : ((tmpValue > 0) ? Qt::darkGreen : Qt::black)));
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 8), QColor((tmpValue < 0) ? Qt::red : ((tmpValue > 0) ? Qt::darkGreen : Qt::black)), Qt::TextColorRole);

        tmpValue = resultList.value(i).value(IDX_CURRENCIES_PERCENT_CH_7D).toDouble();
        if(m_pUi->radioButtonPR->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 9, new TableNumberItem(getStrCalcPercentIncreaseValue(tmpValue, m_pUi->spinBoxPrecisionIncrease->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 9), getStrCalcPercentIncreaseValue(tmpValue, m_pUi->spinBoxPrecisionIncrease->value()), Qt::DisplayRole);
        } else if(m_pUi->radioButtonUSD->isChecked()) {
            m_pUi->tableWidgetCurrencies->setItem(i, 9, new TableNumberItem(getStrCalcDeltaIncreaseValue(price_usd, tmpValue, m_pUi->spinBoxPrecisionIncrease->value(),"$")));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 9), getStrCalcDeltaIncreaseValue(price_usd, tmpValue, m_pUi->spinBoxPrecisionIncrease->value(),"$"), Qt::DisplayRole);
        } else {
            m_pUi->tableWidgetCurrencies->setItem(i, 9, new TableNumberItem(getStrCalcDeltaIncreaseValue(price_btc, tmpValue, m_pUi->spinBoxPrecisionIncrease->value())));
            //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 9), getStrCalcDeltaIncreaseValue(price_btc, tmpValue, m_pUi->spinBoxPrecisionIncrease->value()), Qt::DisplayRole);
        }
        m_pUi->tableWidgetCurrencies->item(i, 9)->setTextColor(QColor((tmpValue < 0) ? Qt::red : ((tmpValue > 0) ? Qt::darkGreen : Qt::black)));
        //m_pUi->tableWidgetCurrencies->model()->setData(m_pUi->tableWidgetCurrencies->model()->index(i, 9), QColor((tmpValue < 0) ? Qt::red : ((tmpValue > 0) ? Qt::darkGreen : Qt::black)), Qt::TextColorRole);
    }

    m_pUi->tableWidgetCurrencies->setSortingEnabled(true);
    m_pUi->tableWidgetCurrencies->sortByColumn(0, Qt::AscendingOrder);

    m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    for (int c = 1; c < m_pUi->tableWidgetCurrencies->horizontalHeader()->count(); ++c)
    {
        m_pUi->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }
}

void MainWindow::displayCurrenciesUpdate(){
    switch (m_tableview_variant) {
    case VIEW_TABLE_ACTUAL:
        this->displayCurrenciesFromBase(IDX_TABLE_CURRENCIES);
        break;
    case VIEW_TABLE_PREV_VALUES:
        this->displayCurrenciesFromBase(IDX_TABLE_CURRENCIES_PREV);
        break;
    case VIEW_TABLE_DEAD_PROJECTS:
        this->displayCurrenciesFromBase(IDX_TABLE_CURRENCIES_DEAD);
        break;
    case VIEW_TABLE_NEW_PROJECTS:
        this->displayCurrenciesFromBase(IDX_TABLE_CURRENCIES_BORN);
        break;
    default:
        this->displayCurrenciesFromBase();
        break;
    }
}

void MainWindow::updateColorCheckTableButton()
{
    switch (m_tableview_variant) {
    case VIEW_TABLE_ACTUAL:
        m_pUi->pushButtonActual->setStyleSheet("background-color: rgb(226, 252, 175);");
        m_pUi->pushButtonPrevScan->setStyleSheet("");
        m_pUi->pushButtonDeadProjects->setStyleSheet("");
        m_pUi->pushButtonNewProjects->setStyleSheet("");
        m_pUi->frame->hide();
        break;
    case VIEW_TABLE_PREV_VALUES:
        m_pUi->pushButtonActual->setStyleSheet("");
        m_pUi->pushButtonPrevScan->setStyleSheet("background-color: rgb(226, 252, 175);");
        m_pUi->pushButtonDeadProjects->setStyleSheet("");
        m_pUi->pushButtonNewProjects->setStyleSheet("");
        m_pUi->frame->hide();
        break;
    case VIEW_TABLE_DEAD_PROJECTS:
        m_pUi->pushButtonActual->setStyleSheet("");
        m_pUi->pushButtonPrevScan->setStyleSheet("");
        m_pUi->pushButtonDeadProjects->setStyleSheet("background-color: rgb(226, 252, 175);");
        m_pUi->pushButtonNewProjects->setStyleSheet("");
        m_pUi->frame->show();
        break;
    case VIEW_TABLE_NEW_PROJECTS:
        m_pUi->pushButtonActual->setStyleSheet("");
        m_pUi->pushButtonPrevScan->setStyleSheet("");
        m_pUi->pushButtonDeadProjects->setStyleSheet("");
        m_pUi->pushButtonNewProjects->setStyleSheet("background-color: rgb(226, 252, 175);");
        m_pUi->frame->show();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    m_pCore->addTask(TASK_UPDATE_CURRENCIES_BASE, QVariantList());

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
        // метод отображения списка из базы тут будет
        displayCurrenciesFromBase(IDX_TABLE_CURRENCIES);
    }
}

QString MainWindow::getStrCalcDeltaIncreaseValue(double price, double percent, int precision, QString currency)
{
    double result = fabs(price-(price+price*percent/100.0));

    return (percent > 0) ? (QString("+%1").arg(currency)+QString::number(result, 'f', precision)) : (QString("-%1").arg(currency)+QString::number(result, 'f', precision));
}

QString MainWindow::getStrCalcPercentIncreaseValue(double percent, int precision)
{
    return (percent > 0) ? (QString("+")+QString::number(percent,'f',precision)+QString("%")) : (QString::number(percent,'f',precision)+QString("%"));
}

void MainWindow::on_pushButtonActual_clicked()
{
    m_tableview_variant = VIEW_TABLE_ACTUAL;
    updateColorCheckTableButton();
    displayCurrenciesUpdate();
}

void MainWindow::on_pushButtonPrevScan_clicked()
{
    m_tableview_variant = VIEW_TABLE_PREV_VALUES;
    updateColorCheckTableButton();
    displayCurrenciesUpdate();
}

void MainWindow::on_pushButtonNewProjects_clicked()
{
    m_tableview_variant = VIEW_TABLE_NEW_PROJECTS;
    updateColorCheckTableButton();
    displayCurrenciesUpdate();
}

void MainWindow::on_pushButtonDeadProjects_clicked()
{
    m_tableview_variant = VIEW_TABLE_DEAD_PROJECTS;
    updateColorCheckTableButton();
    displayCurrenciesUpdate();
}
