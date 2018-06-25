#ifndef KINECTDEPTHIMAGEOUTPUTLIBRARY_H
#define KINECTDEPTHIMAGEOUTPUTLIBRARY_H

#include <string>

#include "libfreenect.h"

using namespace std;

//!
//! \class
//! \brief The Kinect Backend class.
//! Finds and connects to a Kinect camera.
//! I case of multiple connected cameras, only the first
//! will be considered.
//! \details The class is a singleton. Which means that only a single object across
//! the application life span, can exist.
//!
class KinectBackend
{
public:
    //! Return a static reference to the singleton object.
    static KinectBackend &getInstance( );

    KinectBackend(KinectBackend &) = delete;
    void operator = (KinectBackend &) = delete;
    KinectBackend(KinectBackend &&) = delete;
    void operator = (KinectBackend &&) = delete;

    //! Create connections
    int create_connections();

    inline double get_camera_tilt()
    {
        return m_camera_tilt;
    }

    int set_camera_tilt(double);

    string get_output();

    inline string get_depth_output()
    {
        return m_depth_output;
    }

    inline int append_depth_output(string depth_output)
    {
        m_depth_output += depth_output;

        return 0;
    }

    inline int set_depth_output(string depth_output)
    {
        m_depth_output = depth_output;

        return 0;
    }

    inline string get_video_output()
    {
        return m_video_output;
    }

    inline int append_video_output(string video_output)
    {
        m_video_output += video_output;

        return 0;
    }

    inline int set_video_output(string video_output)
    {
        m_video_output = video_output;

        return 0;
    }

private:

    explicit KinectBackend();
    ~KinectBackend();

    //! Holds information about the usb context.
    freenect_context *m_fctx_ptr;

    //! Holds device information.
    freenect_device *m_fdev_ptr;

    //! Current tilt state.
    freenect_raw_tilt_state *m_current_tilt_state_ptr;

    //! Output
    string m_output;

    //! Depth output
    string m_depth_output;

    //! Video output
    string m_video_output;

    double m_camera_tilt;

    //! Step of camera tilt.
    double m_increment;

    //! Number of devices connected to the PC.
    int m_num_devices;

    static void depth_callback(freenect_device *, void *, unsigned int);
    static void video_callback(freenect_device *, void *, unsigned int);

    int update_tilt_state();
    double get_current_camera_tilt();
    int set_current_camera_tilt(double);
};

#endif // KINECTDEPTHIMAGEOUTPUTLIBRARY_H
