#pragma once

#include <IsNdArray/IsNdArray.hpp>

#include <string>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::string;
		using std::vector;

        class NBLA_API Context
		{
		public:
			/**
			 * Function/SolverまたはNdArrayクラスに渡される計算バックエンド記述子.
			 *
			 * params[in]
			 * backend : 計算バックエンド記述子の配列. Function/Solverの特定の実装は各実装によって照会される.
			 * 　　　　　 最初に一致したものが使用される. 各要素について，計算のバックエンドとデータ型configを
			 *           <backend>:<data typeconfig>の形式で記述する.
			 *           バックエンドのみが指定されている場合(<data typeconfig>は省略), デフォルトのデータ型
			 * 　　　　　 構成である<float>が自動的に追加される.
			 *
			 * array_class オプション : 優先される配列の文字表現.
			 *                         指定されていない場合でも, Function/Solverの実装のデフォルト配列クラスに
			 *                         従って, 配列クラスは選択される.
			 *                         すべてのFunction/Solverクラスには計算の入力と出力のストレージとして使用
			 *                         できる配列クラスのリストがある.
			 *                         指定されたarray_classがそれらのいずれにも一致しない場合，実装のデフォルト
			 *                         の配列クラスが使用される.
			 *
			 * device_id : バックエンドのデバイスIDを表現する文字表現
			 */
			explicit Context(const vector<string>& backend = { "cpu:float" },
						     const string& array_class = "CpuArray",
							 const string& device_id = "0");

			vector<string> backend;
			string array_class;
			string device_id;
			Context& set_backend(const vector<string>& backend);
			Context& set_array_class(const string& array_class);
			Context& set_device_id(const string& device_id);
			string to_string() const;
		};


        /**
         * @brief Get the array key from context object
         * 
         * @param ctx 
         * @return string 
         */
        inline string get_array_key_from_context(const Context& ctx)
		{
			return ctx.device_id + ":" + ctx.array_class;
		}
    }
}