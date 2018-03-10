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
            QHash<QString,QVariant> pars;
            QUrl url_subpage("https://coinmarketcap.com");
            url_subpage.setPath(QString("/currencies/")+id+"/");
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
            currencies_sub_urls_result.insert(id, QVariant::fromValue(url_subpage));
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
    result.values.insert("WebsiteUrls", QVariant::fromValue(listWebsiteValues));
    result.values.insert("ExplorerUrls", QVariant::fromValue(listExplorerValues));
    result.values.insert("MessageBoardUrls", QVariant::fromValue(listMessageBoardValues));
    result.values.insert("ChatUrls", QVariant::fromValue(listChatValues));
    result.values.insert("AnnouncementUrls", QVariant::fromValue(listAnnouncementValues));
    result.values.insert("SourceCodeUrls", QVariant::fromValue(listSourceCodeValues));
    result.values.insert("Tags", QVariant::fromValue(listTagsValues));
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
    QJsonDocument document = QJsonDocument::fromJson(m_document);
    if(!(document.isEmpty() || document.isNull()) && document.isArray()) {
        QJsonArray projects = document.array();
        for(int i = 0; i < projects.size(); i++) {
            QJsonObject projectObj = projects.at(i).toObject();
            QHash<QString,QVariant> pars;
            QString nameProject = projectObj.value("name").toString();
            QJsonObject licenseObj = projectObj.value("license").toObject();
            pars.insert("created_at", projectObj.value("created_at").toString());
            pars.insert("updated_at", projectObj.value("updated_at").toString());
            pars.insert("pushed_at", projectObj.value("pushed_at").toString());
            pars.insert("clone_url", projectObj.value("clone_url").toString());
            pars.insert("homepage", projectObj.value("homepage").toString());
            pars.insert("language", projectObj.value("language").toString());
            pars.insert("license_name", licenseObj.value("name").toString());
            pars.insert("license_url", licenseObj.value("url").toString());
            pars.insert("is_fork", projectObj.value("fork").toBool());
            pars.insert("size", projectObj.value("size").toString());
            pars.insert("stars_count", projectObj.value("stargazers_count").toString());
            pars.insert("watchers_count", projectObj.value("watchers_count").toString());
            pars.insert("forks_count", projectObj.value("forks_count").toString());
            pars.insert("open_issues_count", projectObj.value("open_issues_count").toString());
            pars.insert("description", projectObj.value("description").toString());
            result.values.insert(nameProject,QVariant::fromValue(pars));
        }
    } else {
        if(!m_document.isEmpty())
            result.error = Parser::ERR_BAD_SYNTAX;
        else
            result.error = Parser::ERR_NO_DATA_FOUND;
    }
    return result;
}
