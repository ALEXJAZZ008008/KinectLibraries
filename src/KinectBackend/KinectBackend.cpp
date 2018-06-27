#include "src/include/KinectBackend.h"

KinectBackend::KinectBackend():
    m_fctx_ptr(nullptr),
    m_fdev_ptr(nullptr),
    m_current_tilt_state_ptr(nullptr),
    m_gamma(),
    m_depth(),
    m_video(),
    m_output(""),
    m_depth_output(""),
    m_video_output(""),
    m_resolution(),
    m_max_value(0),
    m_reset_camera_tilt(-20.0),
    m_stored_camera_tilt(0.0),
    m_increment(2.0),
    m_num_devices(0)
{

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
    freenect_select_subdevices(m_fctx_ptr, static_cast<freenect_device_flags>(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

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

    freenect_resolution resolution = FREENECT_RESOLUTION_MEDIUM;
    freenect_depth_format depth_format = FREENECT_DEPTH_MM;

    //Set depth and video modes
    if(freenect_set_depth_mode(m_fdev_ptr, freenect_find_depth_mode(resolution, depth_format)))
    {
        m_output += "Unable to set depth mode!!\n";

        return -1;
    }

    if (freenect_set_video_mode(m_fdev_ptr, freenect_find_video_mode(resolution, FREENECT_VIDEO_RGB)))
    {
        m_output += "Unable to set video mode!!\n";

        return -1;
    }

    switch(resolution)
    {
    case FREENECT_RESOLUTION_LOW:

        m_resolution[0] = 320;
        m_resolution[1] = 240;

        break;

    case FREENECT_RESOLUTION_MEDIUM:

        m_resolution[0] = 640;
        m_resolution[1] = 480;

        break;

    case FREENECT_RESOLUTION_HIGH:

        m_resolution[0] = 1280;
        m_resolution[1] = 1024;

        break;

    default:

        m_resolution[0] = 0;
        m_resolution[1] = 0;

        break;
    }

    switch(depth_format)
    {
    case FREENECT_DEPTH_11BIT:
    case FREENECT_DEPTH_10BIT:
    case FREENECT_DEPTH_11BIT_PACKED:
    case FREENECT_DEPTH_10BIT_PACKED:

        m_max_value = 10000.0;

        break;

    case FREENECT_DEPTH_REGISTERED:
    case FREENECT_DEPTH_MM:

        m_max_value = 2048.0;

        break;

    default:

        m_max_value = 0.0;

        break;
    }

    for (int i = 0; i < m_max_value; i++)
    {
        double v = i / m_max_value;
        v = pow(v, 3) * 6;

        m_gamma[i] = v * 1536;
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

    set_device_camera_tilt(m_reset_camera_tilt);

    freenect_set_led(m_fdev_ptr, LED_RED);

    m_output += "Connection established!!\n";

    return 1;
}

int KinectBackend::kinect_backend_kill()
{
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
    set_device_camera_tilt(m_reset_camera_tilt);

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

    ofstream depth0_stream;
    depth0_stream.open("depth0_" + to_string(timestamp) + ".bin", ios::out | ios::binary);

    ofstream depth1_stream;
    depth1_stream.open("depth1_" + to_string(timestamp) + ".bin", ios::out | ios::binary);

    ofstream depth2_stream;
    depth2_stream.open("depth2_" + to_string(timestamp) + ".bin", ios::out | ios::binary);

    ofstream depth_pval_stream;
    depth_pval_stream.open("depth_pval_" + to_string(timestamp) + ".bin", ios::out | ios::binary);

    for(int i = 0; i < KinectBackend::getInstance().m_resolution[1]; i++)
    {
        for(int j = 0; j < KinectBackend::getInstance().m_resolution[0]; j++)
        {
            //int pval = KinectBackend::getInstance().m_gamma[depth[(KinectBackend::getInstance().m_resolution[0] * i) + j]];
            int pval = depth[(KinectBackend::getInstance().m_resolution[0] * i) + j];

            int lb = pval & 0xff;

            switch (pval >> 8)
            {
            case 0:

                KinectBackend::getInstance().m_depth[j][i][0] = 255;
                KinectBackend::getInstance().m_depth[j][i][1] = 255 - lb;
                KinectBackend::getInstance().m_depth[j][i][2] = 255 - lb;

                break;

            case 1:

                KinectBackend::getInstance().m_depth[j][i][0] = 255;
                KinectBackend::getInstance().m_depth[j][i][1] = lb;
                KinectBackend::getInstance().m_depth[j][i][2] = 0;

                break;

            case 2:

                KinectBackend::getInstance().m_depth[j][i][0] = 255 - lb;
                KinectBackend::getInstance().m_depth[j][i][1] = 255;
                KinectBackend::getInstance().m_depth[j][i][2] = 0;

                break;

            case 3:

                KinectBackend::getInstance().m_depth[j][i][0] = 0;
                KinectBackend::getInstance().m_depth[j][i][1] = 255;
                KinectBackend::getInstance().m_depth[j][i][2] = lb;

                break;

            case 4:

                KinectBackend::getInstance().m_depth[j][i][0] = 0;
                KinectBackend::getInstance().m_depth[j][i][1] = 255 - lb;
                KinectBackend::getInstance().m_depth[j][i][2] = 255;

                break;

            case 5:

                KinectBackend::getInstance().m_depth[j][i][0] = 0;
                KinectBackend::getInstance().m_depth[j][i][1] = 0;
                KinectBackend::getInstance().m_depth[j][i][2] = 255 - lb;

                break;

            default:

                KinectBackend::getInstance().m_depth[j][i][0] = 0;
                KinectBackend::getInstance().m_depth[j][i][1] = 0;
                KinectBackend::getInstance().m_depth[j][i][2] = 0;

                break;
            }

            depth0_stream.write(reinterpret_cast<char *>(&KinectBackend::getInstance().m_depth[j][i][0]), sizeof(unsigned short));
            depth1_stream.write(reinterpret_cast<char *>(&KinectBackend::getInstance().m_depth[j][i][1]), sizeof(unsigned short));
            depth2_stream.write(reinterpret_cast<char *>(&KinectBackend::getInstance().m_depth[j][i][2]), sizeof(unsigned short));
            depth_pval_stream.write(reinterpret_cast<char *>(&pval), sizeof(unsigned short));
        }
    }

    depth0_stream.close();
    depth1_stream.close();
    depth2_stream.close();
    depth_pval_stream.close();

    KinectBackend::getInstance().append_depth_output("Received depth frame at " + to_string(timestamp) + "\n");
}

void KinectBackend::video_callback(freenect_device *fdev_ptr, void *data_ptr, unsigned int timestamp)
{
    unsigned char *video_ptr = static_cast<unsigned char *>(data_ptr);

    unsigned char average = 0;

    ofstream video_bw_stream;
    video_bw_stream.open("video_bw_" + to_string(timestamp) + ".bin", ios::out | ios::binary);

    ofstream video_rgb_stream;
    video_rgb_stream.open("video_rgb_" + to_string(timestamp) + ".bin", ios::out | ios::binary);

    for(int i = 0; i < KinectBackend::getInstance().m_resolution[1]; i++)
    {
        for(int j = 0; j < KinectBackend::getInstance().m_resolution[0]; j++)
        {
            KinectBackend::getInstance().m_video[j][i][0] = video_ptr[((KinectBackend::getInstance().m_resolution[0] * i) + j) * 3];
            KinectBackend::getInstance().m_video[j][i][1] = video_ptr[(((KinectBackend::getInstance().m_resolution[0] * i) + j) * 3) + 1];
            KinectBackend::getInstance().m_video[j][i][2] = video_ptr[(((KinectBackend::getInstance().m_resolution[0] * i) + j) * 3) + 2];

            average = (KinectBackend::getInstance().m_video[j][i][0] +
                    KinectBackend::getInstance().m_video[j][i][0] +
                    KinectBackend::getInstance().m_video[j][i][0]) / 3;

            video_bw_stream.write(reinterpret_cast<char *>(&average), sizeof(unsigned char));

            video_rgb_stream.write(reinterpret_cast<char *>(&KinectBackend::getInstance().m_video[j][i][0]), sizeof(unsigned char));
            video_rgb_stream.write(reinterpret_cast<char *>(&KinectBackend::getInstance().m_video[j][i][1]), sizeof(unsigned char));
            video_rgb_stream.write(reinterpret_cast<char *>(&KinectBackend::getInstance().m_video[j][i][2]), sizeof(unsigned char));
        }
    }

    video_bw_stream.close();

    video_rgb_stream.close();

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
