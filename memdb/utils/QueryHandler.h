#pragma once
#include <vector>

#include "Token.h"

class QueryHandler {
public:
    virtual ~QueryHandler() noexcept;

    virtual void Parse() = 0;
    virtual void Evaluate() = 0;
};
