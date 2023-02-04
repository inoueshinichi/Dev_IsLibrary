#pragma once

#include <IsComputerVision/common.hpp>

/*ポリシークラス*/
#include <IsComputerVision/io/format/bmp_policy.hpp>
#include <IsComputerVision/io/format/png_policy.hpp>
#include <IsComputerVision/io/format/jpg_policy.hpp>

namespace is
{
    namespace cv
    {
        using std::string;
        using std::shared_ptr;

        template <class ImageFile>
        class ImageIo
        {
            // ポリシークラス
            ImageFile format_policy_;

        public:
            ImageIo() = default;
            ~ImageIo() = default;
            ImageIo(ImageIo&&) = default;
            ImageIo& operator=(ImageIo&&) = default;

            void save(const string& filename, nbla::NdArrayPtr ndarray, bool is_dump = false)
            {
                using namespace nbla;
                IS_DEBUG_NDARRAY_SHAPE_AS_IMAGE(ndarray)

                Shape_t sh = ndarray->shape(); // (H,W,C)
                Size_t ndim = ndarray->ndim();

                int height = sh.at(0);
                int width = sh.at(1);
                int channels = sh.at(2);

                const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();
                ubyte* data = ndarray->cast_data_and_get_pointer<ubyte>(ctx);
                
                // 画像データをセット
                format_policy_.set(data, height, width, channels);

                // 画像ファイルに書き出し
                format_policy_.save(filename, is_dump);
            }

            nbla::NdArrayPtr load(const string& filename, bool is_dump = false)
            {
                using namespace nbla;
                int width = 0;
                int height = 0;
                int channels = 0;

                // 画像ファイルを読み込む
                format_policy_.load(filename, height, width, channels, is_dump);

                const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();

                // (H,W,C)
                auto ndarray = zeros<ubyte>(Shape_t{height, width, channels});
                auto st = ndarray->strides();
                ubyte* data = ndarray->cast_data_and_get_pointer<ubyte>(ctx);

                // NdArrayにコピー
                format_policy_.copy(data);
                
                return ndarray;
            }

            IS_DISABLE_COPY_AND_ASSIGN(ImageIo)
        };
    } // namespace imgproc
}