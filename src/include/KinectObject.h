#ifndef KINECTOBJECT_H
#define KINECTOBJECT_H

#include <array>
#include <vector>

using namespace std;

//!
//! \class
//! \brief
//! \details
//!
class KinectObject
{
public:

    //! Constructor
    explicit KinectObject();

    //! Destructor
    ~KinectObject();

    //! Copy and move constructos and assignment opperators
    KinectObject(KinectObject &);
    KinectObject & operator = (KinectObject &);
    KinectObject(KinectObject &&);
    KinectObject & operator = (KinectObject &&);

    inline array<array<unsigned short, 1024>, 1280> & get_depth()
    {
        return m_depth;
    }

    inline int set_depth(array<array<unsigned short, 1024>, 1280> &depth)
    {
        m_depth = depth;

        return 1;
    }

    inline array<array<array<unsigned char, 3>, 1024>, 1280> & get_video()
    {
        return m_video;
    }

    inline int set_video(array<array<array<unsigned char, 3>, 1024>, 1280> &video)
    {
        m_video = video;

        return 1;
    }

    inline array<unsigned short, 2> & get_resolution()
    {
        return m_resolution;
    }

    inline int set_resolution(array<unsigned short, 2> &resolution)
    {
        m_resolution = resolution;

        return 1;
    }

    inline vector<vector<float>> & get_point_cloud_buffer()
    {
        return m_point_cloud_buffer;
    }

    inline int set_point_cloud_buffer(vector<vector<float>> &point_cloud_buffer)
    {
        m_point_cloud_buffer = point_cloud_buffer;

        return 1;
    }

    inline vector<vector<float>>::iterator & get_point_cloud_buffer_iterator()
    {
        return m_point_cloud_buffer_iterator;
    }

    inline int set_point_cloud_buffer_iterator(vector<vector<float>>::iterator &point_cloud_buffer_iterator)
    {
        m_point_cloud_buffer_iterator = point_cloud_buffer_iterator;

        return 1;
    }

    inline unsigned int get_timestamp()
    {
        return m_timestamp;
    }

    inline int set_timestamp(unsigned int timestamp)
    {
        m_timestamp = timestamp;

        return 1;
    }

    inline int get_output_offset()
    {
        return m_output_offset;
    }

    inline int set_output_offset(int output_offset)
    {
        m_output_offset = output_offset;

        return 1;
    }

    inline bool get_got_depth()
    {
        return m_got_depth;
    }

    inline int set_got_depth(bool got_depth)
    {
        m_got_depth = got_depth;

        return 1;
    }

    inline bool get_got_video()
    {
        return m_got_video;
    }

    inline int set_got_video(bool got_video)
    {
        m_got_video = got_video;

        return 1;
    }

    int kinect_object_main();

    int kinect_object_kill(bool);

private:

    array<array<unsigned short, 1024>, 1280> m_depth;

    array<array<array<unsigned char, 3>, 1024>, 1280> m_video;

    array<unsigned short, 2> m_resolution;

    vector<vector<float>> m_point_cloud_buffer;

    vector<vector<float>>::iterator m_point_cloud_buffer_iterator;

    unsigned int m_timestamp;

    int m_output_offset;

    bool m_got_depth;

    bool m_got_video;

    int destructor(bool);

};

#endif // KINECTOBJECT_H
