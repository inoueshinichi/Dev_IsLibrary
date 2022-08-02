#include <gtest/gtest.h>
#include <queue>

#include "deal.hpp"
#include "mock_account.hpp" // Accountのニセモノ: MockAccount


/*
https://codezine.jp/article/detail/9560?p=2
gmockが提供するマクロ：EXPECT_CALLを使ってMockAccountにふるまいを仕込んでいます。
EXPECT_CALL(acc, deposit(_)) は"accに対しdeposit(_)がcallされることを期待する"ことを、
deposit(_)の_はワイルドカード："引数の値は何でも構わない"を意味します。
そのcallがTimes(AtLeast(0))すなわち"少なくとも0回"行われること、
そしてそのときWillRepeatedly(Return(true))："くりかえしtrueをreturnせよ"ってことです。
EXPECT_CALL(acc, withdraw(_))……も同様。
*/

/*
gmockはテストに必要なハリボテを作ってそのふるまい（戻り値）を仕込んでおくだけでなく、
それが期待する回数だけcallされることや、正しい引数でcallされることのテストもやれるわけ。
*/

namespace 
{
    // Accountクラスが実装されていた想定のテストケース
    // TEST(case01, test00)
    // {
    //     Account acc;
    //     std::queue<int> amounts;
    //     // amountに取引額を設定 正:預入/負:引出
    //     amounts.push(2); amounts.push(-1); amounts.push(3);

    //     int expected = amounts.size();
    //     int result = deal(&acc, amounts);
    //     EXPECT_EQ(expected, result + amounts.size()); // 結果の検証
    // }

    /**
     * @brief 以下のテストケースは未実装のAccountクラスの代わりにMockAccountクラスが代行する.
     * 
     */

    // Account::deposit/withdrawがいつもtrueを返すなら、全取引が成功する
    TEST(case01, test01)
    {
        MockAccount acc; // Accountの代役:MockAccount
        std::queue<int> amounts;
        using namespace testing;
        amounts.push(2); amounts.push(-1); amounts.push(3);

        EXPECT_CALL(acc, deposit(_))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(true));
        EXPECT_CALL(acc, withdraw(_))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(true));
        
        int expected = amounts.size();
        int result = deal(&acc, amounts);
        EXPECT_EQ(expected, result);
        EXPECT_EQ(expected, result + amounts.size());
    }

    // Account::deposit/withdrawがいつもfalseを返すなら、0件の取引が成功する
    TEST(case01,test02) 
    {
        MockAccount acc;
        std::queue<int> amounts;
        using namespace testing;
        amounts.push(2); amounts.push(-1); amounts.push(3);

        EXPECT_CALL(acc, deposit(_))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(false));
        EXPECT_CALL(acc, withdraw(_))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(false));

        int expected = amounts.size();
        int result = deal(&acc, amounts);
        EXPECT_EQ(0, result);
        EXPECT_EQ(expected, result + amounts.size());
    }

    // queue内の取引額が0のとき、Account::deposit/withdrawがcallされてはならない
    TEST(case01,test03) 
    {
        MockAccount acc;
        std::queue<int> amounts;
        using namespace testing;
        amounts.push(0); amounts.push(0); amounts.push(0);

        ON_CALL(acc, deposit(_) ).WillByDefault(Return(true));
        ON_CALL(acc, withdraw(_)).WillByDefault(Return(true));

        EXPECT_CALL(acc, deposit(_))
            .Times(0);
        EXPECT_CALL(acc, withdraw(_))
            .Times(0);

        int expected = amounts.size();
        int result = deal(&acc, amounts);
        ASSERT_EQ(expected, result + amounts.size());
    }


    // Account::deposit/withdrawに引き渡される取引額は0より大きくなくてはならない
    TEST(case01,test04) 
    {
        MockAccount acc;
        std::queue<int> amounts;
        using namespace testing;
        amounts.push(100); amounts.push(-200); amounts.push(0);

        EXPECT_CALL(acc, deposit(Gt(0)))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(true));
        EXPECT_CALL(acc, withdraw(Gt(0)))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(true));

        int expected = amounts.size();
        int result = deal(&acc, amounts);
        ASSERT_EQ(expected, result);
        ASSERT_EQ(expected, result + amounts.size());
    }


    // Account::deposit/withdrawが失敗するより以前の成功数が戻り値となる
    TEST(case01,test05)
    {
        MockAccount acc;
        std::queue<int> amounts;
        using namespace testing;
        amounts.push(100); amounts.push(100); amounts.push(100);

        EXPECT_CALL(acc, deposit(_))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillRepeatedly(Return(false));
        EXPECT_CALL(acc, withdraw(_))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(true));

        int expected = amounts.size();
        int result = deal(&acc, amounts);
        ASSERT_EQ(2, result);
        ASSERT_EQ(expected, result + amounts.size());
    }

    TEST(case01,test06) 
    {
        MockAccount acc;
        std::queue<int> amounts;
        using namespace testing;
        amounts.push(-100); amounts.push(-100); amounts.push(-100);

        EXPECT_CALL(acc, deposit(_))
            .Times(AtLeast(0))
            .WillRepeatedly(Return(true));
        EXPECT_CALL(acc, withdraw(_))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillRepeatedly(Return(false));

        int expected = amounts.size();
        int result = deal(&acc, amounts);
        ASSERT_EQ(2, result);
        ASSERT_EQ(expected, result + amounts.size());
    }

}

#include <iostream>

int main(int, char**)
{
    std::cout << "Hello test_gtest" << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}