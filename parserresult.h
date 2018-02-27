#ifndef PARSERRESULT_H
#define PARSERRESULT_H

#include <QList>
#include <QHash>

class ParserResult
{

public:
    explicit ParserResult();

    QHash<QString,QHash<QString,QVariant> > content;

};
Q_DECLARE_METATYPE(ParserResult)

#endif // PARSERRESULT_H
