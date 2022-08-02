#pragma once

#include "account.hpp"
#include <gmock/gmock.h>


/**
 * @brief gmockを使ってAccountのハリボテ：MockAccountを作り、Accountの代役を務めさせる.
 * 
 */
class MockAccount : public Account
{
public:
    MOCK_METHOD1(deposit,  bool(int));
    MOCK_METHOD1(withdraw, bool(int));
};


/*
マクロ：
MOCK_METHOD#(関数名, 戻り型(引数型...);
MOCK_CONST_METHOD#(関数名, 戻り型(引数型...);
*/