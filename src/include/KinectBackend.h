#ifndef KINECTBACKEND_H
#define KINECTBACKEND_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <array>
#include <string>

#include "libfreenect.h"
#include "src/include/KinectObject.h"

using namespace std;

//!
//! \class
//! \brief The Kinect Backend class.
//! Finds and connects to a Kinect camera.
//! In case of multiple connected cameras,
//! only the first will be considered currently.
//! \details The class is a singleton.
//! Which means that only a single object across
//! the application life span, can exist.
//!
class KinectBackend
{
public:

    //! Return a static reference to the singleton object.
    static KinectBackend &getInstance();

    //! Copy and move constructos and assignment opperators,
    //! only here to provide delete function for singleton
    KinectBackend(KinectBackend &) = delete;
    void operator = (KinectBackend &) = delete;
    KinectBackend(KinectBackend &&) = delete;
    void operator = (KinectBackend &&) = delete;

    inline KinectObject * get_kinect_object_ptr()
    {
        return m_kinect_object_ptr;
    }

    inline int set_kinect_object_ptr(KinectObject *kinect_object_ptr)
    {
        m_kinect_object_ptr = kinect_object_ptr;

        return 1;
    }

    inline freenect_context * get_freenect_context_ptr()
    {
        return m_freenect_context_ptr;
    }

    inline int set_freenect_context_ptr(freenect_context *freenect_context_ptr)
    {
        m_freenect_context_ptr = freenect_context_ptr;

        return 1;
    }

    inline freenect_device * get_freenect_device_ptr()
    {
        return m_freenect_device_ptr;
    }

    inline int set_freenect_device_ptr(freenect_device *freenect_device_ptr)
    {
        m_freenect_device_ptr = freenect_device_ptr;

        return 1;
    }

    inline freenect_raw_tilt_state * get_current_tilt_state_ptr()
    {
        return m_current_tilt_state_ptr;
    }

    inline int set_current_tilt_state_ptr(freenect_raw_tilt_state *current_tilt_state_ptr)
    {
        m_current_tilt_state_ptr = current_tilt_state_ptr;

        return 1;
    }

    //! Returns output
    string get_output();

    //! Returns depth output
    string get_depth_output();

    //! Appends to depth output
    inline int append_depth_output(string depth_output)
    {
        m_depth_output += depth_output;

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

    //! Returns stored camera tilt
    inline double get_stored_camera_tilt()
    {
        return m_stored_camera_tilt;
    }

    //! Sets stored current camera tilt
    int set_stored_camera_tilt(double);

    //! Updates the callback functions,
    //! must be called at regular intervals
    int update();

    //! Create connections
    int kinect_backend_main();

    //! Disconnect or destruct remotely
    int kinect_backend_kill(bool);

private:

    //! Constructor,
    //! currently private as class is static
    explicit KinectBackend();

    //! Destructor,
    //! currently private as class is static
    ~KinectBackend();

    KinectObject *m_kinect_object_ptr;

    //! Holds information about the usb context.
    freenect_context *m_freenect_context_ptr;

    //! Holds device information.
    freenect_device *m_freenect_device_ptr;

    //! Current tilt state.
    freenect_raw_tilt_state *m_current_tilt_state_ptr;

    //! General output
    string m_output;

    //! Depth output
    string m_depth_output;

    //! Video output
    string m_video_output;

    double m_reset_camera_tilt;

    //! Camera tilt
    double m_stored_camera_tilt;

    //! Step of camera tilt.
    double m_increment;

    //! Number of devices connected to the PC.
    int m_num_devices;

    //! Call back for depth information
    static void depth_callback(freenect_device *, void *, unsigned int);

    //! Call back for video information
    static void video_callback(freenect_device *, void *, unsigned int);

    //! Called by destructor
    //! and any other methods aimign to destruct the class
    int destructor(bool);

    //! Updates the current state of the tilt motor
    int update_tilt_state();

    //! Gets device camera tilt
    double get_device_camera_tilt();

    //! Sets device camera tilt
    int set_device_camera_tilt(double);
};

#endif // KINECTBACKEND_H
