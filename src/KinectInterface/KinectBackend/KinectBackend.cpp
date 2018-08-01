#include "src/include/KinectBackend.h"

KinectBackend::KinectBackend():
    m_kinect_object_ptr(nullptr),
    m_freenect_context_ptr(nullptr),
    m_freenect_device_ptr(nullptr),
    m_current_tilt_state_ptr(nullptr),
    m_reset_camera_tilt(0.0),
    m_increment(2.0),
    m_number_of_devices(0),
    m_depth_image_bool(false),
    m_rgb_image_bool(false),
    m_resolution_small_bool(false),
    m_resolution_med_bool(false),
    m_resolution_high_bool(false)
{

}

KinectBackend::~KinectBackend()
{
    destructor(true);
}

KinectBackend & KinectBackend::getInstance( )
{
    static KinectBackend instance;

    return instance;
}

int KinectBackend::kinect_backend_main()
{
    if(freenect_init(&m_freenect_context_ptr, nullptr) < 0)
    {
        m_kinect_object_ptr->get_log() += "Initialisation error!!\n";

        KinectBackend::getInstance().kinect_backend_kill(true);

        return -1;
    }

    freenect_set_log_level(m_freenect_context_ptr, FREENECT_LOG_FATAL);

    freenect_select_subdevices(m_freenect_context_ptr, static_cast<freenect_device_flags>(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

    m_number_of_devices = freenect_num_devices(m_freenect_context_ptr);

    if(m_number_of_devices == 0)
    {
        m_kinect_object_ptr->get_log() += "No devices found!!\n";

        return -1;
    }
    else
    {
        m_kinect_object_ptr->get_log() += "Num devices found: " + to_string(m_number_of_devices) + "\n";
    }

    //Open the device.
    if(freenect_open_device(m_freenect_context_ptr, &m_freenect_device_ptr, 0) < 0)
    {
        m_kinect_object_ptr->get_log() += "Unable to connect with camera!!\n";

        return -1;
    }

    freenect_resolution resolution;

    if(m_resolution_small_bool)
    {
        resolution = FREENECT_RESOLUTION_LOW;
    }
    else
    {
        if(m_resolution_med_bool)
        {
            resolution = FREENECT_RESOLUTION_MEDIUM;
        }
        else
        {
            if(m_resolution_high_bool)
            {
                resolution = FREENECT_RESOLUTION_HIGH;
            }
            else
            {
                resolution = FREENECT_RESOLUTION_MEDIUM;
            }
        }
    }

    //Set depth and video modes
    if(freenect_set_depth_mode(m_freenect_device_ptr, freenect_find_depth_mode(resolution, FREENECT_DEPTH_MM)))
    {
        m_kinect_object_ptr->get_log() += "Unable to set depth mode!!\n";

        return -1;
    }

    if (freenect_set_video_mode(m_freenect_device_ptr, freenect_find_video_mode(resolution, FREENECT_VIDEO_RGB)))
    {
        m_kinect_object_ptr->get_log() += "Unable to set video mode!!\n";

        return -1;
    }

    switch(resolution)
    {
    case FREENECT_RESOLUTION_LOW:

        m_kinect_object_ptr->get_resolution()[0] = 320;
        m_kinect_object_ptr->get_resolution()[1] = 240;

        break;

    case FREENECT_RESOLUTION_MEDIUM:

        m_kinect_object_ptr->get_resolution()[0] = 640;
        m_kinect_object_ptr->get_resolution()[1] = 480;

        break;

    case FREENECT_RESOLUTION_HIGH:

        m_kinect_object_ptr->get_resolution()[0] = 1280;
        m_kinect_object_ptr->get_resolution()[1] = 1024;

        break;

    default:

        m_kinect_object_ptr->get_resolution()[0] = 0;
        m_kinect_object_ptr->get_resolution()[1] = 0;

        break;
    }

    m_kinect_object_ptr->get_depth() = vector<unsigned short>(0, 0);
    m_kinect_object_ptr->get_video() = vector<unsigned char>(0, 0);

    m_kinect_object_ptr->get_depth() = vector<unsigned short>((m_kinect_object_ptr->get_resolution()[0] * m_kinect_object_ptr->get_resolution()[1]), 0);
    m_kinect_object_ptr->get_video() = vector<unsigned char>(((m_kinect_object_ptr->get_resolution()[0] * m_kinect_object_ptr->get_resolution()[1]) * 3), 0);

    //Set frame callback
    freenect_set_depth_callback(m_freenect_device_ptr, KinectBackend::depth_callback);
    freenect_set_video_callback(m_freenect_device_ptr, KinectBackend::video_callback);

    if(freenect_start_depth(m_freenect_device_ptr))
    {
        m_kinect_object_ptr->get_log() += "Unable to start depth!!\n";

        return -1;
    }

    if(freenect_start_video(m_freenect_device_ptr))
    {
        m_kinect_object_ptr->get_log() += "Unable to start video!!\n";

        return -1;
    }

    set_device_camera_tilt(m_reset_camera_tilt);

    freenect_set_led(m_freenect_device_ptr, LED_RED);

    m_kinect_object_ptr->get_log() += "Connection established!!\n";

    return 1;
}

int KinectBackend::kinect_backend_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KinectBackend::update()
{
    freenect_process_events(m_freenect_context_ptr);

    return 1;
}

int KinectBackend::destructor(bool hard)
{
    if(hard)
    {
        if(m_kinect_object_ptr != nullptr)
        {
            m_kinect_object_ptr = nullptr;
        }
    }

    if(m_freenect_device_ptr != nullptr)
    {
        freenect_stop_depth(m_freenect_device_ptr);
        freenect_stop_video(m_freenect_device_ptr);
        freenect_close_device(m_freenect_device_ptr);

        m_freenect_device_ptr = nullptr;
    }

    if(m_freenect_context_ptr != nullptr)
    {
        freenect_shutdown(m_freenect_context_ptr);

        m_freenect_context_ptr = nullptr;
    }

    if(m_current_tilt_state_ptr != nullptr)
    {
        m_current_tilt_state_ptr = nullptr;
    }

    return 1;
}

int KinectBackend::set_current_camera_tilt_with_angle(double angle)
{
    update_tilt_state();

    freenect_tilt_status_code tilt_state = freenect_get_tilt_status(m_current_tilt_state_ptr);

    switch(tilt_state)
    {
    case freenect_tilt_status_code::TILT_STATUS_MOVING:

        m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_LIMIT:

        if(get_device_camera_tilt() > 0.0)
        {
            if(angle < 0)
            {
                set_device_camera_tilt(angle);
            }
            else
            {
                m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";
            }
        }
        else
        {
            if(angle > 0)
            {
                set_device_camera_tilt(angle);
            }
            else
            {
                m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";
            }
        }

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_STOPPED:

        set_device_camera_tilt(angle);

        return 1;
    }

    m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";

    set_device_camera_tilt(0.0);

    return -1;
}

int KinectBackend::set_current_camera_tilt_with_increment(double increment)
{
    update_tilt_state();

    freenect_tilt_status_code tilt_state = freenect_get_tilt_status(m_current_tilt_state_ptr);

    double current_camera_tilt = get_device_camera_tilt();

    switch(tilt_state)
    {
    case freenect_tilt_status_code::TILT_STATUS_MOVING:

        m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_LIMIT:

        if(current_camera_tilt > 0.0)
        {
            if(increment < 0.0)
            {
                set_device_camera_tilt(static_cast<int>(current_camera_tilt + (m_increment * increment)));
            }
            else
            {
                m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";
            }
        }
        else
        {
            if(increment > 0.0)
            {
                set_device_camera_tilt(static_cast<int>(current_camera_tilt + (m_increment * increment)));
            }
            else
            {
                m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";
            }
        }

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_STOPPED:

        set_device_camera_tilt(current_camera_tilt + (m_increment * increment));

        return 1;
    }

    m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";

    set_device_camera_tilt(0.0);

    return -1;
}

void KinectBackend::depth_callback(freenect_device *freenect_device_ptr, void *data, unsigned int timestamp)
{
    if(freenect_device_ptr)
    {

    }

    if(KinectBackend::getInstance().get_depth_image_bool())
    {
        unsigned short *depth_ptr = static_cast<unsigned short *>(data);

        for(unsigned long i = 0; i < KinectBackend::getInstance().m_kinect_object_ptr->get_depth().size(); ++i)
        {
            KinectBackend::getInstance().m_kinect_object_ptr->get_depth()[i] = depth_ptr[i];
        }

        KinectBackend::getInstance().m_kinect_object_ptr->set_epoch_timestamp(system_clock::now());
        KinectBackend::getInstance().m_kinect_object_ptr->set_kinect_timestamp(timestamp);
    }

    KinectBackend::getInstance().m_kinect_object_ptr->get_log() += "-> depth: " + to_string(timestamp) + "\n";

    KinectBackend::getInstance().m_kinect_object_ptr->get_flags()[0] = true;
}

void KinectBackend::video_callback(freenect_device *freenect_device_ptr, void *data_ptr, unsigned int timestamp)
{
    if(freenect_device_ptr)
    {

    }

    if(KinectBackend::getInstance().get_rgb_image_bool())
    {
        unsigned char *video_ptr = static_cast<unsigned char *>(data_ptr);

        for(unsigned long i = 0; i < KinectBackend::getInstance().m_kinect_object_ptr->get_video().size(); ++i)
        {
            KinectBackend::getInstance().m_kinect_object_ptr->get_video()[i] = video_ptr[i];
        }

        KinectBackend::getInstance().m_kinect_object_ptr->set_epoch_timestamp(system_clock::now());
        KinectBackend::getInstance().m_kinect_object_ptr->set_kinect_timestamp(timestamp);
    }

    KinectBackend::getInstance().m_kinect_object_ptr->get_log() += "-> video: "  + to_string(timestamp) + "\n";

    KinectBackend::getInstance().m_kinect_object_ptr->get_flags()[1] = true;
}

int KinectBackend::update_tilt_state()
{
    if (freenect_update_tilt_state(m_freenect_device_ptr) < 0)
    {
        m_kinect_object_ptr->get_log() += "Unable to retrieve tilt status!!\n";

        return -1;
    }

    m_current_tilt_state_ptr = freenect_get_tilt_state(m_freenect_device_ptr);

    return 1;
}

inline double KinectBackend::get_device_camera_tilt()
{
    return freenect_get_tilt_degs(m_current_tilt_state_ptr);
}

int KinectBackend::set_device_camera_tilt(double new_camera_tilt)
{
    if(new_camera_tilt < 25.0 && new_camera_tilt > -25.0)
    {
        freenect_set_tilt_degs(m_freenect_device_ptr, new_camera_tilt);

        m_kinect_object_ptr->set_current_camera_tilt(new_camera_tilt);

        m_kinect_object_ptr->get_log() += "Camera tilted to: " + to_string(new_camera_tilt) + "\n";
    }
    else
    {
        m_kinect_object_ptr->get_log() += "Unable to tilt the camera!!\n";
    }

    return 1;
}
