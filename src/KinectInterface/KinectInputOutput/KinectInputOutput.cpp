#include "src/include/KinectInputOutput.h"

KinectInputOutput::KinectInputOutput():
    m_kinect_object_ptr(nullptr),
    m_frames_recorded(0)
{

}

KinectInputOutput::~KinectInputOutput()
{
    destructor(true);
}

KinectInputOutput::KinectInputOutput(KinectInputOutput &kinect_input_output_ref):
    m_kinect_object_ptr(kinect_input_output_ref.get_kinect_object()),
    m_frames_recorded(kinect_input_output_ref.get_frames_recorded())
{

}

KinectInputOutput & KinectInputOutput::operator = (KinectInputOutput &kinect_input_output_ref)
{
    m_kinect_object_ptr = kinect_input_output_ref.get_kinect_object();
    m_frames_recorded = kinect_input_output_ref.get_frames_recorded();

    return *this;
}

KinectInputOutput::KinectInputOutput(KinectInputOutput &&kinect_input_output_ref_ref):
    m_kinect_object_ptr(kinect_input_output_ref_ref.get_kinect_object()),
    m_frames_recorded(kinect_input_output_ref_ref.get_frames_recorded())
{

}

KinectInputOutput & KinectInputOutput::operator = (KinectInputOutput &&kinect_input_output_ref_ref)
{
    m_kinect_object_ptr = kinect_input_output_ref_ref.get_kinect_object();
    m_frames_recorded = kinect_input_output_ref_ref.get_frames_recorded();

    return *this;
}

int KinectInputOutput::kinect_input_output_main()
{
    if(m_kinect_object_ptr->get_flags().at(0))
    {
        write_depth_to_file();
        write_video_to_file();

        ++m_frames_recorded;

        m_kinect_object_ptr->get_flags().at(0) = false;
        m_kinect_object_ptr->get_flags().at(1) = false;

        return 1;
    }
    else
    {
        return -1;
    }
}

int KinectInputOutput::kinect_input_output_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KinectInputOutput::write_depth_to_file()
{
    ofstream depth_stream;
    depth_stream.open("depth_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".bin", ios::out | ios::binary);

    for(int i = 0; i < m_kinect_object_ptr->get_resolution().at(0) * m_kinect_object_ptr->get_resolution().at(1); ++i)
    {
        depth_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_depth().at(i)), sizeof(unsigned short));
    }

    depth_stream.close();

    m_kinect_object_ptr->get_log() += "<- depth: " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n";

    return 1;
}

int KinectInputOutput::write_video_to_file()
{
    ofstream video_stream;
    video_stream.open("video_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".bin", ios::out | ios::binary);

    for(int i = 0; i < (m_kinect_object_ptr->get_resolution().at(0) * m_kinect_object_ptr->get_resolution().at(1)) * 3; ++i)
    {
        video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(i)), sizeof(unsigned char));
    }

    video_stream.close();

    m_kinect_object_ptr->get_log() += "<- video: " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n";

    return 1;
}

int KinectInputOutput::destructor(bool hard)
{
    if(m_kinect_object_ptr != nullptr)
    {
        m_kinect_object_ptr = nullptr;
    }

    return 1;
}
