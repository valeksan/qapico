#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_pCore(new Core())
{
    ui->setupUi(this);

    ui->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /* Первым делом необходимо создать объект для работы с базой данных
     * и инициализировать подключение к базе данных
     * */    
    db = new DataBase();
    db->connectToDataBase();

    /* Регистрация нужных задач для выполнения ядром (мультипотоковым менеджером задач) *
    try {
        m_pCore->registerTask(Tasks::TASK_UPDATE, [this]() -> TaskResult
        {
            TaskResult result;
            // Скачивание страницы в текст
            Downloader html_downloader;
            int downloadTypeD = 0;
            int errorD = false;
            QVariantList argsD;
            connect(&html_downloader, &Downloader::onComplete, [=,&downloadTypeD,&errorD,&argsD](QVariant data, int downloadType, int error, QVariantList args) {
                downloadTypeD = downloadType;
                errorD = error;
                argsD = args;
                // формирование списка (парсинг)
                auto doc = QGumboDocument::parse(data.toString().toLatin1().data());
                auto root = doc.rootNode();
                auto tbody = root.getElementsByTagName(HtmlTag::TBODY).front();
                auto currency_nodes = tbody.getElementsByTagName(HtmlTag::TR);                
                for(unsigned int index_node = 0; index_node < currency_nodes.size(); index_node++) {
                    auto table_nodes = currency_nodes.at(index_node).getElementsByTagName(HtmlTag::TD);
                    // TD(1): get curency name
                    auto currency_td1_a_tag = table_nodes.at(1).getElementsByTagName(HtmlTag::SPAN).front().getElementsByTagName(HtmlTag::A).front();
                    QString currency_href = currency_td1_a_tag.getAttribute("href");
                    QUrl currency_url = QUrl(QString("https://coinmarketcap.com%1").arg(currency_href));
                    QString currency_name = currency_href.section("/", 2, 2);
                    currency_name = currency_name.left(1).toUpper()+currency_name.mid(1);
                    QString symbol_name = currency_td1_a_tag.innerText();
                    qDebug() << currency_name << " : " << symbol_name;
                    // TD(4): get currency prices
                    auto currency_td4_a_tag = table_nodes.at(4).getElementsByTagName(HtmlTag::A).front();
                    double price_usd = currency_td4_a_tag.getAttribute("data-usd").toDouble();
                    double price_btc = currency_td4_a_tag.getAttribute("data-btc").toDouble();
                    qDebug() << "price_usd:" << price_usd << " price_btc:" << price_btc;
                    // TD(5): get supply and mineable flag
                    auto nodes_supply_a = table_nodes.at(5).getElementsByTagName(HtmlTag::A);
                    auto nodes_supply_span = (nodes_supply_a.empty() ? table_nodes.at(5).getElementsByTagName(HtmlTag::SPAN) : QGumboNodes());
                    unsigned long long calculating_supply = 0;
                    if(!nodes_supply_a.empty()) {
                        calculating_supply = static_cast<unsigned long long>(nodes_supply_a.front().getAttribute("data-supply").toDouble());
                    } else if(!nodes_supply_span.empty()) {
                        calculating_supply = static_cast<unsigned long long>(nodes_supply_span.front().getAttribute("data-supply").toDouble());
                    }
                    bool is_mineable = !table_nodes.at(5).innerText().contains("*");
                    qDebug() << "calculating_supply:" << calculating_supply << " is_mineable:" << is_mineable;
                    // TD(6): get volume
                    auto nodes_volume_a = table_nodes.at(6).getElementsByTagName(HtmlTag::A);
                    auto nodes_volume_span = (nodes_supply_a.empty() ? table_nodes.at(6).getElementsByTagName(HtmlTag::SPAN) : QGumboNodes());
                    double volume_usd = 0.0;
                    double volume_btc = 0.0;
                    if(!nodes_volume_a.empty()) {
                        volume_usd = nodes_volume_a.front().getAttribute("data-usd").toDouble();
                        volume_btc = nodes_volume_a.front().getAttribute("data-btc").toDouble();
                    } else if(!nodes_volume_span.empty()) {
                        volume_usd = nodes_volume_span.front().getAttribute("data-usd").toDouble();
                        volume_btc = nodes_volume_span.front().getAttribute("data-btc").toDouble();
                    }
                    qDebug() << "volume_usd:" << volume_usd << " volume_btc:" << volume_btc;
                    m_mutex.lock();
                    m_currenciesPagesForParse.insert(currency_name, currency_url); //.append(QPair<QString,QUrl>(currency_name, currency_url));
                    m_mutex.unlock();
                }
                qDebug() << "SIZE_TR: " << currency_nodes.size();
                //qDebug() << data.toString();
            });
            html_downloader.getData(QUrl("https://coinmarketcap.com/all/views/all/"), D_TYPE_TEXT);
            bool complete_parse_main_page = waitSignal(&html_downloader, &Downloader::onComplete, 10000);
            bool complete_parse_currencies_pages = false;
            if(complete_parse_main_page && errorD == D_NO_ERR) {
                connect(&html_downloader, &Downloader::onCompleteList, [=](QList<QSharedPointer<DownloadResult> > results) {
                    //
                    qDebug() << "AAAAAAAAAAAAA THIS WORK!!! size:" << results.size();
                });
                html_downloader.getDataList(m_currenciesPagesForParse.values(), downloadTypeD, argsD);
                complete_parse_currencies_pages = waitSignal(&html_downloader, &Downloader::onCompleteList, 10000);
            }
            qDebug() << "UPDATE COMPLETE";
            return result;
        }, 0, 7000); // Таймаут 7 сек.
    } catch (const CoreException &ex) {
        qDebug() << ex.message();
    }
*/

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    try {
        m_pCore->addTask(Tasks::TASK_UPDATE);
    } catch (const CoreException &ex) {
        qDebug() << ex.message();
    }
}
