#include "parser.h"

Parser::Parser(QByteArray document, int type, QObject *parent) : QObject(parent),
    m_document(document), m_type(type)
{
    ;
}

ParserResult Parser::parse()
{
    switch (m_type) {
    case Parser::TYPE_PARSE_MAIN_PAGE:
        return _parseMainPage();

    case Parser::TYPE_PARSE_SUB_PAGE:
        return _parseSubPage();

    case Parser::TYPE_PARSE_GITHUB_PAGE:
        return _parseGithubPage();

    default:
        return ParserResult(Parser::ERR_UNKNOW_TYPE);
    }
}

void Parser::setDocument(const QByteArray &document)
{
    m_document = document;
}

int Parser::type() const
{
    return m_type;
}

void Parser::setType(int type)
{
    m_type = type;
}

ParserResult Parser::_parseMainPage()
{
    QJsonDocument document = QJsonDocument::fromJson(m_document);
    ParserResult result;
    QHash<QString,QVariant> currencies_result;
    QHash<QString,QUrl> currencies_sub_urls_result;
    result.type = Parser::TYPE_PARSE_MAIN_PAGE;
    result.key = "";
    if(!(document.isEmpty() || document.isNull()) && document.isArray()) {
        QJsonArray currencies = document.array();
        for(int i = 0; i < currencies.size(); i++) {
            QJsonObject currency = currencies.at(i).toObject();
            QString id = currency.value("id").toString();
            QHash<QString,QVariant> pars;
            QUrl url_subpage("https://coinmarketcap.com");
            url_subpage.setPath(QString("/")+id);
            pars.insert("name", currency.value("name").toString());
            pars.insert("symbol", currency.value("symbol").toString());
            pars.insert("rank", currency.value("rank").toString());
            pars.insert("price_usd", currency.value("price_usd").toString());
            pars.insert("price_btc", currency.value("price_btc").toString());
            pars.insert("24h_volume_usd", currency.value("24h_volume_usd").toString());
            pars.insert("market_cap_usd", currency.value("market_cap_usd").toString());
            pars.insert("available_supply", currency.value("available_supply").toString());
            pars.insert("total_supply", currency.value("total_supply").toString());
            pars.insert("max_supply", currency.value("max_supply").toString());
            pars.insert("percent_change_1h", currency.value("percent_change_1h").toString());
            pars.insert("percent_change_24h", currency.value("percent_change_24h").toString());
            pars.insert("percent_change_7d", currency.value("percent_change_7d").toString());
            pars.insert("last_updated", currency.value("last_updated").toString());
            currencies_result.insert(id, pars);
            currencies_sub_urls_result.insert(id, url_subpage);
        }
        result.values.insert("currencies", QVariant::fromValue(currencies_result));
        result.values.insert("currencies_sub_urls", QVariant::fromValue(currencies_sub_urls_result));
        result.error = Parser::ERR_OK;

    } else {
        if(!m_document.isEmpty())
            result.error = Parser::ERR_BAD_SYNTAX;
        else
            result.error = Parser::ERR_NO_DATA_FOUND;
    }
    return result;
}

ParserResult Parser::_parseSubPage()
{
    ;
}

ParserResult Parser::_parseGithubPage()
{
    ;
}
