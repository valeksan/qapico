#include "parserresult.h"

ParserResult::ParserResult()
{
    type = -1;
    key = "";
    error = -1;
}

ParserResult::ParserResult(int err)
{
    this->error = err;
    type = -1;
    key = "";
}
