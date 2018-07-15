#include "src/include/KinectInputOutput.h"

KinectInputOutput::KinectInputOutput():
    m_kinect_object_ptr(nullptr)
{

}

KinectInputOutput::KinectInputOutput(KinectObject *kinect_object_ptr):
    m_kinect_object_ptr(kinect_object_ptr)
{

}

KinectInputOutput::~KinectInputOutput()
{
    destructor(true);
}

KinectInputOutput::KinectInputOutput(KinectInputOutput &kinect_input_output_ref):
    m_kinect_object_ptr(kinect_input_output_ref.get_kinect_object())
{

}

KinectInputOutput & KinectInputOutput::operator = (KinectInputOutput &kinect_input_output_ref)
{
    m_kinect_object_ptr = kinect_input_output_ref.get_kinect_object();

    return *this;
}

KinectInputOutput::KinectInputOutput(KinectInputOutput &&kinect_input_output_ref_ref):
    m_kinect_object_ptr(kinect_input_output_ref_ref.get_kinect_object())
{

}

KinectInputOutput & KinectInputOutput::operator = (KinectInputOutput &&kinect_input_output_ref_ref)
{
    m_kinect_object_ptr = kinect_input_output_ref_ref.get_kinect_object();

    return *this;
}

int KinectInputOutput::kinect_input_output_main()
{
    if(m_kinect_object_ptr->get_flags().at(0) && m_kinect_object_ptr->get_flags().at(1))
    {
        write_depth_to_file();
        write_video_to_file();

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

    for(unsigned short i = 0; i < m_kinect_object_ptr->get_resolution().at(1); ++i)
    {
        for(unsigned short j = 0; j < m_kinect_object_ptr->get_resolution().at(0); ++j)
        {
            depth_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_depth().at(j).at(i)), sizeof(unsigned short));
        }
    }

    depth_stream.close();

    m_kinect_object_ptr->get_log() += "Wrote depth frame to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n";

    return 1;
}

int KinectInputOutput::write_video_to_file()
{
    ofstream video_stream;
    video_stream.open("video_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".bin", ios::out | ios::binary);

    for(unsigned short i = 0; i < m_kinect_object_ptr->get_resolution().at(1); ++i)
    {
        for(unsigned short j = 0; j < m_kinect_object_ptr->get_resolution().at(0); ++j)
        {
            video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(j).at(i).at(0)), sizeof(unsigned char));
            video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(j).at(i).at(1)), sizeof(unsigned char));
            video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(j).at(i).at(2)), sizeof(unsigned char));
        }
    }

    video_stream.close();

    m_kinect_object_ptr->get_log() += "Wrote video frame to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n";

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
