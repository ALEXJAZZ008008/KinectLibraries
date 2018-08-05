#include "src/include/KinectInputOutput.h"

KinectInputOutput::KinectInputOutput():
    m_kinect_object_ptr(nullptr),
    m_output_path(""),
    m_frames_recorded(0),
    m_depth_image_bool(false),
    m_rgb_image_bool(false)
{

}

KinectInputOutput::~KinectInputOutput()
{
    destructor(true);
}

KinectInputOutput::KinectInputOutput(KinectInputOutput &kinect_input_output_ref):
    m_kinect_object_ptr(kinect_input_output_ref.get_kinect_object_ptr()),
    m_output_path(kinect_input_output_ref.get_output_path()),
    m_frames_recorded(kinect_input_output_ref.get_frames_recorded()),
    m_depth_image_bool(kinect_input_output_ref.get_depth_image_bool()),
    m_rgb_image_bool(kinect_input_output_ref.get_rgb_image_bool())
{

}

KinectInputOutput & KinectInputOutput::operator = (KinectInputOutput &kinect_input_output_ref)
{
    m_kinect_object_ptr = kinect_input_output_ref.get_kinect_object_ptr();
    m_output_path = kinect_input_output_ref.get_output_path();
    m_frames_recorded = kinect_input_output_ref.get_frames_recorded();
    m_depth_image_bool = kinect_input_output_ref.get_depth_image_bool();
    m_rgb_image_bool = kinect_input_output_ref.get_rgb_image_bool();

    return *this;
}

KinectInputOutput::KinectInputOutput(KinectInputOutput &&kinect_input_output_ref_ref):
    m_kinect_object_ptr(kinect_input_output_ref_ref.get_kinect_object_ptr()),
    m_output_path(kinect_input_output_ref_ref.get_output_path()),
    m_frames_recorded(kinect_input_output_ref_ref.get_frames_recorded()),
    m_depth_image_bool(kinect_input_output_ref_ref.get_depth_image_bool()),
    m_rgb_image_bool(kinect_input_output_ref_ref.get_rgb_image_bool())
{

}

KinectInputOutput & KinectInputOutput::operator = (KinectInputOutput &&kinect_input_output_ref_ref)
{
    m_kinect_object_ptr = kinect_input_output_ref_ref.get_kinect_object_ptr();
    m_output_path = kinect_input_output_ref_ref.get_output_path();
    m_frames_recorded = kinect_input_output_ref_ref.get_frames_recorded();
    m_depth_image_bool = kinect_input_output_ref_ref.get_depth_image_bool();
    m_rgb_image_bool = kinect_input_output_ref_ref.get_rgb_image_bool();

    return *this;
}

int KinectInputOutput::kinect_input_output_main()
{
    if(m_kinect_object_ptr->get_flags()[0] || m_kinect_object_ptr->get_flags()[1])
    {
        if(m_depth_image_bool)
        {
            if(m_kinect_object_ptr->get_flags()[0])
            {
                write_header_to_file("depth", write_depth_to_file(), "u", "16", "1");

                ++m_frames_recorded;

                m_kinect_object_ptr->get_flags()[0] = false;
            }
        }

        if(m_rgb_image_bool && m_kinect_object_ptr->get_flags()[1])
        {
            if(m_kinect_object_ptr->get_flags()[1])
            {
                write_header_to_file("video", write_video_to_file(), "u", "24", "3");

                ++m_frames_recorded;

                m_kinect_object_ptr->get_flags()[1] = false;
            }
        }
    }

    return 1;
}

int KinectInputOutput::kinect_input_output_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KinectInputOutput::write_header_to_file(string path_options, string object_path, string data_type, string data_size, string data_dimensions)
{
    ofstream header_stream(m_output_path + "/header_" + path_options + "_" + to_string(m_kinect_object_ptr->get_kinect_timestamp()) + ".kpclp", ios::out);

    header_stream << "kpclp_header_version=0.1" << endl;
    header_stream << "data_type=" << data_type << endl;
    header_stream << "data_size=" << data_size << endl;
    header_stream << "data_dimensions=" << data_dimensions << endl;
    header_stream << "data_resolution=" << to_string(m_kinect_object_ptr->get_resolution()[0]) << "," << to_string(m_kinect_object_ptr->get_resolution()[1]) << endl;
    header_stream << "data_path=" << object_path << endl;
    header_stream << "epoch_timestamp=" << to_string(m_kinect_object_ptr->get_epoch_timestamp().time_since_epoch().count()) << endl;
    header_stream << "kinect_timestamp=" << to_string(m_kinect_object_ptr->get_kinect_timestamp()) << endl;
    header_stream << "kpclp_header_status=end" << endl;

    header_stream.flush();
    header_stream.close();

    m_kinect_object_ptr->get_log() += "<- header_" + path_options + ": " + to_string(m_kinect_object_ptr->get_kinect_timestamp()) + "\n";

    return 1;
}

string KinectInputOutput::write_depth_to_file()
{
    string output_path = m_output_path + "/depth_" + to_string(m_kinect_object_ptr->get_kinect_timestamp()) + ".bin";

    ofstream depth_stream(output_path, ios::out | ios::binary);

    for(unsigned long i = 0; i < m_kinect_object_ptr->get_depth().size(); ++i)
    {
        depth_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_depth()[i]), sizeof(unsigned short));
    }

    depth_stream.flush();
    depth_stream.close();

    m_kinect_object_ptr->get_log() += "<- depth: " + to_string(m_kinect_object_ptr->get_kinect_timestamp()) + "\n";

    return output_path;
}

string KinectInputOutput::write_video_to_file()
{
    string output_path = m_output_path + "/video_" + to_string(m_kinect_object_ptr->get_kinect_timestamp()) + ".bin";

    ofstream video_stream(output_path, ios::out | ios::binary);

    for(unsigned long i = 0; i < m_kinect_object_ptr->get_video().size(); ++i)
    {
        video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video()[i]), sizeof(unsigned char));
    }

    video_stream.flush();
    video_stream.close();

    m_kinect_object_ptr->get_log() += "<- video: " + to_string(m_kinect_object_ptr->get_kinect_timestamp()) + "\n";

    return output_path;
}

int KinectInputOutput::destructor(bool hard)
{
    if(hard)
    {

    }

    if(m_kinect_object_ptr != nullptr)
    {
        m_kinect_object_ptr = nullptr;
    }

    return 1;
}
