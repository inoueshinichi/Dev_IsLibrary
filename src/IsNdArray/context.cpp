#include <IsNdArray/common.hpp>
#include <IsNdArray/context.hpp>
#include <IsNdArray/exception.hpp>

namespace is
{
    namespace nbla
    {
        Context::Context(const vector<string>& backend, const string& array_class, 
                         const string& device_id)
			: array_class(array_class), device_id(device_id)
		{
			this->set_backend(backend);
		}


        // 計算のバックエンド([cpu|gpu], [float, double, byte,...])を設定する.
        Context& Context::set_backend(const vector<string>& backend) 
		{
			this->backend = backend;
			for (auto iter = this->backend.begin(); iter != this->backend.end(); ++iter)
			{
				if (iter->find(":") == string::npos)
				{
					// デフォルト配列型のfloatを設定
					*iter = *iter + std::string(":float");
				}
			}
			return *this;
		}


        // 配列クラス(CpuArray, CachedCpuArray, CudaArray)を設定する.
        Context& Context::set_array_class(const string& array_class)
		{
			this->array_class = array_class;
			return *this;
		}


        // デバイスIDを設定する.
        Context& Context::set_device_id(const string& device_id)
		{
			this->device_id = device_id;
			return *this;
		}


        // 現在の状態を文字列で出力する.
        string Context::to_string() const
		{
			string b = "[" + string_join(backend, ", ") + "]";
			return format_string("Context(%s, %s, %s)", b.c_str(), array_class.c_str(), device_id.c_str());
		}
    }
}