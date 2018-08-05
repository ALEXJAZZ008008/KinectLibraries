#ifndef KINECTOBJECT_H
#define KINECTOBJECT_H

#include <chrono>
#include <vector>
#include <string>

using namespace std;
using namespace std::chrono;

//!
//! \class KinectObject
//! \brief The Kinect Object class.
//! Holds the data gained from the kinect,
//! also holds meta data about the kinect eg. resolution etc.
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

    //! Gets the depth array
    inline vector<unsigned short> & get_depth()
    {
        return m_depth;
    }

    //! Sets the depth array
    inline int set_depth(vector<unsigned short> &depth)
    {
        m_depth = depth;

        return 1;
    }

    //! Gets the video array
    inline vector<unsigned char> & get_video()
    {
        return m_video;
    }

    //! Sets the video array
    inline int set_video(vector<unsigned char> &video)
    {
        m_video = video;

        return 1;
    }

    //! Gets the flags array
    inline vector<bool> & get_flags()
    {
        return m_flags;
    }

    //! Sets the flags array
    inline int set_flags(vector<bool> &flags)
    {
        m_flags = flags;

        return 1;
    }

    //! Gets the resolution array
    inline vector<unsigned short> & get_resolution()
    {
        return m_resolution;
    }

    //! Sets the resolution array
    inline int set_resolution(vector<unsigned short> &resolution)
    {
        m_resolution = resolution;

        return 1;
    }

    //! Gets the log string
    inline string & get_log()
    {
        return m_log;
    }

    //! Sets the log string
    inline int set_log(string &log)
    {
        m_log = log;

        return 1;
    }

    //! Gets the current cameta tilt value
    inline double get_current_camera_tilt()
    {
        return m_current_camera_tilt;
    }

    //! Sets the current camra tilt value
    inline int set_current_camera_tilt(double current_camera_tilt)
    {
        m_current_camera_tilt = current_camera_tilt;

        return 1;
    }

    //! Gets the epoch timestamp object
    inline time_point<system_clock> get_epoch_timestamp()
    {
        return m_epoch_timestamp;
    }

    //! Sets the epoch timestamp object
    inline int set_epoch_timestamp(time_point<system_clock> epoch_timestamp)
    {
        m_epoch_timestamp = epoch_timestamp;

        return 1;
    }

    //! Gets the kinect timestamp value
    inline unsigned int get_kinect_timestamp()
    {
        return m_kinect_timestamp;
    }

    //! Sets the kinect timestamp value
    inline int set_kinect_timestamp(unsigned int kinect_timestamp)
    {
        m_kinect_timestamp = kinect_timestamp;

        return 1;
    }

    //! Main
    int kinect_object_main();

    //! Disconnect or destruct remotely
    int kinect_object_kill(bool);

private:

    //! Holds the current depth frame
    vector<unsigned short> m_depth;

    //! Holds the current video frame
    vector<unsigned char> m_video;

    //! Holds the flags that reperesent if a frame is available
    vector<bool> m_flags;

    //! Holds the resolution that the kinect is set to
    vector<unsigned short> m_resolution;

    //! Holds the log of events
    string m_log;

    //! Holds the current angle that the camera is tilted to
    double m_current_camera_tilt;

    //! Holds the point at which a frame is captured,
    //! this is the time since epoch of the computer
    time_point<system_clock> m_epoch_timestamp;

    //! Holds the point at which a frame is captured,
    //! this is the time of the kinect camera
    unsigned int m_kinect_timestamp;

    //! Called by destructor
    //! and any other methods aimign to destruct the class
    int destructor(bool);

};

#endif // KINECTOBJECT_H
