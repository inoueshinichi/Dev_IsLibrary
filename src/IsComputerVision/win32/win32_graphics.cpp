#include <IsComputerVision/win32/win32_graphics.hpp>

#include <cstring>

namespace Is
{
    namespace imgproc
    {
        namespace win32
        {
            WinBmp::WinBmp() 
                : bmp_(nullptr)
                , width_(0)
                , height_(0)
                , mem_width_(0)
                , channels_(0)
                , data_size_(0) {}

            WinBmp::~WinBmp() 
            {
                clear();
            }

            void WinBmp::clear()
            {
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
                width_ = 0;
                height_ = 0;
                mem_width_ = 0;
                channels_ = 0;
                data_size_ = 0;
            }

            void WinBmp::setup(int32_t width, int32_t height, int32_t channels)
            {
                clear();

                // BmpInfoHeaderのメモリサイズを計算
                size_t infosize = 0;
                if (channels == 1)
                    infosize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
                else
                    infosize = sizeof(BITMAPINFOHEADER);

                // 4バイト境界単位の画像1行のメモリサイズを計算
                int padding = 0;
                uint16_t bit_count = 0;
                size_t mem_width = 0;
                if (channels == 1)
                {
                    bit_count = 8;
                    padding = width % 4;
                    if (padding)
                        mem_width = width + (4 - padding);
                    else
                        mem_width = width;
                } 
                else if (channels == 3) 
                {
                    bit_count = 24;
                    padding = (3 * width) % 4;
                    if (padding)
                        mem_width = (3 * width) + (4 - padding);
                    else
                        mem_width = (3 * width);
                } 
                else if (channels == 4) 
                {
                    bit_count = 32;
                    mem_width = 4 * width;
                } 
                else 
                {
                    throw std::runtime_error(
                        utils::format_string("Channels is not match. Given is %d", 
                                                channels));
                }

                this->datasize_ = mem_width * height;
                this->channels_ = channels;
                this->width_ = width;
                this->height_ = height;
                this->mem_width_ = mem_width;

                /*BmpFileHeader*/
                bmp_file_header_.bfType = (0x4d << 8) | 0x42; // 'B'|'M' (リトルエンディアン環境なので実際は 'MB')
                bmp_file_header_.bfSize = sizeof(BITMAPFILEHEADER) + infosize + this->datasize_;
                bmp_file_header_.bfReserved1 = (WORD)0;
                bmp_file_header_.bfReserved1 = (WORD)0;
                bmp_file_header_.bfOffBits = sizeof(BITMAPFILEHEADER) + infosize;

                /*BmiInfoHeader*/
                bmi_info_ = (BITMAPINFO *)new BYTE[infosize];
                bmi_info_->bmiHeader.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
                bmi_info_->bmiHeader.biWidth = (LONG)width;
                bmi_info_->bmiHeader.biHeight = (LONG)height;
                bmi_info_->bmiHeader.biPlanes = (WORD)1;
                bmi_info_->bmiHeader.biBitCount = (WORD)bit_count;
                bmi_info_->bmiHeader.biCompression = (DWORD)0; // 無圧縮
                bmi_info_->bmiHeader.biSizeImage = (DWORD)data_size;
                bmi_info_->bmiHeader.biXPelsPerMeter = (LONG)0; // 無効
                bmi_info_->bmiHeader.biYPelsPerMeter = (LONG)0; // 無効
                bmi_info_->bmiHeader.biClrUsed = (DWORD)0; // 全カラー使う
                bmi_info_->bmiHeader.biClrImportant = (DWORD)0; // 全カラー使う

                if (channels == 1)
                {
                    /*カラーパレット*/
                    for (int n = 0; n < 256; ++n)
                    {
                        bmi_info_->bmiColors[n].rgbBlue = BYTE(n);
                        bmi_info_->bmiColors[n].rgbGreen = BYTE(n);
                        bmi_info_->bmiColors[n].rgbRed = BYTE(n);
                        bmi_info_->bmiColors[n].rgbReserved = 0;
                    }
                }
                
                /*データ部*/
                bmp_ = new byte[this->datasize_];
                std::memset((void *)bmp_, 0, this->datasize_);
            }

