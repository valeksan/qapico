#include "parser.h"

Parser::Parser(int maxDownloadThreads, QObject *parent) : QObject(parent),
    m_pCore(new Core()),
    m_pNetworkAccessManager(new QNetworkAccessManager())
{
    if(!isAppPathExists()) {
        qCritical() << "Missing application directory, and could not create it! (Access)";
        return;
    }

    m_stage = STAGE_DOWNLOAD_MAIN_PAGE;
    m_pResult = nullptr;

}

void Parser::registerAllTasks()
{
    m_pCore->registerTask(STAGE_PARSE_MAIN_PAGE, [=,&m_pNetworkAccessManager](const QByteArray& page) {

        if(m_pResult != nullptr)
            delete m_pResult;

        m_pResult = new ParserResult();

        int downloadTypeD = 0;
        int errorD = false;
        QVariantList argsD;
        Downloader downloader(m_pNetworkAccessManager)

        connect(&m_pageDownloader, &Downloader::complete, [&m_pResult, &downloadTypeD, &errorD, &argsD](QVariant data, int downloadType, int error, QVariantList args) {
            downloadTypeD = downloadType;
            errorD = error;
            argsD = args;
            // формирование списка (парсинг)
            auto doc = QGumboDocument::parse(data.toString().toLatin1().data());
            auto root = doc.rootNode();
            auto tbody = root.getElementsByTagName(HtmlTag::TBODY).front();
            auto currency_nodes = tbody.getElementsByTagName(HtmlTag::TR);
            for(int index_node = 0; index_node < currency_nodes.size(); index_node++) {
                auto table_nodes = currency_nodes.at(index_node).getElementsByTagName(HtmlTag::TD);
                QString name;

                // TD(1): get curency name
                auto currency_td1_a_tags = table_nodes.at(1).getElementsByTagName(HtmlTag::SPAN).front().getElementsByTagName(HtmlTag::A);
                if(!currency_td1_a_tags.empty()) {
                    auto currency_td1_a_tag = currency_td1_a_tags.front();
                    QString currency_href = currency_td1_a_tag.getAttribute("href");
                    QString url = QString("https://coinmarketcap.com%1").arg(currency_href);
                    QString symbol = currency_td1_a_tag.innerText();
                    name = currency_href.section("/", 2, 2);
                    m_pResult->content.value(name).insert("name", name);
                    m_pResult->content.value(name).insert("url", url);
                    m_pResult->content.value(name).insert("symbol", symbol);
                    qDebug() << name << " : " << symbol;
                } else {
                    qDebug() << "Bad section_1 ! " << "index:" << index_node;
                    continue;
                }

                // TD(4): get currency prices
                auto currency_td4_a_tags = table_nodes.at(4).getElementsByTagName(HtmlTag::A);
                if(!currency_td4_a_tags.empty()) {
                    auto currency_td4_a_tag = currency_td4_a_tags.front();
                    double price_usd = currency_td4_a_tag.getAttribute("data-usd").toDouble();
                    double price_btc = currency_td4_a_tag.getAttribute("data-btc").toDouble();
                    m_pResult->content.value(name).insert("price_usd", price_usd);
                    m_pResult->content.value(name).insert("price_btc", price_btc);
                    qDebug() << "price_usd:" << price_usd << " price_btc:" << price_btc;
                } else {
                    m_pResult->content.remove(name);
                    qDebug() << "Bad section_2 ! " << "index:" << index_node;
                    continue;
                }

                // TD(5): get supply and mineable flag
                bool is_mineable = !table_nodes.at(5).innerText().contains("*");
                auto nodes_supply_a_tags = table_nodes.at(5).getElementsByTagName(HtmlTag::A);
                auto nodes_supply_span_tags = (nodes_supply_a_tags.empty() ? table_nodes.at(5).getElementsByTagName(HtmlTag::SPAN) : QGumboNodes());
                double calculating_supply = 0.0;
                if(!nodes_supply_a_tags.empty()) {
                    calculating_supply = static_cast<unsigned long long>(nodes_supply_a_tags.front().getAttribute("data-supply").toDouble());
                } else if(!nodes_supply_span_tags.empty()) {
                    calculating_supply = static_cast<unsigned long long>(nodes_supply_span_tags.front().getAttribute("data-supply").toDouble());
                } else {
                    //...
                    qDebug() << "Bad section_3 (not important) ! " << "index:" << index_node;
                }
                m_pResult->content.value(name).insert("calculating_supply", calculating_supply);
                m_pResult->content.value(name).insert("mineable", is_mineable);
                qDebug() << "calculating_supply:" << calculating_supply << " mineable:" << is_mineable;

                // TD(6): get volume
                auto nodes_volume_a_tags = table_nodes.at(6).getElementsByTagName(HtmlTag::A);
                auto nodes_volume_span_tags = (nodes_supply_a_tags.empty() ? table_nodes.at(6).getElementsByTagName(HtmlTag::SPAN) : QGumboNodes());
                double volume_usd = 0.0;
                double volume_btc = 0.0;
                if(!nodes_volume_a_tags.empty()) {
                    volume_usd = nodes_volume_a_tags.front().getAttribute("data-usd").toDouble();
                    volume_btc = nodes_volume_a_tags.front().getAttribute("data-btc").toDouble();
                } else if(!nodes_volume_span_tags.empty()) {
                    volume_usd = nodes_volume_span_tags.front().getAttribute("data-usd").toDouble();
                    volume_btc = nodes_volume_span_tags.front().getAttribute("data-btc").toDouble();
                } else {
                    //...
                    qDebug() << "Bad section_4 (not important) ! " << "index:" << index_node;
                }
                m_pResult->content.value(name).insert("volume_usd", volume_usd);
                m_pResult->content.value(name).insert("volume_btc", volume_btc);
                qDebug() << "volume_usd:" << volume_usd << " volume_btc:" << volume_btc;
            }
        });

        m_pageDownloader.getData(QUrl("https://coinmarketcap.com/all/views/all/"), Downloader::D_TYPE_TEXT);
        if(waitSignal(&downloader, &Downloader::complete, 10000)){
            //...
        }

    }, 0);
}

