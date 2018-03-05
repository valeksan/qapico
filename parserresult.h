#ifndef PARSERRESULT_H
#define PARSERRESULT_H

#include <QList>
#include <QHash>

class ParserResult
{

public:
    ParserResult(int type = -1, int err = -1);

    QHash<QString, QVariant> values;
    int type;
    int error;

    void clear() {
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
