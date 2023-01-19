#pragma once

#include <IsComputerVision/IsComputerVision.hpp>

#include <cstdint>

namespace is
{
    namespace cv
    {
        namespace format
        {
            using std::string;
            using byte = unsigned char;

            class IS_CV_API JpgFile
            {
                int32_t width_;
                int32_t height_;
                size_t mem_width_;
                int32_t channels_;
                size_t datasize_;

                void clear();
                void setup(int32_t height, int32_t width, int32_t channels);
                void dump() const;

            public:
                JpgFile();
                virtual ~JpgFile();
                JpgFile(const JpgFile&) = delete;
                JpgFile& operator=(const JpgFile&) = delete;
                JpgFile(JpgFile&&) = default;
                JpgFile& operator=(JpgFile&&) = default;

                int width() const { return width_; }
                int height() const { return height_; }
                int channels() const { return channels_; }
                size_t datasize() const { return datasize_; }

                void set_data(byte *data, int height, int width, int channels, int insert_color = -1);
                bool get_data(byte *data, int extract_color = -1);
                bool save(const string &filename, bool is_dump);
                bool load(const string &filename, int &height, int &width, int &channels, bool is_dump);
            };
        } // namespace format
    } // namespace imgproc
}