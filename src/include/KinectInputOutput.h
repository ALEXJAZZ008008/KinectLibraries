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
    string get_depth_output();

    //! Appends to depth output
    inline int append_depth_output(string depth_output)
    {
        m_depth_output += depth_output;

        return 1;
    }

    //! Returns point cloud output
    string get_point_cloud_output();

    //! Appends to depth output
    inline int append_point_cloud_output(string point_cloud_output)
    {
        m_point_cloud_output += point_cloud_output;

        return 1;
    }

    //! Returns video output
    string get_video_output();

    //! Appends to depth output
    inline int append_video_output(string video_output)
    {
        m_video_output += video_output;

        return 1;
    }

    int kinect_input_output_main();

    int kinect_input_output_kill(bool);

private:

    KinectObject *m_kinect_object_ptr;

    //! Depth output
    string m_depth_output;

    //! Depth output
    string m_point_cloud_output;

    //! Video output
    string m_video_output;

    int output_depth();

    int output_point_cloud();

    int output_video();

    int destructor(bool);

};

#endif // KINECTINPUTOUTPUT_H
