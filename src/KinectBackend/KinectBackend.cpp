#include "src/include/KinectBackend.h"

KinectBackend::KinectBackend():
    m_fctx_ptr(nullptr),
    m_fdev_ptr(nullptr),
    m_current_tilt_state_ptr(nullptr),
    m_depth(),
    m_video_ptr((unsigned char *)new unsigned char[640 * 480 * 3]),
    m_gamma(),
    m_output(""),
    m_depth_output(""),
    m_video_output(""),
    m_stored_camera_tilt(0.0),
    m_increment(2.0),
    m_num_devices(0)
{
    for (int i = 0; i < 2048; i++)
    {
        float v = i / 2048.0f;
        v = powf(v, 3) * 6;

        m_gamma[i] = v * 6 * 256;
    }
}

KinectBackend::~KinectBackend()
{
    destructor();
}

KinectBackend & KinectBackend::getInstance( )
{
    static KinectBackend instance;

    return instance;
}

int KinectBackend::update()
{
    freenect_process_events(m_fctx_ptr);

    return 1;
}

int KinectBackend::kinect_backend_main()
{
    if(freenect_init(&m_fctx_ptr, NULL) < 0)
    {
        m_output += "Initialisation error!!\n";

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
    if(freenect_set_depth_mode(m_fdev_ptr, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT)))
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

    if(freenect_start_depth(m_fdev_ptr))
    {
        m_output += "Unable to start depth!!\n";

        return -1;
    }

    if(freenect_start_video(m_fdev_ptr))
    {
        m_output += "Unable to start video!!\n";

        return -1;
    }

    set_device_camera_tilt(0.0);

    freenect_set_led(m_fdev_ptr, LED_RED);

    m_output += "Connection established!!\n";

    return 1;
}

int KinectBackend::kinect_backend_kill()
{
    set_device_camera_tilt(0.0);

    destructor();

    return 1;
}

string KinectBackend::get_output()
{
    string output = m_output;

    m_output = "";

    return output;
}

string KinectBackend::get_depth_output()
{
    string output = m_depth_output;

    m_depth_output = "";

    return output;
}

string KinectBackend::get_video_output()
{
    string output = m_video_output;

    m_video_output = "";

    return output;
}

int KinectBackend::destructor()
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

    if(m_current_tilt_state_ptr != nullptr)
    {
        delete m_current_tilt_state_ptr;

        m_current_tilt_state_ptr = nullptr;
    }

    if(m_video_ptr != nullptr)
    {
        //delete[] m_video_ptr;

        m_video_ptr = nullptr;
    }

    m_output += "Connection terminated!!\n";

    return 1;
}

int KinectBackend::set_stored_camera_tilt(double increment)
{
    update_tilt_state();

    freenect_tilt_status_code tilt_state = freenect_get_tilt_status(m_current_tilt_state_ptr);

    double current_camera_tilt = get_device_camera_tilt();

    switch(tilt_state)
    {
    case freenect_tilt_status_code::TILT_STATUS_MOVING:

        m_output += "Unable to tilt the camera!!\n";

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
                m_output += "Unable to tilt the camera!!\n";
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
                m_output += "Unable to tilt the camera!!\n";
            }
        }

        return -1;

    case freenect_tilt_status_code::TILT_STATUS_STOPPED:

        set_device_camera_tilt(current_camera_tilt + (m_increment * increment));

        return 1;

    default:

        m_output += "Unable to tilt the camera!!\n";

        set_device_camera_tilt(0.0);

        return -1;
    }

    return -1;
}

void KinectBackend::depth_callback(freenect_device *fdev_ptr, void *data, unsigned int timestamp)
{
    unsigned short *depth = (unsigned short *)data;

    for (int i = 0; i < 640 * 480; i++)
    {
        int pval = KinectBackend::getInstance().m_gamma[depth[i]];

        int lb = pval & 0xff;

        switch (pval >> 8)
        {
        case 0:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 255;
            KinectBackend::getInstance().m_depth[3 * i + 1] = 255 - lb;
            KinectBackend::getInstance().m_depth[3 * i + 2] = 255 - lb;

            break;

        case 1:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 255;
            KinectBackend::getInstance().m_depth[3 * i + 1] = lb;
            KinectBackend::getInstance().m_depth[3 * i + 2] = 0;

            break;

        case 2:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 255 - lb;
            KinectBackend::getInstance().m_depth[3 * i + 1] = 255;
            KinectBackend::getInstance().m_depth[3 * i + 2] = 0;

            break;

        case 3:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 0;
            KinectBackend::getInstance().m_depth[3 * i + 1] = 255;
            KinectBackend::getInstance().m_depth[3 * i + 2] = lb;

            break;

        case 4:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 0;
            KinectBackend::getInstance().m_depth[3 * i + 1] = 255 - lb;
            KinectBackend::getInstance().m_depth[3 * i + 2] = 255;

            break;

        case 5:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 0;
            KinectBackend::getInstance().m_depth[3 * i + 1] = 0;
            KinectBackend::getInstance().m_depth[3 * i + 2] = 255 - lb;

            break;

        default:

            KinectBackend::getInstance().m_depth[3 * i + 0] = 0;
            KinectBackend::getInstance().m_depth[3 * i + 1] = 0;
            KinectBackend::getInstance().m_depth[3 * i + 2] = 0;

            break;
        }

        KinectBackend::getInstance().append_depth_output(to_string(KinectBackend::getInstance().m_depth[3 * i + 0]) + " " +
                to_string(KinectBackend::getInstance().m_depth[3 * i + 1]) + " " +
                to_string(KinectBackend::getInstance().m_depth[3 * i + 2]) + "\n");

        //KinectBackend::getInstance().append_video_output(to_string(KinectBackend::getInstance().m_video_ptr[3 * i + 0]) + " " +
        //        to_string(KinectBackend::getInstance().m_video_ptr[3 * i + 1]) + " " +
        //        to_string(KinectBackend::getInstance().m_video_ptr[3 * i + 2]) + "\n");
    }

    KinectBackend::getInstance().append_depth_output("Received depth frame at " + to_string(timestamp) + "\n");
}

void KinectBackend::video_callback(freenect_device *fdev_ptr, void *data, unsigned int timestamp)
{
    KinectBackend::getInstance().m_video_ptr = (unsigned char *)data;

    KinectBackend::getInstance().append_video_output("Received video frame at "  + to_string(timestamp) + "\n");
}

int KinectBackend::update_tilt_state()
{
    if (freenect_update_tilt_state(m_fdev_ptr) < 0)
    {
        m_output += "Unable to retrieve tilt status!!\n";

        return -1;
    }

    m_current_tilt_state_ptr = freenect_get_tilt_state(m_fdev_ptr);

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
        freenect_set_tilt_degs(m_fdev_ptr, new_camera_tilt);

        m_stored_camera_tilt = new_camera_tilt;

        m_output += "Camera tilted to: " + to_string(m_stored_camera_tilt) + "\n";
    }
    else
    {
        m_output += "Unable to tilt the camera!!\n";
    }

    return 1;
}
