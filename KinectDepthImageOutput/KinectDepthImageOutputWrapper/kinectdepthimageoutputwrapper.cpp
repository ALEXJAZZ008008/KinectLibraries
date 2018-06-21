#include "kinectdepthimageoutputwrapper.h"

namespace KinectDepthImageOutputWrapper
{
    namespace
    {
        int start()
        {
            cout << "KinectDepthImageOutputWrapper kinectdepthimageoutputwrapper:   Start" << endl;

            return 0;
        }
    }

    int KinectDepthImageOutputToConsole()
    {
        start();

        cout << KinectDepthImageOutputLibraryMain() << endl;

        return 0;
    }
}
