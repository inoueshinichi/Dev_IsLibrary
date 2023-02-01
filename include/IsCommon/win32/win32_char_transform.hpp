/**
 * @file win32_char_transform.hpp
 * @author your name (you@domain.com)
 * @brief 文字コードの変換
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

/**************************************************************************************************************
[文字コード定義]
Shift_JIS: マルチバイト文字  半角文字: 1byte, 全角文字: 2byte
UTF-16: ワイド文字       基本的に全文字 2byte, 一部の文字が4byte
UTF-8: マルチバイト文字  ASCII文字: 1byte, 多くの日本語:3byte, 一部の文字:4byte, 全世界の言語を含んで最大1文字:6byte
※UTF-8の1byteはANCII文字に対応しているため、何かと便利.
**************************************************************************************************************/

/*
[Windows関係の文字列型]
1) ANSI/UNICODE C/C++言語
2) STL
3) Windows(C/C++)
4) Windows(OLE)
5) Windows(COM, DCOM, ActiveX)
6) Windows(ATL)
7) Windows(CLI)
8) Windows(MFC)
********************************************************************************************************
--------------------------------------------------------------------------------------------------------
1) ANSI/UNICODE C/C++言語
char     // 8bit  (UTF-8, Shift-JIS)   Windows: default:Shift-JIS, custom:UTF-8 (Linux:UTF-8)
char16_t // 16bit (UTF-16=ワイド文字)    C++11非対応
char32_t // 32bit (UTF-32=長ワイド文字)  C++11非対応
wchar_t  // ワイド文字 2Byte(UTF-16=ワイド文字)
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
2) STL
std::basic_string<char_traits> // 統一表現
std::string            // char用マルチバイト文字配列クラス const char *    を内部表現に持つテンプレートクラス
std::wstring           // wchar用ワイド文字配列クラス     const wchar_t * を内部表現に持つテンプレートクラス
--------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
3) Windows(C/C++)
CHAR            = char                          // マルチバイト文字
WCHAR           = wchar_t                       // ワイド文字
TCHAR           = char or WCHAR                 // マルチバイト文字，ワイド文字の統一表記
LPSTR(PSTR)     = char *                        // マルチバイト文字へのポインタ
LPCSTR(PCSTR)   = const char *                  // 読み取り専用マルチバイト文字へのポインタ
LPWSTR(PWSTR)   = WCHAR *                       // ワイド文字へのポインタ
LPWCSTR(PWCSTR) = const WCHAR *                 // 読み取り専用ワイド文字へのポインタ
LPTSTR(PTSTR)   = char * or WCHAR *             // 統一表記へのポインタ
LPCTSTR(PCTSTR) = const char* or const WCHAR*   // 読み取り専用統一表記へのポインタ

無印 -> マルチバイト系(Shift-JIS or utf-8)
W付き -> Unicode(utf-16)　通称ワイド文字
T付き -> マルチバイト文字とUnicode(utf-16)の統一表記(_UNICODEプリプロセッサで切り替え)
#include <tchar.h>ディレクティブでマルチバイト系とワイドバイト系の型特性を操作できる.
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
4) OLE (Object Linking and Embedding)
※ Windowsの機能・仕様の一つで、複数のソフトウェアが連携したり、データを共有したりするための仕組み.
Windowsコアの文字表現は残念ながらutf-16のワイドバイト文字系になる.
(コアに近くなるほどマルチバイト系とワイドバイト系の変換作業が増える)
OLECHAR = WCHAR  // ワイド文字のエイリアス
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
5-1) COM (Component Object Model)
※ OLEの後身, DCOM(Distributed COM)
Microsoft社が提唱していた，ソフトウェアの機能を部品化して，外部から日々出して利用する仕組みを定めた技術仕様.
VC++からExcelやWordなどを操作するときに使う.
BSTR = OLECHAR *  // Visual Basic文字列型
_bstr_t           // BSTRのC++クラスラッパー

5-2) ActiveX
※ OLEの仕組みをインターネットなどの通信ネットワークを通じて利用できるようにしたもの.
一般的に単にActiveXといった場合には、WebサーバからWebブラウザにソフトウェア部品を送信し、
即座に実行することができる「ActiveXコントロール」（ActiveX Control）のことを指すことが多い.
OLECHAR = WCHAR       // Visual Basic文字列型
BSTR    = OLECHAR *   // Visual Basic文字列型へのポインタ
-------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------
6) ATL (Active Template Library)
※ COMプログラミングを容易に行えるように, Microsoft社によるテンプレートベースのC++専用ライブラリ
VC++からExcelやWordなどを操作するときに使う。
CComBSTR  // Visual Basic文字列(BSTR)型用のもう1つのC++クラスラッパー
CString型がマルチバイト系とワイドバイト系の変換を容易にしてくれるのでかなり優秀.
#include <atlstr.h>ディレクティブが必要になる.
------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------
7) CLI (Common Library Interface)
※ .Net Framework
String^  // C++/CLI言語でC++から.Net FrameWorkを利用するための拡張.
※ ^はヒープ領域にメモリを格納する意味.C++/CLI言語ではクラスをガーベッジコレクションで破棄させるようにしている.

※ 2003年以前 -> C++マネージ拡張
※ 2005年以降 -> C++/CLI

8) MFC
CStringA  // char用文字列クラス const char * を内部表現に持つテンプレートクラス
CStringW  // wchar用文字列クラス const wchar_t * を内部表現に持つテンプレートクラス
CString   // 統一表記  const char * or const wchar_t *
CStringT  // MFC CStringのテンプレート テンプレートクラス -> char, wchar_t, char16_t char32_t 等
*************************************************************************************************************
*/

