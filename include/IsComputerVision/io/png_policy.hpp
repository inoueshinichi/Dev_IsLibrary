// #pragma once

// #include <png.h>

// #include <IsComputerVision/common.hpp>

// #include <cstdint>

// namespace is
// {
// namespace imgproc
// {
// namespace format
// {
//     using std::string;
//     using uchar = unsigned char;
//     using std::shared_ptr;

//     class IS_CV_API PngFile
//     {
//         // libpng
//         png_structp png_;
//         png_infop   info_;
//         png_bytepp  row_ptrs_;
//         png_byte    color_type_;
//         png_byte    bit_depth_;
//         png_byte    sig_[8];
//         png_colorp palette_;
//         int num_palette_;

//         // bmp
//         uchar *bmp_;

//         int32_t width_;
//         int32_t height_;
//         int32_t channels_;
//         size_t mem_width_;
//         size_t datasize_;

//         void clear();
//         void setup(int32_t height, int32_t width, int32_t channels);
//         void dump() const;

//     public:
//         PngFile();
//         virtual ~PngFile();
//         PngFile(const PngFile &) = delete;
//         PngFile &operator=(const PngFile &) = delete;
//         PngFile(PngFile &&) = default;
//         PngFile &operator=(PngFile &&) = default;

//         int width() const { return width_; }
//         int height() const { return height_; }
//         int channels() const { return channels_; }
//         size_t datasize() const { return datasize_; }

//         void set_data(uchar *data, int width, int height, int channels, int insert_color = -1);
//         bool get_data(uchar *data, int extract_color = -1);
//         bool save(const string &filename, bool is_dump);
//         bool load(const string &filename, int &height, int &width, int &channels, bool is_dump);
//     };
// } // namespace format
// } // namespace imgproc
// }