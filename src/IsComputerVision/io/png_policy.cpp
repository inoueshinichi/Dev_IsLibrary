// #include <IsComputerVision/io/png_policy.hpp>

// namespace is
// {
// namespace cv
// {
// namespace format
// {
//     /*----------------------------------------------------------------------------------*/
//     PngFile::PngFile()
//     {
//         width_ = (int32_t)0;
//         height_ = (int32_t)0;
//         channels_ = (int32_t)0;
//         mem_width_ = (size_t)0;
//         datasize_ = (size_t)0;

//         memset((void *)sig_, 0, sizeof(sig_));
//         png_ = nullptr;
//         info_ = nullptr;
//         row_ptrs_ = nullptr;
//         bmp_ = nullptr;

//         color_type_ = (png_byte)0;
//         bit_depth_ = (png_byte)0;

//         /*インデックスカラー画像*/
//         palette_ = nullptr;
//         num_palette_ = (int)0;
//     }

//     PngFile::~PngFile()
//     {
//         clear();
//     }

//     void PngFile::clear()
//     {
//         if (bmp_)
//         {
//             std::free(bmp_);
//             bmp_ = nullptr;
//         }

//         png_destroy_read_struct(&png_, &info_, NULL);
//     }

//     void PngFile::set_data(uchar *data, int height, int width,
//                             int channels, int insert_color)
//     {

//     }

//     bool PngFile::get_data(uchar *data, int extract_color)
//     {
//         return true;
//     }

//     bool PngFile::save(const string &filename, bool is_dump)
//     {
//         std::cout << "Save : pngfile" << std::endl;
//         return true;
//     }

//     bool PngFile::load(const string &filename, int &height, 
//                        int &width, int &channels, bool is_dump)
//     {
//         std::cout << "Load : pngfile" << std::endl;

//         auto lambda_fclose = [](FILE *fp) {
//             if (!fp)
//                 return;
//             fclose(fp);
//         };

//         // Pngファイルの読み出し
//         shared_ptr<FILE> fp(fopen(filename.c_str(), "rb"), lambda_fclose);
//         if (!fp)
//         {
//             std::cerr << utils::format_string("Can not open %s", 
//                                               filename.c_str()) << std::endl;
//             return false;
//         }

//         try 
//         {
//             /*Header(8byte)*/
//             fread(sig_, sizeof(png_byte) * sizeof(sig_), 1, fp.get());
//             if (png_sig_cmp(sig_, 0, sizeof(sig_)))
//             {
//                 throw std::runtime_error(
//                     utils::format_string(
//                         "[Error libpng] file is not recognized as a png file."
//                         " Given 8 byte header is `%x %x %x %x %x %x %x %x`.",
//                         sig_[0], sig_[1], sig_[2], sig_[3],
//                         sig_[4], sig_[5], sig_[6], sig_[7]));
//             }

//             /*Png*/
//             png_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
//                                             NULL, NULL, NULL);
//             if (!png_)
//             {
//                 throw std::runtime_error(
//                     "[Error libpng] png_create_read_struct func.");
//             }

//             /*Info*/
//             info_ = png_create_info_struct(png_);
//             if (!info_)
//             {
//                 png_destroy_read_struct(&png_, NULL, NULL);
//                 throw std::runtime_error(
//                     "[Error libpng] png_create_info_struct func.");
//             }

//             /*Pngへのデータの書き込み*/
//             png_init_io(png_, fp.get());
//             png_set_sig_bytes(png_, sizeof(png_byte) * sizeof(sig_));
//             png_read_info(png_, info_);

//             // 取得
//             width_ = png_get_image_width(png_, info_);
//             height_ = png_get_image_height(png_, info_);
//             row_ptrs_ = png_get_rows(png_, info_);
//             color_type_ = png_get_color_type(png_, info_);
//             bit_depth_ = png_get_bit_depth(png_, info_);

//             /*画像データをBMP形式で取得*/
//             int channels_ = (bit_depth_ / 8);
//             switch (color_type_)
//             {
//             case PNG_COLOR_TYPE_RGBA:
//                 break;

//             case PNG_COLOR_TYPE_RGB:
//                 break;

//             case PNG_COLOR_TYPE_GRAY:
//                 break;

//             // 8bitインデックスカラー画像
//             // 256色(8bit)カラーパレット
//             // パレットの表現は8bit x 3(RGB) x 256色
//             // 1pixelは256色からのインデックスで決まる
//             case (PNG_COLOR_TYPE_GRAY | PNG_COLOR_TYPE_PALETTE):
//                 png_get_PLTE(png_, info_, &palette_, &num_palette_);
//                 break;
//             }

//             /*メモリ確保*/
//             mem_width_ = sizeof(uchar) * width_ * channels_;
//             datasize_ = mem_width_ * height_;
//             bmp_ = (uchar *)std::malloc(datasize_);
//             if (!bmp_)
//             {
//                 png_destroy_read_struct(&png_, &info_, NULL);
//                 throw std::runtime_error(
//                     "[Error libpng] std::maloc func.");
//             }


//             // コピー
//             if (palette_)
//             {
//                 /*カラーパレット*/
//                 for (int j = 0; j < height_; ++j)
//                 {
//                     for (int i = 0; i < width_; ++i)
//                     {
//                         int index = row_ptrs_[j][i];
//                         bmp_[j * mem_width_ + i + 0] = palette_[index].red;
//                         bmp_[j * mem_width_ + i + 1] = palette_[index].green;
//                         bmp_[j * mem_width_ + i + 2] = palette_[index].blue;
//                     }
//                 }
//             }
//             else
//             {
//                 for (int j = 0; j < height_; ++j)
//                 {
//                     // 行ごとに一括
//                     std::memcpy((void *)&bmp_[j * mem_width_], (void *)row_ptrs_[j], mem_width_);
//                 }
//             }
            
//             // 破棄
//             png_destroy_read_struct(&png_, &info_, NULL);
//         }
//         catch (const std::exception &e)
//         {
//             std::cerr << e.what() << std::endl;
//             return false;
//         }

//         return true;
//     }
// } // namespace format
// } // namespace imgproc
// }