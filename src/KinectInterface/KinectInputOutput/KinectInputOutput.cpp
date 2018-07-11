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

string KinectInputOutput::get_depth_message()
{
    string depth_message = m_depth_message;

    m_depth_message = "";

    return depth_message;
}

string KinectInputOutput::get_point_cloud_message()
{
    string point_cloud_message = m_point_cloud_message;

    m_point_cloud_message = "";

    return point_cloud_message;
}

string KinectInputOutput::get_video_message()
{
    string video_message = m_video_message;

    m_video_message = "";

    return video_message;
}

int KinectInputOutput::kinect_input_output_main()
{
    if(m_kinect_object_ptr->get_got_depth() && m_kinect_object_ptr->get_got_video())
    {
        write_depth_to_file();

        write_point_cloud_to_file();

        write_video_to_file();

        m_kinect_object_ptr->set_got_depth(false);
        m_kinect_object_ptr->set_got_video(false);

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

    append_depth_message("Wrote depth frame to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n");

    return 1;
}

int KinectInputOutput::write_point_cloud_to_file()
{
    ofstream point_cloud_stream;

    point_cloud_stream.open("point_cloud_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".bin", ios::out | ios::binary);

    vector<float>::iterator point_cloud_iterator = m_kinect_object_ptr->get_point_cloud().begin();

    for(unsigned short j = 0; j < m_kinect_object_ptr->get_resolution().at(1); ++j)
    {
        for(unsigned short k = 0; k < m_kinect_object_ptr->get_resolution().at(0); ++k)
        {
            point_cloud_stream.write(reinterpret_cast<char *>(&point_cloud_iterator), sizeof(float));
            ++point_cloud_iterator;

            point_cloud_stream.write(reinterpret_cast<char *>(&point_cloud_iterator), sizeof(float));
            ++point_cloud_iterator;

            point_cloud_stream.write(reinterpret_cast<char *>(&point_cloud_iterator), sizeof(float));
            ++point_cloud_iterator;
        }
    }

    point_cloud_stream.close();

    point_cloud_stream.open("point_cloud_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".txt", ios::out | ios::binary);

    point_cloud_stream << "# .PCD v.7 - Point Cloud Data file format" << endl;
    point_cloud_stream << "VERSION .7" << endl;
    point_cloud_stream << "FIELDS x y z" << endl;
    point_cloud_stream << "SIZE 4 4 4" << endl;
    point_cloud_stream << "TYPE F F F" << endl;
    point_cloud_stream << "COUNT 1 1 1" << endl;
    point_cloud_stream << "WIDTH " << to_string(m_kinect_object_ptr->get_resolution().at(0) * m_kinect_object_ptr->get_resolution().at(1)) << endl;
    point_cloud_stream << "HEIGHT 1" << endl;
    point_cloud_stream << "VIEWPOINT 0 0 0 1 0 0 0" << endl;
    point_cloud_stream << "POINTS " << to_string(m_kinect_object_ptr->get_resolution().at(0) * m_kinect_object_ptr->get_resolution().at(1)) << endl;
    point_cloud_stream << "DATA ascii" << endl;

    point_cloud_iterator = m_kinect_object_ptr->get_point_cloud().begin();

    for(unsigned short j = 0; j < m_kinect_object_ptr->get_resolution().at(1); ++j)
    {
        for(unsigned short k = 0; k < m_kinect_object_ptr->get_resolution().at(0); ++k)
        {
            point_cloud_stream << to_string(*point_cloud_iterator) << " ";
            ++point_cloud_iterator;

            point_cloud_stream << to_string(*point_cloud_iterator) << " ";
            ++point_cloud_iterator;

            point_cloud_stream << to_string(*point_cloud_iterator) << endl;
            ++point_cloud_iterator;
        }
    }

    point_cloud_stream.close();

    append_point_cloud_message("Wrote point cloud to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n");

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

    append_video_message("Wrote video frame to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n");

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
