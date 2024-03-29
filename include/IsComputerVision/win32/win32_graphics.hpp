#pragma once

#include "IsComputerVision/IsComputerVision.hpp"

#include <Windows.h>
#include <atlstr.h> // CString

static_assert(_MSC_FULL_VER >= 190023506, 
              "Compile with newer than Visual Studio 2015 [v14.0] Update 1.");

namespace is
{
    namespace cv
    {
        
        class IS_CV_API WinBmp
        {
        protected:
            /*Windows Bitmap*/
            BITMAPFILEHEADER bmp_file_header_;
            BITMAPINFO *bmi_info_;

            BYTE *bmp_;
            int width_;
            int height_;
            size_t mem_width_;
            int channels_;
            size_t datasize_;

            void clear();
            void setup(int width, int height, int channels);
            void dump() const;

        public:
            WinBmp();
            virtual ~WinBmp();
            WinBmp(const WinBmp &) = delete;
            WinBmp &operator=(const WinBmp &) = delete;
            WinBmp(WinBmp &&) = default;
            WinBmp &operator=(WinBmp &&) = default;

            int width() const { return width_; }
            int height() const { return height_; }
            int channels() const { return channels_; }
            size_t datasize() const { return datasize_; }
            void set_data(BYTE *data, int width, int height, int channels, int insert_color = -1);
            bool get_data(BYTE *data, int extract_color = -1);
            void view_on_window(HDC hdc, const RECT &rect, int mode = COLORONCOLOR);

            bool save(const CString &filename, bool is_dump);
            bool load(const CString &filename, int &width, int &height, int &channels, bool is_dump);
        };
    } // namespace imgproc
}