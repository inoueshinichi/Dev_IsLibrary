#pragma once

#include "account.hpp"
#include <queue>

/*
https://codezine.jp/article/detail/9560
一連の取引を行う関数：dealは引数：amountsから取引額を一つずつ取り出し、
それが正なら預入／負なら引出をaccに対して行い、
取引が成功すればamountsから取り除きます（取引額が0ならなにもせず取引成功扱いとします）。
途中一度でも取引に失敗したら、それ以降の取引を行いません。
dealは成功した取引の回数を返します。
従って例えば5つの取引がdealに渡されたとき、
全取引が成功すればamountsは空になりdealは5を返しますし、
最初の取引が失敗すればamountsはそのままで0が返ることになります。

各取引の成功／失敗にかかわらず、「deal前のqueueの長さ＝dealの戻り値＋deal後のqueueの長さ」となるはず。
*/

int deal(Account* acc, std::queue<int>& amounts);