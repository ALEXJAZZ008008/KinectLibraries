#include "src/include/kinectdepthimageoutputlibrary.h"

namespace KinectDepthImageOutputLibrary
{
    namespace
    {
        string start()
        {
            return "KinectDepthImageOutputLibrary kinectdepthimageoutputlibrary:   Start\r\n";
        }
    }

    string KinectDepthImageOutputLibraryMain()
    {
        string output = string();

        output += start();

        return output;
    }
}