            void WinBmp::set_data(BYTE *data, int width, int height, int channels, int insert_color)
            {
                // Bmpフォーマット作成
                this->setup(width, height, channels);

                size_t wdata = 0;
                size_t wbmp = mem_width_ * (height_ - 1);
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
                    if (insert_color > 0 && insert_color < channels_)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0, k = 0; k < 3 * width_; i+=3)
                            {
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
                    if (insert_color > 0 && insert_color < channels_)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < 4 * width_; i+=4)
                            {
                                bmp_[wbmp + i + insert_color] = data[wdata + i + insert_color];
                            }
                            wdata += 4 * width_;
                            wbmp -= mem_width_;
                        }
                    }
                    else
                    {
                        /*常に4バイトの倍数なので、一括コピー*/
                        std::memmove(bmp_, data, sizeof(BYTE) * mem_width_ * height_);
                    }
                }
            }

            BOOL WinBmp::get_data(BYTE* data)
            {
                if (!bmp_ || !bmi_info_)
                    return false;

                /**
                 * @brief bitmap形式の高さについて
                 * 高さには+符号と-符号の2種類がある.
                 * +符号: 画像データの構造がボトムアップ型. 左下(0,0)
                 * -符号: 画像データの構造がトップダウン型. 左上(0,0)
                 * 通常、互換性の観点から-符号は非推奨.
                 */
                if (bmi_info_->bmiHeader.biHeight > 0)
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
                                for (int i = 0; i < 3 * width_; i += 3)
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
                                for (int i = 0; i < 3 * width_; i += 3)
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
                                for (int i = 0, k = 0; k < 4 * width_; i += 4)
                                {
                                    /*常に4バイト境界*/
                                    data[wdata + i + extract_color] = bmp_[wbmp + i + extract_color];
                                }
                            }
                        }
                        else
                        {
                            /*常に4バイトの倍数なので、一括コピー*/
                            std::memmove(data, bmp_, sizeof(byte) * mem_width_ * height_);
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
                                for (int i = 0; i < 3 * width_; i += 3)
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
                                for (int i = 0; i < 3 * width_; i += 3)
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
                                for (int i = 0, k = 0; k < 4 * width_; i += 4)
                                {
                                    /*常に4バイト境界*/
                                    data[wdata + i + extract_color] = bmp_[wbmp + i + extract_color];
                                }
                            }
                        }
                        else
                        {
                            /*常に4バイトの倍数なので、一括コピー*/
                            std::memmove(data, bmp_, sizeof(byte) * mem_width_ * height_);
                        }
                    }
                }

                return true;
            }
            

            void WinBmp::view_on_window(HDC hdc, const RECT& rect, int mode)
            {
                if (!bmp_ || !bmi_info_)
                    return;
                
                // Win API
                ::SetStretchBltMode(hdc, mode);
                ::StretchDIBits(
                    hdc,
                    rect.left,
                    rect.top,
                    rect.right,
                    rect.bottom,
                    0,
                    0,
                    width_,
                    height_,
                    bmp_,
                    bmi_info_,
                    DIB_RGB_COLORS,
                    SRCCOPY);
            }

            BOOL WinBmp::save(const CString &filename, BOOL is_dump)
            {
#ifdef UNICODE
                static_assert(false, "User must use utf-8 multibyte environment on Windows.");
#else
                std::cout << "save: WinBmp" << std::endl;
#endif
                if (!bmp_ || !bmi_info_)
                    return false;

                auto lambda_fclose = [](FILE *fp) {
                    if (!fp)
                        return;
                    fclose(fp);
                };

                // Bmpファイルの書き出し
                std::shared_ptr<FILE> fp(fopen(filename.GetString(), "wb"), lambda_fclose);
                if (!fp)
                {
                    std::cerr << utils::format_string("Can not open %s", filename.GetString()) << std::endl;
                }

                try
                {
                    /*BITMAPFILEHEADER*/
                    fwrite(&bmp_file_header_, sizeof(BITMAPFILEHEADER), 1, fp.get());

                    /*BmiInfo*/
                    size_t bmp_infosize = bmp_file_header_.bfOffBits - sizeof(BITMAPFILEHEADER);
                    fwrite(bmi_info_, bmp_infosize, 1, fp.get());

                    /*Data*/
                    fwrite(bmp_, this->datasize_, 1, fp.get());

                    if (is_dump)
                        this->dump();
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }

                return true;
            }

            BOOL WinBmp::load(const CString &filename, int &width, int &height, int &channels, BOOL is_dump)
            {
#ifdef UNICODE
                static_assert(false, "User must use utf-8 multibyte environment on Windows.");
#else
                std::cout << "load: WinBmp" << std::endl;
#endif
                auto lambda_fclose = [](FILE *fp) {
                    if (!fp)
                        return;
                    fclose(fp);
                };

                // Bmpファイルの読み出し
                std::shared_ptr<FILE> fp(fopen(filename.GetString()(), "rb"), lambda_fclose);
                if (!fp)
                {
                    std::cerr << utils::format_string("Can not open %s", filename.GetString()()) << std::endl;
                    return false;
                }

                try
                {
                    /*BITMAPFILEHEADER*/
                    fread(&bmp_file_header_, sizeof(BITMAPFILEHEADER), 1, fp.get());

                    /*BITMAPINFO*/
                    size_t bmp_infosize = bmp_file_header_.bfOffBits - sizeof(BITMAPFILEHEADER);
                    bmi_info_ = (BmiInfo *)new byte[bmp_infosize];
                    fread(bmi_info_, bmp_infosize, 1, fp.get());
                    this->width_ = bmi_info_->bmi_header.biWidth;
                    this->height_ = bmi_info_->bmi_header.biHeight
                                        ? bmi_info_->bmi_header.biHeight
                                        : -bmi_info_->bmi_header.biHeight;

                    // 4バイト境界単位の画像1行のメモリサイズを計算
                    int padding = 0;
                    uint16_t bit_count = bmi_info_->bmi_header.biBitCount;
                    size_t mem_width = 0;
                    if (bit_count == 8)
                    {
                        this->channels_ = 1;
                        padding = this->width_ % 4;
                        if (padding)
                            mem_width = this->width_ + (4 - padding);
                        else
                            mem_width = this->width_;
                    }
                    else if (bit_count == 24)
                    {
                        this->channels_ = 3;
                        padding = (3 * this->width_) % 4;
                        if (padding)
                            mem_width = (3 * this->width_) + (4 - padding);
                        else
                            mem_width = 3 * this->width_;
                    }
                    else if (bit_count == 32)
                    {
                        this->channels_ = 4;
                        mem_width = 4 * this->width_;
                    }
                    else
                    {
                        throw std::runtime_error(
                            utils::format_string("Bitcount is not match. Given is %d",
                                                 bit_count));
                    }

                    this->mem_width_ = mem_width;
                    this->datasize_ = mem_width * this->height_;

                    /*Data*/
                    bmp_ = new byte[this->datasize_];
                    fread(bmp_, this->datasize_, 1, fp.get());

                    if (is_dump)
                        this->dump();

                    width = this->width_;
                    height = this->height_;
                    channels = this->channels_;
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }

                return true;
            }

            void WinBmp::dump() const
            {
                if (!bmp_ || !bmi_info_)
                    return;

                printf("WinBmp::dump\n");
                printf("----- BITMAPFILEHEADER -----\n");
                printf("sizeof(BITMAPFILEHEADER): %ld\n", sizeof(BITMAPFILEHEADER));
                printf("bfType = %d\n", bmp_file_header_.bfType);
                printf("bfSize = %d\n", bmp_file_header_.bfSize);
                printf("bfReserved1 = %d\n", bmp_file_header_.bfReserved1);
                printf("bfReserved1 = %d\n", bmp_file_header_.bfReserved1);
                printf("bfOffBits = %d\n", bmp_file_header_.bfOffBits);
                printf("----- BITMAPINFO -----\n");
                printf("sizeof(BITMAPINFO): %ld\n", sizeof(BITMAPINFO));
                printf("biSize = %d\n", bmi_info_->bmiHeader.biSize);
                printf("biWidth = %d\n", bmi_info_->bmiHeader.biWidth);
                printf("biHeight = %d\n", bmi_info_->bmiHeader.biHeight);
                printf("biPlanes = %d\n", bmi_info_->bmiHeader.biPlanes);
                printf("biBitCount = %d\n", bmi_info_->bmiHeader.biBitCount);
                printf("biCompression = %d\n", bmi_info_->bmiHeader.biCompression);
                printf("biSizeImage = %d\n", bmi_info_->bmiHeader.biSizeImage);
                printf("biXPelsPerMeter = %d\n", bmi_info_->bmiHeader.biXPelsPerMeter);
                printf("biYPelsPerMeter = %d\n", bmi_info_->bmiHeader.biYPelsPerMeter);
                printf("biClrUsed = %d\n", bmi_info_->bmiHeader.biClrUsed);
                printf("biClrImportant = %d\n", bmi_info_->bmiHeader.biClrImportant);
            }
        } // namespace win32
    } // namespace imgproc
}