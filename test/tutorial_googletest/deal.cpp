#include "deal.hpp"


// // きっとバグだらけの deal
// int deal(Account* acc, std::queue<int>& amounts) {
//   int count = 0; // 成功した取引数
//   // 取引がなくなるまで
//   while ( !amounts.empty() ) {
//     // 取引を一つ取り出し
//     int amount = amounts.front();
//     amounts.pop();
//     // 取引額の正負に応じて預入/払出
//     bool success = (amount > 0) ? acc->deposit(amount) : acc->withdraw(amount);
//     if ( success ) { // 成功したらcount-up
//       ++count;
//     } else {
//       break; // 失敗したらそこで中断
//     }
//   }
//   return count; // 成功数を返す
// }


// 正しく動作するdeal
int deal(Account* acc, std::queue<int>& amounts) {
  int count = 0;
  bool success = true;
  while ( !amounts.empty() && success ) {
    int amount = amounts.front();
    if ( amount != 0 ) {
      success = (amount > 0) ? acc->deposit(amount) : acc->withdraw(-amount);
    }
    if ( success ) {
      amounts.pop();
      ++count;
    }
  }
  return count;
}