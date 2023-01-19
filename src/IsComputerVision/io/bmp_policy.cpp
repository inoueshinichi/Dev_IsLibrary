#include <IsComputerVision/io/bmp_policy.hpp>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <iostream>

namespace is
{
    namespace imgproc
    {
        namespace format
        {
            BmpFile::BmpFile()
                : bmi_info_(nullptr)
                , bmp_(nullptr) 
                , bmp_upside_down_(false)
            {
                // BmpFileHeader
                bmp_file_header_.bf_type = (uint16_t)0;
                bmp_file_header_.bf_size = (uint32_t)0;
                bmp_file_header_.bf_reserved1 = (uint16_t)0;
                bmp_file_header_.bf_reserved2 = (uint16_t)0;
                bmp_file_header_.bf_offset_bits = (uint32_t)0;

                width_ = (int32_t)0;
                height_ = (int32_t)0;
                channels_ = (int32_t)0;
                mem_width_ = (size_t)0;
                datasize_ = (size_t)0;
            }

            BmpFile::~BmpFile()
            {
                clear(); 
            }

            void BmpFile::clear()
            {
                bmp_file_header_ = /*(BmpFileHeader)*/{ 0 };
                if (bmi_info_)
                {
                    delete[] bmi_info_;
                    bmi_info_ = nullptr;
                }
                if (bmp_)
                {
                    delete[] bmp_;
                    bmp_ = nullptr;
                }

                bmp_upside_down_ = false;
            }

            void BmpFile::setup(int32_t height, int32_t width, int32_t channels)
            {
                if (!(channels == 1 || channels == 3 || channels == 4))
                {
                    throw std::runtime_error(
                            is::common::format_string("Channels is not match. Given is %d", channels));
                }

                // BmpInfoHeaderのメモリサイズを計算
                size_t infosize = 0;

                /*RgbQuad*/
                if (channels == 1)
                    infosize = sizeof(BmpInfoHeader) + sizeof(RgbQuad) * 256;
                else
                    infosize = sizeof(BmpInfoHeader);

                
                // 4バイト境界単位の画像1行のメモリサイズを計算
                uint16_t bit_count = 8 * channels; 
                int padding = (channels * width) % 4;
                size_t mem_width = (channels * width);
                if (padding) 
                    mem_width += (4 - padding);

                // 諸元を設定
                this->width_ = width;
                this->height_ = height;
                this->channels_ = channels;
                this->mem_width_ = mem_width;
                this->datasize_ = mem_width * height;

                // 既存のイメージデータを削除
                clear();
    
                /*BmpFileHeader*/
                bmp_file_header_.bf_type = (0x4d << 8) | 0x42; // 'B'|'M' (リトルエンディアン環境なので実際は 'MB')
                bmp_file_header_.bf_size = sizeof(BmpFileHeader) + infosize + this->datasize_;
                bmp_file_header_.bf_reserved1 = (uint16_t)0;
                bmp_file_header_.bf_reserved2 = (uint16_t)0;
                bmp_file_header_.bf_offset_bits = sizeof(BmpFileHeader) + infosize;

                /*BmiInfoHeader*/
                bmi_info_ = (BmiInfo *)new uchar[infosize];
                bmi_info_->bmi_header.bi_size = sizeof(BmpInfoHeader);
                bmi_info_->bmi_header.bi_width = width;
                bmi_info_->bmi_header.bi_height = height;
                bmi_info_->bmi_header.bi_planes = 1;
                bmi_info_->bmi_header.bi_bitcount = bit_count;
                bmi_info_->bmi_header.bi_compression = 0; // 無圧縮
                bmi_info_->bmi_header.bi_size_image = this->datasize_;
                bmi_info_->bmi_header.bi_x_pels_per_meter = 0; // 無効
                bmi_info_->bmi_header.bi_y_pels_per_meter = 0; // 無効
                bmi_info_->bmi_header.bi_color_used = 0; // 全カラー使う
                bmi_info_->bmi_header.bi_clor_important = 0; // 全カラー使う

                if (channels == 1)
                {
                     /*カラーパレット*/
                    for (int n = 0; n < 256; ++n)
                    {
                        bmi_info_->bmi_colors[n].rgb_blue = uchar(n);
                        bmi_info_->bmi_colors[n].rgb_green = uchar(n);
                        bmi_info_->bmi_colors[n].rgb_red = uchar(n);
                        bmi_info_->bmi_colors[n].rgb_reserved = 0;
                    }
                }
               
                /*データ部*/
                bmp_ = new uchar[this->datasize_];
                std::memset((void*)bmp_, 0, this->datasize_);
            }

