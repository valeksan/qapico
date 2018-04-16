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
    ParserResult result(Parser::TYPE_PARSE_MAIN_PAGE);
    QJsonDocument document = QJsonDocument::fromJson(m_document);    
    QHash<QString,QVariant> currencies_result;
    QHash<QString,QVariant> currencies_sub_urls_result;
    if(!(document.isEmpty() || document.isNull()) && document.isArray()) {
        QJsonArray currencies = document.array();
        for(int i = 0; i < currencies.size(); i++) {
            QJsonObject currency = currencies.at(i).toObject();
            QString id = currency.value("id").toString();
            QHash<int,QVariant> pars;
            QUrl url_subpage("https://coinmarketcap.com");
            url_subpage.setPath(QString("/currencies/")+id+"/");
            pars.insert(KEY_CYR_NAME_ATTR, currency.value("name").toString());
            pars.insert(KEY_CYR_CYMBOL_ATTR, currency.value("symbol").toString());
            pars.insert(KEY_CYR_RANK_ATTR, currency.value("rank").toString());
            pars.insert(KEY_CYR_PRICE_USD_ATTR, currency.value("price_usd").toString());
            pars.insert(KEY_CYR_PRICE_BTC_ATTR, currency.value("price_btc").toString());
            pars.insert(KEY_CYR_24H_VOLUME_USD_ATTR, currency.value("24h_volume_usd").toString());
            pars.insert(KEY_CYR_MARKETCAP_USD_ATTR, currency.value("market_cap_usd").toString());
            pars.insert(KEY_CYR_AVAIBLE_SUPPLY_ATTR, currency.value("available_supply").toString());
            pars.insert(KEY_CYR_TOTAL_SUPPLY_ATTR, currency.value("total_supply").toString());
            pars.insert(KEY_CYR_MAX_SUPPLY_ATTR, currency.value("max_supply").toString());
            pars.insert(KEY_CYR_PERCENT_CH_1H_ATTR, currency.value("percent_change_1h").toString());
            pars.insert(KEY_CYR_PERCENT_CH_24H_ATTR, currency.value("percent_change_24h").toString());
            pars.insert(KEY_CYR_PERCENT_CH_7D_ATTR, currency.value("percent_change_7d").toString());
            pars.insert(KEY_CYR_DATE_LAST_UPDATED_ATTR, currency.value("last_updated").toString());
            currencies_result.insert(id, QVariant::fromValue(pars));
            currencies_sub_urls_result.insert(id, QVariant::fromValue(url_subpage));
        }
        result.values.insert(KEY_MAIN_TABLE_CURRENCIES, QVariant::fromValue(currencies_result));
        result.values.insert(KEY_MAIN_TABLE_CURRENCIES_INFO_URLS, QVariant::fromValue(currencies_sub_urls_result));
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
    bool nodeFound = false;
    QList<QString> listWebsiteValues;
    QList<QString> listExplorerValues;
    QList<QString> listMessageBoardValues;
    QList<QString> listChatValues;
    QList<QString> listAnnouncementValues;
    QList<QString> listSourceCodeValues;
    QList<QString> listTagsValues;

    try {
        currentNode = currentNode.getElementsByTagName(HtmlTag::BODY).front();
        qDebug() << "classes: " << currentNode.classList();
    } catch(const std::out_of_range& oor) {
        qDebug() << "Body tag not found!(body) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    try {
        currentNodes = currentNode.getElementsByTagName(HtmlTag::UL);
        for(uint i = 0; i < currentNodes.size(); i++) {
//            qDebug() << "UL[" << i << "]{classes:" << currentNodes.at(i).classList() << ";id:" << currentNodes.at(i).id() << "}";
            if(currentNodes.at(i).classList().contains("list-unstyled")) {
                currentNode = currentNodes.at(i);
                nodeFound = true;
                break;
            }
        }
        if(!nodeFound) {
            result.error = Parser::ERR_BAD_SYNTAX;
            return result;
        }
    } catch(const std::out_of_range& oor) {
        qDebug() << "Div tag not found!(ul) " << "Out of Range error: " << oor.what();
        result.error = Parser::ERR_BAD_SYNTAX;
        return result;
    }
    currentNodes = currentNode.getElementsByTagName(HtmlTag::LI);
    for(uint i = 0; i < currentNodes.size(); i++) {
        QGumboNode li_node = currentNodes.at(i);
        try {
            QGumboNodes span_tags = li_node.getElementsByTagName(HtmlTag::SPAN);
            QString currentTitle = span_tags.front().getAttribute("title");
            if(currentTitle == "Website") {
                QString url = li_node.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listWebsiteValues.append(url);
                //qDebug() << "Website:" << url;
            } else if(currentTitle == "Announcement") {
                QString url = li_node.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listAnnouncementValues.append(url);
            } else if(currentTitle == "Chat") {
                QString url = li_node.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listChatValues.append(url);
            } else if(currentTitle == "Explorer") {
                QString url = li_node.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listExplorerValues.append(url);
            } else if(currentTitle == "Message Board") {
                QString url = li_node.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listMessageBoardValues.append(url);
            } else if(currentTitle == "Source Code") {
                QString url = li_node.getElementsByTagName(HtmlTag::A).front().getAttribute("href");
                listSourceCodeValues.append(url);
            } else if(currentTitle == "Tags") {
                for(uint j = 1; j < span_tags.size(); j++) {
                    QString tagName = span_tags.at(j).innerText();
                    listTagsValues.append(tagName);
                }
            }
        } catch(const std::out_of_range& oor) {
            Q_UNUSED(oor)
            continue;
        }
    }
    result.values.insert(KEY_INF_LIST_WEBSITE_URLS, QVariant::fromValue(listWebsiteValues));
    result.values.insert(KEY_INF_LIST_EXPLORER_URLS, QVariant::fromValue(listExplorerValues));
    result.values.insert(KEY_INF_LIST_MSG_BOARD_URLS, QVariant::fromValue(listMessageBoardValues));
    result.values.insert(KEY_INF_LIST_CHAT_URLS, QVariant::fromValue(listChatValues));
    result.values.insert(KEY_INF_LIST_ANNOUNCEMENT_URLS, QVariant::fromValue(listAnnouncementValues));
    result.values.insert(KEY_INF_LIST_SOURCECODE_URLS, QVariant::fromValue(listSourceCodeValues));
    result.values.insert(KEY_INF_LIST_TAGS, QVariant::fromValue(listTagsValues));
    result.error = Parser::ERR_OK;
    /*
    qDebug() << "listWebsiteValues:"    << listWebsiteValues;
    qDebug() << "listExplorerValues:"   << listExplorerValues;
    qDebug() << "listMessageBoardValues:" << listMessageBoardValues;
    qDebug() << "listChatValues"        << listChatValues;
    qDebug() << "listAnnouncementValues" << listAnnouncementValues;
    qDebug() << "listSourceCodeValues"  << listSourceCodeValues;
    qDebug() << "listTagsValues"        << listTagsValues;
    */
    return result;
}

