#include "src/include/kinectdepthimageoutputlibrary.h"

KinectBackend::KinectBackend(): m_fctx_ptr(nullptr), m_fdev_ptr(nullptr), m_current_tilt_state_ptr(nullptr), m_output(), m_depth_output(), m_video_output(), m_camera_tilt(0.0), m_increment(2.0), m_num_devices(0)
{

}

KinectBackend::~KinectBackend()
{
    if(m_fdev_ptr != nullptr)
    {
        freenect_stop_depth(m_fdev_ptr);
        freenect_stop_video(m_fdev_ptr);
        freenect_close_device(m_fdev_ptr);

        m_fdev_ptr = nullptr;
    }

    if(m_fctx_ptr != nullptr)
    {
        freenect_shutdown(m_fctx_ptr);

        m_fctx_ptr = nullptr;
    }
}

KinectBackend & KinectBackend::getInstance( )
{
    static KinectBackend instance;

    return instance;
}

void KinectBackend::depth_callback(freenect_device *fdev_ptr, void *data, unsigned int timestamp)
{
    KinectBackend::getInstance().append_depth_output("Received depth frame at " + to_string(timestamp) + "\n");
}

void KinectBackend::video_callback(freenect_device *fdev_ptr, void *data, unsigned int timestamp)
{
    KinectBackend::getInstance().append_video_output("Received video frame at "  + to_string(timestamp) + "\n");
}

int KinectBackend::create_connections()
{
    if (freenect_init(&m_fctx_ptr, NULL) < 0)
    {
        m_output += "Initialisation error!\n";

        KinectBackend::getInstance().~KinectBackend();

        return -1;
    }

    freenect_set_log_level(m_fctx_ptr, FREENECT_LOG_DEBUG);
    freenect_select_subdevices(m_fctx_ptr, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

    m_num_devices = freenect_num_devices(m_fctx_ptr);

    if (m_num_devices == 0)
    {
        m_output += "No devices found!!\n";

        return -1;
    }
    else
    {
        m_output += "Num devices found: " + to_string(m_num_devices) + "\n";
    }

    //Open the device.
    if (freenect_open_device(m_fctx_ptr, &m_fdev_ptr, 0) < 0)
    {
        m_output += "Unable to connect with camera!!\n";

        return -1;
    }

    //Set depth and video modes
    if(freenect_set_depth_mode(m_fdev_ptr, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM)))
    {
        m_output += "Unable to set depth mode!!\n";

        return -1;
    }

    if (freenect_set_video_mode(m_fdev_ptr, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB)))
    {
        m_output += "Unable to set video mode!!\n";

        return -1;
    }

    //Set frame callback
    freenect_set_depth_callback(m_fdev_ptr, KinectBackend::depth_callback);
    freenect_set_video_callback(m_fdev_ptr, KinectBackend::video_callback);

    if (freenect_start_depth(m_fdev_ptr))
    {
        m_output += "Unable to start depth!!\n";

        return -1;
    }

    if (freenect_start_video(m_fdev_ptr))
    {
        m_output += "Unable to start video!!\n";

        return -1;
    }

    set_camera_tilt(0.0);

    m_output += "Connection established!!\n";
}

int KinectBackend::set_camera_tilt(double increment)
{
    update_tilt_state();

    freenect_tilt_status_code tilt_state = freenect_get_tilt_status(m_current_tilt_state_ptr);

    double new_camera_tilt = get_current_camera_tilt() + (m_increment * increment);

    switch(tilt_state)
    {
    case freenect_tilt_status_code::TILT_STATUS_MOVING:

        m_output += "Unable to tilt the camera!!\n";

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_LIMIT:

        m_output += "Unable to tilt the camera!!\n";

        if(new_camera_tilt > 0)
        {
            set_current_camera_tilt(new_camera_tilt - ((m_increment * increment) * 2.0));
        }
        else
        {
            set_current_camera_tilt(new_camera_tilt + ((m_increment * increment) * 2.0));
        }

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_STOPPED:

        set_current_camera_tilt(new_camera_tilt);

        return 0;
    }

    return -1;
}

string KinectBackend::get_output()
{
    string output = m_output;

    m_output = "";

    return output;
}

int KinectBackend::update_tilt_state()
{
    if (freenect_update_tilt_state(m_fdev_ptr) < 0)
    {
        m_output += "Unable to retrieve tilt status!!\n";

        return -1;
    }

    m_current_tilt_state_ptr = freenect_get_tilt_state(m_fdev_ptr);

    return 0;
}

double KinectBackend::get_current_camera_tilt()
{
    return freenect_get_tilt_degs(m_current_tilt_state_ptr);
}

int KinectBackend::set_current_camera_tilt(double new_camera_tilt)
{
    freenect_set_tilt_degs(m_fdev_ptr, new_camera_tilt);

    m_camera_tilt = new_camera_tilt;

    return 0;
}
