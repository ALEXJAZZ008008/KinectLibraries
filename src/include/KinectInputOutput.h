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

    //! Constructor
    explicit KinectInputOutput(KinectObject *);

    //! Destructor
    ~KinectInputOutput();

    //! Copy and move constructos and assignment opperators
    KinectInputOutput(KinectInputOutput &);
    KinectInputOutput & operator = (KinectInputOutput &);
    KinectInputOutput(KinectInputOutput &&);
    KinectInputOutput & operator = (KinectInputOutput &&);

    inline int set_kinect_object(KinectObject *kinect_object_ptr)
    {
        m_kinect_object_ptr = kinect_object_ptr;

        return 1;
    }

    //! Returns depth output
    string get_depth_message();

    //! Appends to depth output
    inline int append_depth_message(string depth_message)
    {
        m_depth_message += depth_message;

        return 1;
    }

    //! Returns point cloud output
    string get_point_cloud_message();

    //! Appends to depth output
    inline int append_point_cloud_message(string point_cloud_message)
    {
        m_point_cloud_message += point_cloud_message;

        return 1;
    }

    //! Returns video output
    string get_video_message();

    //! Appends to depth output
    inline int append_video_message(string video_message)
    {
        m_video_message += video_message;

        return 1;
    }

    int kinect_input_output_main();

    int kinect_input_output_kill(bool);

private:

    KinectObject *m_kinect_object_ptr;

    //! Depth output
    string m_depth_message;

    //! Depth output
    string m_point_cloud_message;

    //! Video output
    string m_video_message;

    int write_depth_to_file();

    int write_point_cloud_to_file();

    int write_video_to_file();

    int destructor(bool);

};

#endif // KINECTINPUTOUTPUT_H
