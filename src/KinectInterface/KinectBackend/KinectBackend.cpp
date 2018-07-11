#include "src/include/KinectBackend.h"

KinectBackend::KinectBackend():
    m_kinect_object_ptr(nullptr),
    m_freenect_context_ptr(nullptr),
    m_freenect_device_ptr(nullptr),
    m_current_tilt_state_ptr(nullptr),
    m_message(""),
    m_depth_message(""),
    m_video_message(""),
    m_reset_camera_tilt(0.0),
    m_stored_camera_tilt(0.0),
    m_increment(2.0),
    m_num_devices(0)
{

}

KinectBackend::KinectBackend(KinectObject *kinect_object_ptr):
    m_kinect_object_ptr(kinect_object_ptr),
    m_freenect_context_ptr(nullptr),
    m_freenect_device_ptr(nullptr),
    m_current_tilt_state_ptr(nullptr),
    m_message(""),
    m_depth_message(""),
    m_video_message(""),
    m_reset_camera_tilt(0.0),
    m_stored_camera_tilt(0.0),
    m_increment(2.0),
    m_num_devices(0)
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

string KinectBackend::get_message()
{
    string message = m_message;

    m_message = "";

    return message;
}

string KinectBackend::get_depth_message()
{
    string depth_message = m_depth_message;

    m_depth_message = "";

    return depth_message;
}

string KinectBackend::get_video_message()
{
    string video_message = m_video_message;

    m_video_message = "";

    return video_message;
}

int KinectBackend::set_stored_camera_tilt(double increment)
{
    update_tilt_state();

    freenect_tilt_status_code tilt_state = freenect_get_tilt_status(m_current_tilt_state_ptr);

    double current_camera_tilt = get_device_camera_tilt();

    switch(tilt_state)
    {
    case freenect_tilt_status_code::TILT_STATUS_MOVING:

        m_message += "Unable to tilt the camera!!\n";

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_LIMIT:

        if(current_camera_tilt > 0)
        {
            if(increment < 0)
            {
                set_device_camera_tilt(current_camera_tilt + (m_increment * increment));
            }
            else
            {
                m_message += "Unable to tilt the camera!!\n";
            }
        }
        else
        {
            if(increment > 0)
            {
                set_device_camera_tilt(current_camera_tilt + (m_increment * increment));
            }
            else
            {
                m_message += "Unable to tilt the camera!!\n";
            }
        }

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_STOPPED:

        set_device_camera_tilt(current_camera_tilt + (m_increment * increment));

        return 1;

    default:

        m_message += "Unable to tilt the camera!!\n";

        set_device_camera_tilt(0.0);

        return -1;
    }

    return -1;
}