void Parser::start()
{

}

void Parser::nextStage()
{
    // ...
}


//m_pageDownloader.getData(QUrl("https://coinmarketcap.com/all/views/all/"), Downloader::D_TYPE_TEXT);
/* CODE
QList<QHash<QString,QVariant> > results;

if(m_stage == PARSE_MAIN_PAGE) {
    auto doc = QGumboDocument::parse(page.toLatin1().data());
    auto root = doc.rootNode();
    auto tbody = root.getElementsByTagName(HtmlTag::TBODY).front();
    auto currency_nodes = tbody.getElementsByTagName(HtmlTag::TR);
    for(int index_node = 0; index_node < currency_nodes.size(); index_node++) {

        auto table_nodes = currency_nodes.at(index_node).getElementsByTagName(HtmlTag::TD);

        // TD(1): get curency name
        auto currency_td1_a_tag = table_nodes.at(1).getElementsByTagName(HtmlTag::SPAN).front().getElementsByTagName(HtmlTag::A).front();
        QString currency_href = currency_td1_a_tag.getAttribute("href");
        QUrl currency_url = QUrl(QString("https://coinmarketcap.com%1").arg(currency_href));
        results.value(index_node).insert("url", currency_url);
        QString currency_name = currency_href.section("/", 2, 2);
        currency_name = currency_name.left(1).toUpper()+currency_name.mid(1);
        results.value(index_node).insert("name", currency_name);
        QString symbol_name = currency_td1_a_tag.innerText();
        results.value(index_node).insert("symbol", symbol_name);
        qDebug() << currency_name << " : " << symbol_name;

        // TD(4): get currency prices
        auto currency_td4_a_tag = table_nodes.at(4).getElementsByTagName(HtmlTag::A).front();
        double price_usd = currency_td4_a_tag.getAttribute("data-usd").toDouble();
        results.value(index_node).insert("price_usd", price_usd);
        double price_btc = currency_td4_a_tag.getAttribute("data-btc").toDouble();
        results.value(index_node).insert("price_btc", price_btc);
        qDebug() << "price_usd:" << price_usd << " price_btc:" << price_btc;

        // TD(5): get supply and mineable flag
        auto nodes_supply_a = table_nodes.at(5).getElementsByTagName(HtmlTag::A);
        auto nodes_supply_span = (nodes_supply_a.empty() ? table_nodes.at(5).getElementsByTagName(HtmlTag::SPAN) : QGumboNodes());
        double calculating_supply = 0.0;
        if(!nodes_supply_a.empty()) {
            calculating_supply = static_cast<unsigned long long>(nodes_supply_a.front().getAttribute("data-supply").toDouble());
        } else if(!nodes_supply_span.empty()) {
            calculating_supply = static_cast<unsigned long long>(nodes_supply_span.front().getAttribute("data-supply").toDouble());
        }
        results.value(index_node).insert("calculating_supply", calculating_supply);
        bool is_mineable = !table_nodes.at(5).innerText().contains("*");
        results.value(index_node).insert("is_mineable", is_mineable);
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
        results.value(index_node).insert("volume_usd", volume_usd);
        results.value(index_node).insert("volume_btc", volume_btc);
        qDebug() << "volume_usd:" << volume_usd << " volume_btc:" << volume_btc;


//            m_mutex.lock();
//            m_currenciesPagesForParse.insert(currency_name, currency_url); //.append(QPair<QString,QUrl>(currency_name, currency_url));
//            m_mutex.unlock();
    }
}
*/
