#ifndef KINECTINPUTOUTPUT_H
#define KINECTINPUTOUTPUT_H

#include <fstream>
#include <string>

#include "KinectObject.h"

using namespace std;

//!
//! \class
//! \brief
//! \details
//!
class KinectInputOutput
{
public:

    //! Constructor
    explicit KinectInputOutput();

    //! Destructor
    ~KinectInputOutput();

    //! Copy and move constructos and assignment opperators
    KinectInputOutput(KinectInputOutput &);
    KinectInputOutput & operator = (KinectInputOutput &);
    KinectInputOutput(KinectInputOutput &&);
    KinectInputOutput & operator = (KinectInputOutput &&);

    inline KinectObject * get_kinect_object()
    {
        return m_kinect_object_ptr;
    }

    inline int set_kinect_object(KinectObject *kinect_object_ptr)
    {
        m_kinect_object_ptr = kinect_object_ptr;

        return 1;
    }

    inline string & get_output_path()
    {
        return m_output_path;
    }

    inline int set_output_path(string output_path)
    {
        m_output_path = output_path;

        return 1;
    }

    inline int get_frames_recorded()
    {
        return m_frames_recorded;
    }

    inline int set_frames_recorded(int frames_recorded)
    {
        m_frames_recorded = frames_recorded;
    }

    inline bool get_depth_image_bool()
    {
        return m_depth_image_bool;
    }

    inline int set_depth_image_bool(bool depth_image_bool)
    {
        m_depth_image_bool = depth_image_bool;

        return 1;
    }

    inline bool get_rgb_image_bool()
    {
        return m_rgb_image_bool;
    }

    inline int set_rgb_image_bool(bool rgb_image_bool)
    {
        m_rgb_image_bool = rgb_image_bool;

        return 1;
    }

    int kinect_input_output_main();

    int kinect_input_output_kill(bool);

private:

    KinectObject *m_kinect_object_ptr;

    string m_output_path;

    int m_frames_recorded;

    bool m_depth_image_bool;

    bool m_rgb_image_bool;

    int write_depth_to_file();

    int write_video_to_file();

    int destructor(bool);

};

#endif // KINECTINPUTOUTPUT_H
