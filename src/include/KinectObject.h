#ifndef KINECTOBJECT_H
#define KINECTOBJECT_H

#include <vector>
#include <string>

using namespace std;

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

    inline string & get_log()
    {
        return m_log;
    }

    inline int set_log(string &log)
    {
        m_log = log;

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

    int kinect_object_main();

    int kinect_object_kill(bool);

private:

    vector<unsigned short> m_depth;

    vector<unsigned char> m_video;

    vector<bool> m_flags;

    string m_log;

    unsigned int m_timestamp;

    int destructor(bool);

};

#endif // KINECTOBJECT_H
