#pragma once
#include "Token.h"
#include <vector>

class QueryHandler {
public:
    virtual ~QueryHandler() noexcept;

    virtual void Parse() = 0;
    virtual void Evaluate() = 0;
};
