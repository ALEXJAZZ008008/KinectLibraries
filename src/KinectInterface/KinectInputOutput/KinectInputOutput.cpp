#include "KinectInputOutput.h"

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

string KinectInputOutput::get_depth_output()
{
    string depth_output = m_depth_output;

    m_depth_output = "";

    return depth_output;
}

string KinectInputOutput::get_point_cloud_output()
{
    string point_cloud_output = m_point_cloud_output;

    m_point_cloud_output = "";

    return point_cloud_output;
}

string KinectInputOutput::get_video_output()
{
    string video_output = m_video_output;

    m_video_output = "";

    return video_output;
}

int KinectInputOutput::kinect_input_output_main()
{
    if(m_kinect_object_ptr->get_got_depth() && m_kinect_object_ptr->get_got_video())
    {
        output_depth();
        output_point_cloud();

        output_video();

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

int KinectInputOutput::output_depth()
{
    ofstream depth_stream;
    depth_stream.open("depth_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".bin", ios::out | ios::binary);

    for(int i = 0; i < m_kinect_object_ptr->get_resolution().at(1); i++)
    {
        for(int j = 0; j < m_kinect_object_ptr->get_resolution().at(0); j++)
        {
            depth_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_depth().at(j).at(i)), sizeof(unsigned short));
        }
    }

    depth_stream.close();

    append_depth_output("Wrote depth frame to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n");

    return 1;
}

int KinectInputOutput::output_point_cloud()
{
    ofstream point_cloud_stream;
    point_cloud_stream.open("point_cloud_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".txt", ios::out | ios::binary);

    point_cloud_stream << "# .PCD v.7 - Point Cloud Data file format" << endl;
    point_cloud_stream << "VERSION .7" << endl;
    point_cloud_stream << "FIELDS x y z" << endl;
    point_cloud_stream << "SIZE 2 2 2" << endl;
    point_cloud_stream << "TYPE U U U" << endl;
    point_cloud_stream << "COUNT 1 1 1" << endl;
    point_cloud_stream << "WIDTH 640" << endl;
    point_cloud_stream << "HEIGHT 480" << endl;
    point_cloud_stream << "VIEWPOINT 0 0 0 1 0 0 0" << endl;
    point_cloud_stream << "POINTS 307200" << endl;
    point_cloud_stream << "DATA ascii" << endl;

    for(unsigned short i = 0; i < m_kinect_object_ptr->get_resolution().at(1); i++)
    {
        for(unsigned short j = 0; j < m_kinect_object_ptr->get_resolution().at(0); j++)
        {
            point_cloud_stream << j << " " << i << " " << m_kinect_object_ptr->get_depth().at(j).at(i) << endl;
        }
    }

    point_cloud_stream.close();

    append_point_cloud_output("Wrote point cloud to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n");

    return 1;
}

int KinectInputOutput::output_video()
{
    ofstream video_stream;
    video_stream.open("video_" + to_string(m_kinect_object_ptr->get_timestamp()) + ".bin", ios::out | ios::binary);

    for(int i = 0; i < m_kinect_object_ptr->get_resolution().at(1); i++)
    {
        for(int j = 0; j < m_kinect_object_ptr->get_resolution().at(0); j++)
        {
            video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(j).at(i).at(0)), sizeof(unsigned char));
            video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(j).at(i).at(1)), sizeof(unsigned char));
            video_stream.write(reinterpret_cast<char *>(&m_kinect_object_ptr->get_video().at(j).at(i).at(2)), sizeof(unsigned char));
        }
    }

    video_stream.close();

    append_video_output("Wrote video frame to file at " + to_string(m_kinect_object_ptr->get_timestamp()) + "\n");

    return 1;
}

int KinectInputOutput::destructor(bool hard)
{
    if(m_kinect_object_ptr != nullptr)
    {
        delete m_kinect_object_ptr;

        m_kinect_object_ptr = nullptr;
    }

    return 1;
}
