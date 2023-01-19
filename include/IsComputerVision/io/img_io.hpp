#pragma once

#include <IsComputerVision/IsComputerVision.hpp>

/*ポリシークラス*/
#include <IsComputerVision/io/bmp_policy.hpp>
#include <IsComputerVision/io/png_policy.hpp>
#include <IsComputerVision/io/jpg_policy.hpp>

namespace is
{
    namespace imgproc
    {
        using std::string;
        using std::shared_ptr;

        template <class ImageFile>
        class ImageIo
        {
            ImageFile format_policy_;

            IS_DISABLE_COPY_AND_ASSIGN(ImageIo)
        public:
            ImageIo() = default;
            ~ImageIo() = default;
            ImageIo(ImageIo&&) = default;
            ImageIo& operator=(ImageIo&&) = default;

            void save(const string& filename, nbla::NdArrayPtr ndarray, bool is_dump = false)
            {
                using uchar = unsigned char;
                using namespace nbla;
                if (!ndarray) 
                {
                    std::cerr << "NdArray is nullptr." << std::endl;
                    return;
                }

                int32_t width = 0;
                int32_t height = 0;
                int32_t channels = 0;
                
                Shape_t sh = ndarray->shape(); // (H,W,C)
                Size_t ndim = ndarray->ndim();

                if (ndim == 2) 
                {
                    height = sh.at(0);
                    width = sh.at(1);
                    channels = 1;
                    ndarray->reshape(Shape_t{height, width, channels});
                }
                else if (ndim == 3) 
                {
                    height = sh.at(0);
                    width = sh.at(1);
                    channels = sh.at(2);
                } 
                else
                {
                    throw std::runtime_error(common::format_string(
                        "Unmatch shape of ndarray for bitmap file format. Given is %d", ndarray->ndim()));
                }
                
                // GlobalContext
                const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();

                uchar* data = ndarray->cast_data_and_get_pointer<uchar>(ctx);
                
                // 画像データをセット
                format_policy_.set(data, height, width, channels);

                // 画像ファイルに書き出し
                format_policy_.save(filename, is_dump);
            }

            nbla::NdArrayPtr load(const string& filename, bool is_dump = false)
            {
                using uchar = unsigned char;
                using namespace nbla;
                int width = 0;
                int height = 0;
                int channels = 0;

                // 画像ファイルを読み込む
                if (!format_policy_.load(filename, height, width, channels, is_dump))
                {
                    std::cerr << "ndarray nullptr" << std::endl;
                    return std::make_shared<NdArray>(nullptr);
                }

                // GlobalContext
                const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();

                // (h, w, c)
                auto ndarray = zeros<uchar>(Shape_t{height, width, channels});
                auto st = ndarray->strides();
                uchar* data = ndarray->cast_data_and_get_pointer<uchar>(ctx);

                // NdArrayにコピー
                format_policy_.copy(data);
                
                return ndarray;
            }
        };
    } // namespace imgproc
}