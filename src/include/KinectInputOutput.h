#ifndef KINECTINPUTOUTPUT_H
#define KINECTINPUTOUTPUT_H

#include <memory>
#include <fstream>
#include <string>

#include "KinectObject.h"

using namespace std;

//!
//! \class KinectInputOutput
//! \brief The Kinect Input Output class.
//! Outputs the current data in the Kinect Object class,
//! outputs every time that the kinect backend gets a new frame.
//! What should be output can be selected from the frontend.
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

    //! Gets kinect object ptr
    inline shared_ptr<KinectObject> & get_kinect_object_ptr()
    {
        return m_kinect_object_ptr;
    }

    //! Sets kinect object ptr
    inline int set_kinect_object_ptr(shared_ptr<KinectObject> &kinect_object_ptr)
    {
        m_kinect_object_ptr = kinect_object_ptr;

        return 1;
    }

    //! Gets output path string
    inline string & get_output_path()
    {
        return m_output_path;
    }

    //! Sets output path string
    inline int set_output_path(string output_path)
    {
        m_output_path = output_path;

        return 1;
    }

    //! Gets frames recorded value
    inline int get_frames_recorded()
    {
        return m_frames_recorded;
    }

    //! Sets frames recorded value
    inline int set_frames_recorded(int frames_recorded)
    {
        m_frames_recorded = frames_recorded;

        return 1;
    }

    //! Gets depth image bool
    inline bool get_depth_image_bool()
    {
        return m_depth_image_bool;
    }

    //! Sets depth image bool
    inline int set_depth_image_bool(bool depth_image_bool)
    {
        m_depth_image_bool = depth_image_bool;

        return 1;
    }

    //! Gets rgb image bool
    inline bool get_rgb_image_bool()
    {
        return m_rgb_image_bool;
    }

    //! Sets rgb image bool
    inline int set_rgb_image_bool(bool rgb_image_bool)
    {
        m_rgb_image_bool = rgb_image_bool;

        return 1;
    }

    //! Main
    int kinect_input_output_main();

    //! Disconnect or destruct remotely
    int kinect_input_output_kill(bool);

private:

    //! Holds the values gathered from the kinect
    shared_ptr<KinectObject> m_kinect_object_ptr;

    //! Holds path to where output should be located
    string m_output_path;

    //! Holds the total number of frames recorded
    int m_frames_recorded;

    //! True if the depth buffer should be written to file
    bool m_depth_image_bool;

    //! True if the rgb buffer should be written to file
    bool m_rgb_image_bool;

    //! Writes header file for depth and video files
    int write_header_to_file(string, string, string, string, string);

    //! Writes contents of depth buffer to file
    string write_depth_to_file();

    //! Writes contents of video buffer to file
    string write_video_to_file();

    //! Called by destructor,
    //! other methods may call to destruct the class
    int destructor(bool);

};

#endif // KINECTINPUTOUTPUT_H
