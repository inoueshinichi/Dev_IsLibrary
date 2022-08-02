#pragma once

#include "IsComputerVision/common.hpp"

#include <Windows.h>
#include <atlstr.h> // CString

static_assert(_MSC_FULL_VER >= 190023506, 
              "Compile with newer than Visual Studio 2015 [v14.0] Update 1.");

namespace is
{
    namespace imgproc
    {
        namespace win32
        {
            class IMGPROC_API WinBmp
            {
            protected:
                /*Windows Bitmap*/
                BITMAPFILEHEADER bmp_file_header_;
                BITMAPINFO *bmi_info_;

                BYTE *bmp_;
                int32_t width_;
                int32_t height_;
                size_t mem_width_;
                int32_t channels_;
                size_t datasize_;

                void clear();
                void setup(int32_t width, int32_t height, int32_t channels);
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
                BOOL get_data(BYTE *data, int extract_color = -1);
                void view_on_window(HDC hdc, const RECT &rect, int mode = COLORONCOLOR);

                BOOL save(const CString &filename, BOOL is_dump);
                BOOL load(const CString &filename, int &width, int &height, int &channels, BOOL is_dump);
            };
        } // namespace win32
    } // namespace imgproc
}