            void BmpFile::set(uchar* data, int height, int width, int channels, int insert_color)
            {
                // Bmpフォーマット作成
                this->setup(height, width, channels);

                size_t wdata = 0;
                int64_t wbmp = mem_width_ * (height_ - 1);
                if (channels_ == 1)
                {
                    for (int j = 0; j < height_; ++j)
                    {
                        for (int i = 0; i < width_; ++i)
                        {
                            /*4バイト境界の端数を無視して保存*/
                            bmp_[wbmp + i] = data[wdata + i];
                        }
                        wdata += width_;
                        wbmp -= mem_width_;
                    }
                }
                else if (channels_ == 3)
                {
                    if (insert_color >= 0 && insert_color < channels_)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < 3 * width_; i+=3)
                            {
                                /*4バイト境界の端数を無視して保存*/
                                bmp_[wbmp + i + insert_color] = data[wdata + i + insert_color];
                            }
                            wdata += 3 * width_;
                            wbmp -= mem_width_;
                        }
                    }
                    else
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < 3 * width_; i+=3)
                            {
                                /*4バイト境界の端数を無視して保存*/
                                bmp_[wbmp + i + 0] = data[wdata + i + 0];
                                bmp_[wbmp + i + 1] = data[wdata + i + 1];
                                bmp_[wbmp + i + 2] = data[wdata + i + 2];
                            }
                            wdata += 3 * width_;
                            wbmp -= mem_width_;
                        }
                    }
                }
                else // channels == 4
                {
                    if (insert_color >= 0 && insert_color < channels_)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < 4 * width_; i+=4)
                            {
                                /*常に4バイト境界*/
                                bmp_[wbmp + i + insert_color] = data[wdata + i + insert_color];
                            }
                            wdata += 4 * width_;
                            wbmp -= mem_width_;
                        }
                    }
                    else
                    {
                        /*常に4バイトの倍数なので、一括コピー*/
                        std::memmove(bmp_, data, sizeof(uchar) * mem_width_ * height_);
                    }
                }
            }

            bool BmpFile::copy(uchar* data, int extract_color)
            {
                if (!bmp_ || !bmi_info_)
                    return false;

                /**
                 * @note bitmap形式の高さについて
                 * 高さには+符号と-符号の2種類がある.
                 * +符号: 画像データの構造がボトムアップ型. 左下(0,0)
                 * -符号: 画像データの構造がトップダウン型. 左上(0,0)
                 * 通常、互換性の観点から-符号は非推奨.
                 */
                if (!bmp_upside_down_)
                {
                    size_t wdata = 0;
                    int64_t wbmp = mem_width_ * (height_ - 1);
                    if (channels_ == 1)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < width_; ++i)
                            {
                                /*4バイト境界の端数を無視して抽出*/
                                data[wdata + i] = bmp_[wbmp + i];
                            }
                            wdata += width_;
                            wbmp -= mem_width_;
                        }
                    }
                    else if (channels_ == 3)
                    {
                        if (extract_color > 0 && extract_color < channels_)
                        {
                            for (int j = 0; j < height_; ++j)
                            {
                                for (int i = 0; i < 3 * width_; i+=3)
                                {
                                    /*4バイト境界の端数を無視して抽出*/
                                    data[wdata + i + extract_color] = bmp_[wbmp + i + extract_color];
                                }
                                wdata += 3 * width_;
                                wbmp -= mem_width_;
                            }
                        }
                        else
                        {
                            for (int j = 0; j < height_; ++j)
                            {
                                for (int i = 0; i < 3 * width_; i+=3)
                                {
                                    /*4バイト境界の端数を無視して抽出*/
                                    data[wdata + i + 0] = bmp_[wbmp + i + 0];
                                    data[wdata + i + 1] = bmp_[wbmp + i + 1];
                                    data[wdata + i + 2] = bmp_[wbmp + i + 2];
                                }
                                wdata += 3 * width_;
                                wbmp -= mem_width_;
                            }
                        }
                    }
                    else // channels == 4
                    {
                        if (extract_color > 0 && extract_color < channels_)
                        {
                            for (int j = 0; j < height_; ++j)
                            {
                                for (int i = 0, k = 0; k < 4 * width_; i+=4)
                                {
                                    /*常に4バイト境界*/
                                    data[wdata + i + extract_color] = bmp_[wbmp + i + extract_color];
                                }
                            }
                        }
                        else
                        {
                            /*常に4バイトの倍数なので、一括コピー*/
                            std::memmove(data, bmp_, sizeof(uchar) * mem_width_ * height_);
                        }
                    }
                }
                else
                {
                    size_t wdata = 0;
                    int64_t wbmp = 0;
                    if (channels_ == 1)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < width_; ++i)
                            {
                                /*4バイト境界の端数を無視して抽出*/
                                data[wdata + i] = bmp_[wbmp + i];
                            }
                            wdata += width_;
                            wbmp += mem_width_;
                        }
                    }
                    else if (channels_ == 3)
                    {
                        if (extract_color > 0 && extract_color < channels_)
                        {
                            for (int j = 0; j < height_; ++j)
                            {
                                for (int i = 0; i < 3 * width_; i+=3)
                                {
                                    /*4バイト境界の端数を無視して抽出*/
                                    data[wdata + i + extract_color] = bmp_[wbmp + i + extract_color];
                                }
                                wdata += 3 * width_;
                                wbmp += mem_width_;
                            }
                        }
                        else
                        {
                            for (int j = 0; j < height_; ++j)
                            {
                                for (int i = 0; i < 3 * width_; i+=3)
                                {
                                    /*4バイト境界の端数を無視して抽出*/
                                    data[wdata + i + 0] = bmp_[wbmp + i + 0];
                                    data[wdata + i + 1] = bmp_[wbmp + i + 1];
                                    data[wdata + i + 2] = bmp_[wbmp + i + 2];
                                }
                                wdata += 3 * width_;
                                wbmp += mem_width_;
                            }
                        }
                    }
                    else // channels == 4
                    {
                        if (extract_color > 0 && extract_color < channels_)
                        {
                            for (int j = 0; j < height_; ++j)
                            {
                                for (int i = 0, k = 0; k < 4 * width_; i+=4)
                                {
                                    /*常に4バイト境界*/
                                    data[wdata + i + extract_color] = bmp_[wbmp + i + extract_color];
                                }
                            }
                        }
                        else
                        {
                            /*常に4バイトの倍数なので、一括コピー*/
                            std::memmove(data, bmp_, sizeof(uchar) * mem_width_ * height_);
                        }
                    }
                }

                return true;
            }

            /*----------------------------------------------------------------------------------*/

            void BmpFile::save(const string& filename, bool is_dump)
            {
                std::cout << "Save bmp file." << std::endl;

                if (!bmp_ || !bmi_info_)
                {
                    std::cerr << is::common::format_string("No data : %s", filename.c_str()) << std::endl;
                    return;
                }
                    

                auto lambda_fclose = [](FILE *fp) {
                    if (!fp)
                        return;
                    fclose(fp);
                };

                // Bmpファイルの書き出し
                std::shared_ptr<FILE> fp(fopen(filename.c_str(), "wb"), lambda_fclose);
                if (!fp)
                {
                    std::cerr << is::common::format_string("Can not open %s", filename.c_str()) << std::endl;
                    return;
                }

                try
                {
                    /*BmpFileHeader*/
                    fwrite(&bmp_file_header_, sizeof(BmpFileHeader), 1, fp.get()); // 書き出し

                    /*BmiInfo*/
                    size_t bmp_infosize = bmp_file_header_.bf_offset_bits - sizeof(BmpFileHeader);
                    fwrite(bmi_info_, bmp_infosize, 1, fp.get()); // 書き出し

                    /*Data*/
                    fwrite(bmp_, this->datasize_, 1, fp.get());

                    if (is_dump)
                        this->dump();
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }

            bool BmpFile::load(const string& filename, int& height, int& width, int& channels, bool is_dump)
            {
                std::cout << "Load bitmap file." << std::endl;

                auto lambda_fclose = [](FILE* fp) {
                    if (!fp) return;
                    fclose(fp);
                };

                // Bmpファイルの読み出し
                std::shared_ptr<FILE> fp(fopen(filename.c_str(), "rb"), lambda_fclose);
                if (!fp)
                {
                    std::cerr << is::common::format_string("Can not open %s", filename.c_str()) << std::endl;
                    return false;
                }

                try
                {
                    /*BmpFileHeader*/
                    BmpFileHeader bmp_file_header;
                    fread(&bmp_file_header, sizeof(BmpFileHeader), 1, fp.get()); // 読み出し

                    uint16_t sig = (0x4d << 8) | 0x42; // 'B'|'M' (リトルエンディアン環境なので実際は 'MB')
                    if (bmp_file_header.bf_type != sig) 
                    {
                        char sigs[2];
                        sigs[0] = (char)(sig >> 8 | 0xff);
                        sigs[1] = (char)(sig | 0xff);
                        throw std::runtime_error(
                            is::common::format_string("Not bmp file. Signature is `%c%c`", sigs[0], sigs[1]));
                    }

                    /*BmiInfo*/
                    size_t bmp_infosize = bmp_file_header.bf_offset_bits - sizeof(BmpFileHeader);
                    BmiInfo* bmi_info = (BmiInfo*)new uchar[bmp_infosize];
                    fread(bmi_info, bmp_infosize, 1, fp.get()); // 読み出し
                    
                    int32_t twidth, theight;
                    bool bmp_upside_down;
                    twidth = bmi_info->bmi_header.bi_width;
                    theight = bmi_info->bmi_header.bi_height;
                    if (theight > 0)
                    {
                        bmp_upside_down = false;
                    }
                    else
                    {
                        // 読み込んだBMPデータが上下逆さだった場合
                        bmp_upside_down = true;
                        theight *= -1;
                    }

                    // 4バイト境界単位の画像1行のメモリサイズを計算
                    int padding = 0;
                    uint16_t bit_count = /*bmi_info_*/bmi_info->bmi_header.bi_bitcount;
                    int32_t tchannels = (int32_t)(bit_count / 8);
                    size_t mem_width = 0;
                    size_t datasize = 0;

                    if (!(tchannels == 1 || tchannels == 3 || tchannels == 4))
                    {
                        delete[] bmi_info;
                        bmi_info = nullptr;
                        throw std::runtime_error(
                            is::common::format_string("Bitcount is not match. Given is %d", 
                                                    bit_count));
                    }

                    padding = (tchannels * twidth) % 4;
                    if (padding == 0)
                    {
                        mem_width = tchannels * twidth;
                    }
                    else
                    {
                        mem_width = tchannels * twidth + (4 - padding);
                    }
                    
                    datasize = mem_width * theight;

                    /*Data*/
                    uchar* bmp = new uchar[datasize];
                    fread(bmp, datasize, 1, fp.get()); // 読み込み

                    // 既存データの削除
                    clear();

                    // 新規データで更新
                    bmp_file_header_ = bmp_file_header;
                    bmi_info_ = bmi_info;
                    bmp_ = bmp;
                    width_ = width = twidth;
                    height_ = height = theight;
                    channels_ = channels = tchannels;
                    mem_width_ = mem_width;
                    datasize_ = datasize;
                    bmp_upside_down_ = bmp_upside_down;
                    
                    if (is_dump)
                        this->dump();
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    return false;
                }

                return true;
            }

            void BmpFile::dump() const
            {
                if (!bmp_ || !bmi_info_)
                    return;

                printf("BmpFile::dump\n");
                printf("----- BmpFileHeader -----\n");
                printf("sizeof(BmpFileHeader): %ld\n", sizeof(BmpFileHeader));
                printf("bf_type = %d\n", bmp_file_header_.bf_type);
                printf("bf_size = %d\n", bmp_file_header_.bf_size);
                printf("bf_reserved1 = %d\n", bmp_file_header_.bf_reserved1);
                printf("bf_reserved2 = %d\n", bmp_file_header_.bf_reserved2);
                printf("bf_offbits = %d\n", bmp_file_header_.bf_offset_bits);
                printf("----- BmpInfoHeader -----\n");
                printf("sizeof(BmpInfoHeader): %ld\n", sizeof(BmpInfoHeader));
                printf("bi_size = %d\n", bmi_info_->bmi_header.bi_size);
                printf("bi_width = %d\n", bmi_info_->bmi_header.bi_width);
                printf("bi_height = %d\n", bmi_info_->bmi_header.bi_height);
                printf("bi_planes = %d\n", bmi_info_->bmi_header.bi_planes);
                printf("bi_bitcount = %d\n", bmi_info_->bmi_header.bi_bitcount);
                printf("bi_compression = %d\n", bmi_info_->bmi_header.bi_compression);
                printf("bi_size_image = %d\n", bmi_info_->bmi_header.bi_size_image);
                printf("bi_x_pels_per_meter = %d\n", bmi_info_->bmi_header.bi_x_pels_per_meter);
                printf("bi_y_pels_per_meter = %d\n", bmi_info_->bmi_header.bi_y_pels_per_meter);
                printf("clr_used = %d\n", bmi_info_->bmi_header.bi_color_used);
                printf("clr_important = %d\n", bmi_info_->bmi_header.bi_clor_important);
            }
        }
    }
}