int KinectBackend::kinect_backend_main()
{
    if(freenect_init(&m_freenect_context_ptr, NULL) < 0)
    {
        m_message += "Initialisation error!!\n";

        KinectBackend::getInstance().~KinectBackend();

        return -1;
    }

    freenect_set_log_level(m_freenect_context_ptr, FREENECT_LOG_DEBUG);
    freenect_select_subdevices(m_freenect_context_ptr, static_cast<freenect_device_flags>(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

    m_num_devices = freenect_num_devices(m_freenect_context_ptr);

    if (m_num_devices == 0)
    {
        m_message += "No devices found!!\n";

        return -1;
    }
    else
    {
        m_message += "Num devices found: " + to_string(m_num_devices) + "\n";
    }

    //Open the device.
    if (freenect_open_device(m_freenect_context_ptr, &m_freenect_device_ptr, 0) < 0)
    {
        m_message += "Unable to connect with camera!!\n";

        return -1;
    }

    freenect_resolution resolution = FREENECT_RESOLUTION_MEDIUM;

    //Set depth and video modes
    if(freenect_set_depth_mode(m_freenect_device_ptr, freenect_find_depth_mode(resolution, FREENECT_DEPTH_MM)))
    {
        m_message += "Unable to set depth mode!!\n";

        return -1;
    }

    if (freenect_set_video_mode(m_freenect_device_ptr, freenect_find_video_mode(resolution, FREENECT_VIDEO_RGB)))
    {
        m_message += "Unable to set video mode!!\n";

        return -1;
    }

    switch(resolution)
    {
    case FREENECT_RESOLUTION_LOW:

        m_kinect_object_ptr->get_resolution().at(0) = 320;
        m_kinect_object_ptr->get_resolution().at(1) = 240;

        break;

    case FREENECT_RESOLUTION_MEDIUM:

        m_kinect_object_ptr->get_resolution().at(0) = 640;
        m_kinect_object_ptr->get_resolution().at(1) = 480;

        break;

    case FREENECT_RESOLUTION_HIGH:

        m_kinect_object_ptr->get_resolution().at(0) = 1280;
        m_kinect_object_ptr->get_resolution().at(1) = 1024;

        break;

    default:

        m_kinect_object_ptr->get_resolution().at(0) = 0;
        m_kinect_object_ptr->get_resolution().at(1) = 0;

        break;
    }

    //Set frame callback
    freenect_set_depth_callback(m_freenect_device_ptr, KinectBackend::depth_callback);
    freenect_set_video_callback(m_freenect_device_ptr, KinectBackend::video_callback);

    if(freenect_start_depth(m_freenect_device_ptr))
    {
        m_message += "Unable to start depth!!\n";

        return -1;
    }

    if(freenect_start_video(m_freenect_device_ptr))
    {
        m_message += "Unable to start video!!\n";

        return -1;
    }

    set_device_camera_tilt(m_reset_camera_tilt);

    freenect_set_led(m_freenect_device_ptr, LED_RED);

    m_message += "Connection established!!\n";

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

    if(m_kinect_object_ptr->get_got_depth())
    {
        calculate_point_cloud();
    }

    return 1;
}

int KinectBackend::destructor(bool hard)
{
    if(m_freenect_device_ptr != nullptr)
    {
        set_device_camera_tilt(m_reset_camera_tilt);
    }

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
        delete m_current_tilt_state_ptr;

        m_current_tilt_state_ptr = nullptr;
    }

    m_message += "Connection terminated!!\n";

    return 1;
}

void KinectBackend::depth_callback(freenect_device *m_freenect_device_ptr, void *data, unsigned int timestamp)
{
    unsigned short *depth_ptr = (unsigned short *)data;

    for(unsigned short i = 0; i < KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(1); ++i)
    {
        for(unsigned short j = 0; j < KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(0); ++j)
        {
            KinectBackend::getInstance().m_kinect_object_ptr->get_depth().at(j).at(i) =
                    depth_ptr[(KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(0) * i) + j];
        }
    }

    KinectBackend::getInstance().m_kinect_object_ptr->set_timestamp(timestamp);

    KinectBackend::getInstance().m_kinect_object_ptr->set_got_depth(true);

    KinectBackend::getInstance().append_depth_message("Received depth frame at " + to_string(timestamp) + "\n");
}

void KinectBackend::video_callback(freenect_device *m_freenect_device_ptr, void *data_ptr, unsigned int timestamp)
{
    unsigned char *video_ptr = static_cast<unsigned char *>(data_ptr);

    for(unsigned short i = 0; i < KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(1); ++i)
    {
        for(unsigned short j = 0; j < KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(0); ++j)
        {
            KinectBackend::getInstance().m_kinect_object_ptr->get_video().at(j).at(i).at(0) =
                    video_ptr[((KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(0) * i) + j) * 3];

            KinectBackend::getInstance().m_kinect_object_ptr->get_video().at(j).at(i).at(1) =
                    video_ptr[(((KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(0) * i) + j) * 3) + 1];

            KinectBackend::getInstance().m_kinect_object_ptr->get_video().at(j).at(i).at(2) =
                    video_ptr[(((KinectBackend::getInstance().m_kinect_object_ptr->get_resolution().at(0) * i) + j) * 3) + 2];
        }
    }

    KinectBackend::getInstance().m_kinect_object_ptr->set_timestamp(timestamp);

    KinectBackend::getInstance().m_kinect_object_ptr->set_got_video(true);

    KinectBackend::getInstance().append_video_message("Received video frame at "  + to_string(timestamp) + "\n");
}

int KinectBackend::calculate_point_cloud()
{
    vector<float>::iterator vector_iterator = m_kinect_object_ptr->get_point_cloud().begin();

    for(unsigned short i = 0; i < m_kinect_object_ptr->get_resolution().at(1); ++i)
    {
        for(unsigned short j = 0; j < m_kinect_object_ptr->get_resolution().at(0); ++j)
        {
            *vector_iterator = (j - (m_kinect_object_ptr->get_resolution().at(1) / 2.0f)) * (m_kinect_object_ptr->get_depth().at(j).at(i) - 10) * 0.0021f;
            ++vector_iterator;

            *vector_iterator = (i - (m_kinect_object_ptr->get_resolution().at(0) / 2.0f)) * (m_kinect_object_ptr->get_depth().at(j).at(i) - 10) * 0.0021f;
            ++vector_iterator;

            *vector_iterator = m_kinect_object_ptr->get_depth().at(j).at(i);
            ++vector_iterator;
        }
    }

    return 1;
}

int KinectBackend::update_tilt_state()
{
    if (freenect_update_tilt_state(m_freenect_device_ptr) < 0)
    {
        m_message += "Unable to retrieve tilt status!!\n";

        return -1;
    }

    m_current_tilt_state_ptr = freenect_get_tilt_state(m_freenect_device_ptr);

    return 1;
}

double KinectBackend::get_device_camera_tilt()
{
    return freenect_get_tilt_degs(m_current_tilt_state_ptr);
}

int KinectBackend::set_device_camera_tilt(double new_camera_tilt)
{
    if(new_camera_tilt < 25.0 && new_camera_tilt > -25.0)
    {
        freenect_set_tilt_degs(m_freenect_device_ptr, new_camera_tilt);

        m_stored_camera_tilt = new_camera_tilt;

        m_message += "Camera tilted to: " + to_string(m_stored_camera_tilt) + "\n";
    }
    else
    {
        m_message += "Unable to tilt the camera!!\n";
    }

    return 1;
}
