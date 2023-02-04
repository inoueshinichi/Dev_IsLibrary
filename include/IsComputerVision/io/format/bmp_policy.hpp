#pragma once

#include <IsComputerVision/common.hpp>

#include <cstdint>

namespace is
{
    namespace cv
    {
        using std::string;
        using std::tuple;

        // Windows Bitmapファイルのフォーマットについて
        // https://www.mm2d.net/main/prog/c/image_io-05.html
        // https://github.com/ohmae/image-io
        class IS_CV_API BmpFile
        {
/*BMPファイルヘッダ (14byte)*/
#pragma pack(2) // 構造体のアライメントを2byte境界にすることで、余計な詰め物がない構造体とする. ※重要
            typedef struct /*IMGPROC_ALIGN(2)*/ BmpFileHeader
            {
                uint16 bf_type;        // ファイルタイプ. 必ず"BM" B=0x42、M=0x4D
                uint32 bf_size;        // ファイルサイズ
                uint16 bf_reserved1;   // 予約1
                uint16 bf_reserved2;   // 予約2
                uint32 bf_offset_bits; // ファイル先頭から画像情報までのオフセット
            } BmpFileHeader;

/*BMP情報ヘッダ*/
#pragma pack() // 構造体のアライメントをデフォルトの4byte境界に戻す. ※重要
            typedef struct BmpInfoHeader
            {
                uint32 bi_size;            // この構造体のサイズ(40byte)
                int32 bi_width;            // 画像の幅. 負の値は不正な値
                int32 bi_height;           // 画像の高さ. 値が負の場合はトップダウン画像となる. ただし、この方式は互換性の観点から非推奨とされる.
                uint16 bi_planes;          // 画像の枚数. 常に1
                uint16 bi_bitcount;        // 色深度. 0, 1, 4, 8, 16, 24, 32
                uint32 bi_compression;     // 圧縮形式. 0, 1, 2, 3, 4, 5
                uint32 bi_size_image;      // 画像領域のサイズ
                int32 bi_x_pels_per_meter; // 画像の横方向解像度情報. 解像度を扱わない場合は0でも問題ない
                int32 bi_y_pels_per_meter; // 画像の縦方向解像度情報. 解像度を扱わない場合は0でも問題ない
                uint32 bi_color_used;      // カラーパレットのうち、実際に使用している色の個数
                uint32 bi_clor_important;  // カラーパレットのうち、重要な色の数
            } BmpInfoHeader;

            /*BMPカラーパレット*/
            typedef struct RgbQuad
            {
                ubyte rgb_blue;
                ubyte rgb_green;
                ubyte rgb_red;
                ubyte rgb_reserved;
            } RgbQuad;

            /*BMP情報*/
            typedef struct BmiInfo
            {
                BmpInfoHeader bmi_header;
                RgbQuad bmi_colors[1];
            } BmiInfo;

            BmpFileHeader bmp_file_header_;
            BmiInfo *bmi_info_;
            ubyte *bmp_;

            int32 width_;
            int32 height_;
            int32 channels_;
            uint32 mem_width_;
            uint32 datasize_;
            bool bmp_upside_down_;

            void clear();
            void setup(int32 height, int32 width, int32 channels);
            void dump() const;

        public:
            BmpFile();
            ~BmpFile();
            BmpFile(BmpFile &&) = default;
            BmpFile &operator=(BmpFile &&) = default;

            int width() const { return width_; }
            int height() const { return height_; }
            int channels() const { return channels_; }
            uint32 datasize() const { return datasize_; }

            void set(ubyte *data, int32 height, int32 width, int32 channels, int32 select_color = -1);
            bool copy(ubyte *data, int32 select_color = -1);
            void save(const string &filename, bool is_dump);
            void load(const string &filename, int32 &height, int32 &width, int32 &channels, bool is_dump);

            IS_DISABLE_COPY_AND_ASSIGN(BmpFile)
        };
    }
}