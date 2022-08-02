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
            
        public:
            ImageIo() = default;
            ~ImageIo() = default;
            ImageIo(const ImageIo&) = delete;
            ImageIo& operator=(const ImageIo&) = delete;
            ImageIo(ImageIo&&) = delete;
            ImageIo& operator=(ImageIo&&) = delete;

            bool save(const string& filename, nbla::NdArrayPtr ndarray, bool is_dump = false)
            {
                using uchar = unsigned char;
                using namespace nbla;
                if (!ndarray) 
                {
                    std::cerr << "NdArray is nullptr." << std::endl;
                    return false;
                }
                
                int32_t channels = 0;
                int32_t width = 0;
                int32_t height = 0;
                Shape_t sh = ndarray->shape(); // (C,H,W)
                Size_t ndim = ndarray->ndim();
                if (ndim == 2) 
                {
                    height = sh.at(0);
                    width = sh.at(1);
                    channels = 1;
                    ndarray->reshape(Shape_t{channels, height, width});
                }
                else if (ndim == 3) 
                {
                    channels = sh.at(0);
                    height = sh.at(1);
                    width = sh.at(2);
                } 
                else
                {
                    throw std::runtime_error(common::format_string(
                        "Unmatch shape of ndarray for bitmap file format. Given is %d", ndarray->ndim()));
                }
                
                sh = ndarray->shape();
                auto st = ndarray->strides();
                std::printf("(C,H,W)=(%d,%d,%d)\n", channels, height, width);
                std::printf("strides(%d,%d,%d)\n", st[0], st[1], st[2]);

                // GlobalContext
                const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();

                uchar *data = ndarray->cast_data_and_get_pointer<uchar>(ctx);

                auto tmp_array = zeros<uchar>(Shape_t{height, width, channels});
                auto tmp_sh = tmp_array->shape();
                auto tmp_st = tmp_array->strides();
                uchar *tmp_data = tmp_array->cast_data_and_get_pointer<uchar>(ctx);
                std::printf("tmp_shape(H,W,C)=(%ld,%ld,%ld)\n", tmp_sh[0], tmp_sh[1], tmp_sh[2]);
                std::printf("tmp_strides(%d,%d,%d)\n", tmp_st[0], tmp_st[1], tmp_st[2]);

                for (int c = 0; c < channels; ++c)
                {
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
                        {
                            tmp_data[y * tmp_st[0] + x * tmp_st[1] + c * tmp_st[2]] =
                                data[c * st[0] + y * st[1] + x * st[2]];
                        }
                    }
                }
                
                // 書き込み
                format_policy_.set_data(tmp_data, height, width, channels);
                // format_policy_.set_data(data, height, width, channels);

                // 画像ファイルに書き出し
                return format_policy_.save(filename, is_dump);
            }

            nbla::NdArrayPtr load(const string &filename, bool is_dump = false)
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
                const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();

                auto ndarray = zeros<uchar>(Shape_t{channels, height, width});
                auto st = ndarray->strides();
                uchar *data = ndarray->cast_data_and_get_pointer<uchar>(ctx);

                // (h, w, c)
                auto tmp_array = zeros<uchar>(Shape_t{height, width, channels});
                auto tmp_sh = tmp_array->shape();
                auto tmp_st = tmp_array->strides();
                uchar *tmp_data = tmp_array->cast_data_and_get_pointer<uchar>(ctx);
                std::printf("tmp_shape(H,W,C)=(%ld,%ld,%ld)\n", tmp_sh[0], tmp_sh[1], tmp_sh[2]);
                std::printf("tmp_strides(%d,%d,%d)\n", tmp_st[0], tmp_st[1], tmp_st[2]);

                // NdArrayにコピー
                format_policy_.get_data(tmp_data);

                for (int c = 0; c < channels; ++c)
                {
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < width; ++x)
                        {
                            data[c * st[0] + y * st[1] + x * st[2]] = 
                             tmp_data[y * tmp_st[0] + x * tmp_st[1] + c * tmp_st[2]];
                        }
                    }
                }

                std::printf("channels: %d, height: %d, width: %d\n", channels, height, width);

                return ndarray;
            }
        };
    } // namespace imgproc
}