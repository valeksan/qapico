#include "parser.h"

Parser::Parser(QObject *parent) : QObject(parent)
{

}

void Parser::parse(QString page, int stage)
{
    QList<QHash<QString,QVariant> > results;

    if(stage == PARSE_MAIN_PAGE) {
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
}
