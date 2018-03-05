#include "parserresult.h"

ParserResult::ParserResult(int type, int err)
{
    this->error = err;
    this->type = -1;
}
