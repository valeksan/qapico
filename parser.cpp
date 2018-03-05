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
    ParserResult result(Parser::TYPE_PARSE_MAIN_PAGE);
    QHash<QString,QVariant> currencies_result;
    QHash<QString,QUrl> currencies_sub_urls_result;
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
    ParserResult result(Parser::TYPE_PARSE_SUB_PAGE);
    QGumboDocument doc = QGumboDocument::parse(m_document);
    QGumboNode currentNode = doc.rootNode();
    QGumboNodes currentNodes;
    QList<QString> listWebsiteValues;
    QList<QString> listExplorerValues;
    QList<QString> listMessageBoardValues;
    QList<QString> listChatValues;
    QList<QString> listAnnouncementValues;
    QList<QString> listSourceCodeValues;
    QList<QString> listTagsValues;
    uint hashWebsiteTitle = qHash("Website");
    uint hashExplorerTitle = qHash("Explorer");
    uint hashMessageBoardTitle = qHash("MessageBoard");
    uint hashChatTitle = qHash("Chat");
    uint hashAnnouncementTitle = qHash("Announcement");
    uint hashSourceCodeTitle = qHash("SourceCode");
    uint hashTagsTitle = qHash("Tags");
    uint hashCurrentTitle = 0;

    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::BODY).front();
    } catch(const std::out_of_range& oor) {
        qDebug() << "Body tag not found!(b) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::DIV).at(3);
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(b:d3) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::DIV).front();
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(b:d3:d0) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::DIV).front();
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(b:d3:d0:d0) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::DIV).at(3);
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(b:d3:d0:d0:d3) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::DIV).at(1);
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(b:d3:d0:d0:d3:d1) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::UL).front();
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(b:d3:d0:d0:d3:d1:ul0) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    currentNodes = currentNode.getElementsByTagName(HtmlTag::LI);
    for(int nNode = 0; nNode < currentNodes.size(); nNode++) {
        QGumboNode currentNode = currentNodes.at(nNode);
        QGumboNode span_tag;
        try {
            span_tag = currentNode.getElementsByTagName(HtmlTag::SPAN).front();
            hashCurrentTitle = qHash(span_tag.getAttribute("title"));
            if(hashCurrentTitle == hashWebsiteTitle) {
                QString url = span_tag.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listWebsiteValues.append(url);
            } else if(hashCurrentTitle == hashAnnouncementTitle) {
                QString url = span_tag.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listAnnouncementValues.append(url);
            } else if(hashCurrentTitle == hashChatTitle) {
                QString url = span_tag.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listChatValues.append(url);
            } else if(hashCurrentTitle == hashExplorerTitle) {
                QString url = span_tag.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listExplorerValues.append(url);
            } else if(hashCurrentTitle == hashMessageBoardTitle) {
                QString url = span_tag.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listMessageBoardValues.append(url);
            } else if(hashCurrentTitle == hashSourceCodeTitle) {
                QString url = span_tag.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listSourceCodeValues.append(url);
            } else if(hashCurrentTitle == hashTagsTitle) {
                QGumboNodes tagNodes = currentNode.getElementsByTagName(HtmlTag::SMALL).front().getElementsByTagName(HtmlTag::SPAN);
                for(int nTagNode = 0; nTagNode < tagNodes.size(); nTagNode++) {
                    QString tagName = tagNodes.at(nTagNode).innerText();
                    listTagsValues.append(tagName);
                }
            }
        } catch(const std::out_of_range& oor) {
            continue;
        }
    }
    return result;
}

ParserResult Parser::_parseGithubPage()
{
    ParserResult result;
    //
    return result;
}
