#ifndef KINECTOBJECT_H
#define KINECTOBJECT_H

#include<chrono>
#include <vector>
#include <string>

using namespace std;
using namespace std::chrono;

//!
//! \class KinectObject
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

    inline vector<unsigned short> & get_depth()
    {
        return m_depth;
    }

    inline int set_depth(vector<unsigned short> &depth)
    {
        m_depth = depth;

        return 1;
    }

    inline vector<unsigned char> & get_video()
    {
        return m_video;
    }

    inline int set_video(vector<unsigned char> &video)
    {
        m_video = video;

        return 1;
    }

    inline vector<bool> & get_flags()
    {
        return m_flags;
    }

    inline int set_flags(vector<bool> &flags)
    {
        m_flags = flags;

        return 1;
    }

    inline vector<unsigned short> & get_resolution()
    {
        return m_resolution;
    }

    inline int set_resolution(vector<unsigned short> &resolution)
    {
        m_resolution = resolution;

        return 1;
    }

    inline string & get_log()
    {
        return m_log;
    }

    inline int set_log(string &log)
    {
        m_log = log;

        return 1;
    }

    inline double get_current_camera_tilt()
    {
        return m_current_camera_tilt;
    }

    inline int set_current_camera_tilt(double current_camera_tilt)
    {
        m_current_camera_tilt = current_camera_tilt;

        return 1;
    }

    inline time_point<system_clock> get_epoch_timestamp()
    {
        return m_epoch_timestamp;
    }

    inline int set_epoch_timestamp(time_point<system_clock> epoch_timestamp)
    {
        m_epoch_timestamp = epoch_timestamp;

        return 1;
    }

    inline unsigned int get_kinect_timestamp()
    {
        return m_kinect_timestamp;
    }

    inline int set_kinect_timestamp(unsigned int kinect_timestamp)
    {
        m_kinect_timestamp = kinect_timestamp;

        return 1;
    }

    int kinect_object_main();

    int kinect_object_kill(bool);

private:

    vector<unsigned short> m_depth;

    vector<unsigned char> m_video;

    vector<bool> m_flags;

    //! Holds the resolution that the kinect is set to
    vector<unsigned short> m_resolution;

    string m_log;

    double m_current_camera_tilt;

    time_point<system_clock> m_epoch_timestamp;

    unsigned int m_kinect_timestamp;

    int destructor(bool);

};

#endif // KINECTOBJECT_H
