#ifndef PARSERRESULT_H
#define PARSERRESULT_H

#include <QList>
#include <QHash>

class ParserResult
{

public:
    ParserResult();
    ParserResult(int err);

    QHash<QString, QVariant> values;
    QString key;
    int type;
    int error;

    void clear() {
        key.clear();
        type = -1;
        error = -1;
        values.clear();
    }

    bool empty() {
        return values.empty();
    }

};
Q_DECLARE_METATYPE(ParserResult)

#endif // PARSERRESULT_H