ParserResult Parser::_parseGithubPage()
{
    ParserResult result(Parser::TYPE_PARSE_GITHUB_PAGE);
    QHash<QString,QVariant> table;
    QJsonDocument document = QJsonDocument::fromJson(m_document);
    if(!(document.isEmpty() || document.isNull()) && document.isArray()) {
        QJsonArray projects = document.array();
        for(int i = 0; i < projects.size(); i++) {
            QJsonObject projectObj = projects.at(i).toObject();
            QHash<int,QVariant> pars;
            QString nameProject = projectObj.value("name").toString();
            QJsonObject licenseObj = projectObj.value("license").toObject();
            pars.insert(KEY_GHB_CREATED_AT, projectObj.value("created_at").toString());
            pars.insert(KEY_GHB_UPDATED_AT, projectObj.value("updated_at").toString());
            pars.insert(KEY_GHB_PUSHED_AT, projectObj.value("pushed_at").toString());
            pars.insert(KEY_GHB_CLONE_URL, projectObj.value("clone_url").toString());
            pars.insert(KEY_GHB_HOMEPAGE, projectObj.value("homepage").toString());
            pars.insert(KEY_GHB_LANGUAGE, projectObj.value("language").toString());
            pars.insert(KEY_GHB_LICENSE_NAME, licenseObj.value("name").toString());
            pars.insert(KEY_GHB_LICENSE_URL, licenseObj.value("url").toString());
            pars.insert(KEY_GHB_IS_FORK, projectObj.value("fork").toBool());
            pars.insert(KEY_GHB_SIZE, projectObj.value("size").toString());
            pars.insert(KEY_GHB_STARS_CNT, projectObj.value("stargazers_count").toString());
            pars.insert(KEY_GHB_WATCHERS_CNT, projectObj.value("watchers_count").toString());
            pars.insert(KEY_GHB_FORKS_CNT, projectObj.value("forks_count").toString());
            pars.insert(KEY_GHB_OPEN_ISSUES_CNT, projectObj.value("open_issues_count").toString());
            pars.insert(KEY_GHB_DESCRIPTION, projectObj.value("description").toString());
            table.insert(nameProject,QVariant::fromValue(pars));
            //result.values.insert(nameProject,QVariant::fromValue(pars));
        }
        if(!table.isEmpty()) result.values.insert(KEY_GHB_TABLE_PROJECTS, QVariant::fromValue(table));
        else result.error = Parser::ERR_NO_DATA_FOUND;
    } else {
        if(!m_document.isEmpty())
            result.error = Parser::ERR_BAD_SYNTAX;
        else
            result.error = Parser::ERR_NO_DATA_FOUND;
    }
    return result;
}