#include <IsCommon/win32/win32_defs.hpp>


#include <string>
#include <iostream>
#include <iomanip> // std::setfill, std::setw

namespace is
{
    namespace common
    {
        namespace win32
        {
            //////////////////////////////////
            /* Convert from UTF-8 to UTF-16 */
            //////////////////////////////////
            inline std::wstring cvt_utf8_to_utf16(char const *utf8_char);
            inline std::wstring cvt_utf8_to_utf16(const std::string &utf8_string);

            //////////////////////////////////
            /* Convert from UTF-16 to UTF-8 */
            //////////////////////////////////
            inline std::string cvt_utf16_to_utf8(const wchar_t *utf16_wchar);
            inline std::string cvt_utf16_to_utf8(const std::wstring &utf16_wstring);
            inline std::string cvt_utf16_to_utf8(const char16_t *utf16_char16);
            inline std::string cvt_utf16_to_utf8(const std::u16string &utf16_string);

            //////////////////////////////////////
            /* Convert from Shift-JIS to UTF-16 */
            //////////////////////////////////////
            inline std::wstring cvt_shiftjis_to_utf16(const char *shift_jis_char);
            inline std::wstring cvt_shiftjis_to_utf16(const std::string &shift_jis_string);

            //////////////////////////////////////
            /* Convert from UTF-16 to Shift-JIS */
            //////////////////////////////////////
            inline std::string cvt_utf16_to_shiftjis(const wchar_t *utf16_wchar);
            inline std::string cvt_utf16_to_shiftjis(const std::wstring &utf16_wstring);
            inline std::string cvt_utf16_to_shiftjis(const char16_t *utf16_char16);
            inline std::string cvt_utf16_to_shiftjis(const std::u16string &utf16_string);

            //////////////////////////////////////
            /* Convert from UTT-8 to Shift-JIS  */
            //////////////////////////////////////
            inline std::string cvt_utf8_to_shiftjis(const char *utf8_char);
            inline std::string cvt_utf8_to_shiftjis(const std::string &utf8_string);

            //////////////////////////////////////
            /* Convert from Shift-JIS to UTF-8  */
            //////////////////////////////////////
            inline std::string cvt_shiftjis_to_utf8(const char *shift_jis_char);
            inline std::string cvt_shiftjis_to_utf8(const std::string &shift_jis_string);

            /*16進数でダンプ*/
            template <typename OS, typename T>
            void dump_by_hex(OS &os, T const *t)
            {
                bool is_first = true;
                const auto *byte = reinterpret_cast<unsigned char const *>(t);
                os << std::hex << std::uppercase << std::setfill('0') << std::setw(2);
                while (*byte)
                {
                    if (!std::exchange(is_first, false))
                    {
                        os << " ";
                    }
                    os << static_cast<unsigned int>(*byte);
                    ++byte;
                }
                os << std::endl;
            }

            /**
             * @brief Windowsで通常の入出力をUTF-8にする場合
             * https://qiita.com/tats-u/items/ef149aee6b69407db79b
             * Windows10専用の手法: setlocale・std::locale::globalなどに空文字のデフォルトロケールを渡す.
             */
            // C兼用
            // #include <locale.h>
            // #include <stdio.h>

            // int main(int argc, char **argv)
            // {
            //     setlocale(LC_ALL, "");
            //     puts("日本語だよ");
            //     for (int i = 0; i < argc; ++i)
            //     {
            //         printf("引数%d: %s\n", i, argv[i]);
            //     }
            //     return 0;
            // }

            // C++ 専用
            // #include <locale>
            // #include <iostream>

            // int main()
            // {
            //     std::locale::global(std::locale(""));
            //     std::cout << "日本語だよ\n";
            //     for (int i = 0; i < argc; ++i)
            //     {
            //         std::cout << "引数" << i << ": " << argv[i] << '\n';
            //     }
            //     return 0;
            // }

            // マニフェストファイル. CMakeのadd_executeble()に一緒に埋め込む.
            // <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
            // <assembly manifestVersion="1.0" xmlns="urn:schemas-microsoft-com:asm.v1">
            // <application>
            //     <windowsSettings>
            //     <activeCodePage xmlns="http://schemas.microsoft.com/SMI/2019/WindowsSettings">UTF-8</activeCodePage>
            //     </windowsSettings>
            // </application>
            // </assembly>
        }
    }
}