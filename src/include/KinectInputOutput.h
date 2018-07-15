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

    inline KinectObject * get_kinect_object()
    {
        return m_kinect_object_ptr;
    }

    inline int set_kinect_object(KinectObject *kinect_object_ptr)
    {
        m_kinect_object_ptr = kinect_object_ptr;

        return 1;
    }

    int kinect_input_output_main();

    int kinect_input_output_kill(bool);

private:

    KinectObject *m_kinect_object_ptr;

    int write_depth_to_file();

    int write_video_to_file();

    int destructor(bool);

};

#endif // KINECTINPUTOUTPUT_H
