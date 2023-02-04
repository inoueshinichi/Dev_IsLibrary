#include <IsComputerVision/io/format/jpg_policy.hpp>

namespace is
{
    namespace cv
    {
        /*----------------------------------------------------------------------------------*/
        JpgFile::~JpgFile()
        {
        }

        JpgFile::JpgFile()
        {
        }

        void JpgFile::set_data(byte *data, int height, int width, int channels, int insert_color)
        {
        }

        bool JpgFile::get_data(byte *data, int extract_color)
        {
            return true;
        }

        bool JpgFile::save(const string &filename, bool is_dump)
        {
            std::cout << "save : JpgFile" << std::endl;
            return true;
        }

        bool JpgFile::load(const string &filename, int &height, int &width, int &channels, bool is_dump)
        {
            std::cout << "load : JpgFile" << std::endl;

            return true;
        }
    } // namespace imgproc
}