#ifndef KINECTBACKEND_H
#define KINECTBACKEND_H

#include <memory>
#include <string>

#include "libfreenect.h"
#include "src/include/KinectObject.h"

using namespace std;

//!
//! \class KinectBackend
//! \brief The Kinect Backend class.
//! Finds and connects to a Kinect camera.
//! In case of multiple connected cameras,
//! only the first will be considered currently.
//! What should be output can be selected from the frontend.
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

    //! Gets kinect object pointer
    inline shared_ptr<KinectObject> & get_kinect_object_ptr()
    {
        return m_kinect_object_ptr;
    }

    //! Sets kinect object pointer
    inline int set_kinect_object_ptr(shared_ptr<KinectObject> &kinect_object_ptr)
    {
        m_kinect_object_ptr = kinect_object_ptr;

        return 1;
    }

    //! Gets freenect context pointer
    inline freenect_context * get_freenect_context_ptr()
    {
        return m_freenect_context_ptr;
    }

    //! Sets freenect context pointer
    inline int set_freenect_context_ptr(freenect_context *freenect_context_ptr)
    {
        m_freenect_context_ptr = freenect_context_ptr;

        return 1;
    }

    //! Gets freenect device pointer
    inline freenect_device * get_freenect_device_ptr()
    {
        return m_freenect_device_ptr;
    }

    //! Sets freenect device pointer
    inline int set_freenect_device_ptr(freenect_device *freenect_device_ptr)
    {
        m_freenect_device_ptr = freenect_device_ptr;

        return 1;
    }

    //! Gets current tilt state pointer
    inline freenect_raw_tilt_state * get_current_tilt_state_ptr()
    {
        return m_current_tilt_state_ptr;
    }

    //! Sets current tilt state pointer
    inline int set_current_tilt_state_ptr(freenect_raw_tilt_state *current_tilt_state_ptr)
    {
        m_current_tilt_state_ptr = current_tilt_state_ptr;

        return 1;
    }

    //! Gets resolution vector
    inline vector<unsigned short> & get_resolution()
    {
        return m_resolution;
    }

    //! Sets resolution vector
    inline int set_resolution(vector<unsigned short> &resolution)
    {
        m_resolution = resolution;

        return 1;
    }

    //! Gets reset camera tilt value
    inline double get_reset_camera_tilt()
    {
        return m_reset_camera_tilt;
    }

    //! Sets reset camera tilt value
    inline int set_reset_camera_tilt(int reset_camera_tilt)
    {
        m_reset_camera_tilt = reset_camera_tilt;

        return 1;
    }

    //! Gets stored camera tilt value
    inline int get_current_camera_tilt()
    {
        return m_kinect_object_ptr->get_current_camera_tilt();
    }

    //! Sets stored current camera tilt value
    int set_current_camera_tilt(int);

    //! Sets stored current camera tilt value
    int set_current_camera_tilt(double);

    //! Gets increment value
    inline double get_increment()
    {
        return m_increment;
    }

    //! Sets increment value
    inline int set_increment(double increment)
    {
        m_increment = increment;

        return 1;
    }

    //! Gets number of devices value
    inline int get_number_of_devices()
    {
        return m_number_of_devices;
    }

    //! Sets number of devices value
    inline int set_number_of_devices(int number_of_devices)
    {
        m_number_of_devices = number_of_devices;

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

    //! Gets resolution small bool
    inline bool get_resolution_small_bool()
    {
        return m_resolution_small_bool;
    }

    //! Sets resolution small bool
    inline int set_resolution_small_bool(bool resolution_small_bool)
    {
        m_resolution_small_bool = resolution_small_bool;

        return 1;
    }

    //! Gets resolution med bool
    inline bool get_resolution_med_bool()
    {
        return m_resolution_med_bool;
    }

    //! Sets resolution med bool
    inline int set_resolution_med_bool(bool resolution_med_bool)
    {
        m_resolution_med_bool = resolution_med_bool;

        return 1;
    }

    //! Gets resolution high bool
    inline bool get_resolution_high_bool()
    {
        return m_resolution_high_bool;
    }

    //! Sets resolution high bool
    inline int set_resolution_high_bool(bool resolution_high_bool)
    {
        m_resolution_high_bool = resolution_high_bool;

        return 1;
    }

    //! Main, create connections
    int kinect_backend_main();

    //! Disconnect or destruct remotely
    int kinect_backend_kill(bool);

    //! Updates the callback functions,
    //! must be called at regular intervals
    int update();

private:

    //! Constructor,
    //! currently private as class is static
    explicit KinectBackend();

    //! Destructor,
    //! currently private as class is static
    ~KinectBackend();

    //! Holds the values gathered from the kinect
    shared_ptr<KinectObject> m_kinect_object_ptr;

    //! Holds information about the usb context.
    freenect_context *m_freenect_context_ptr;

    //! Holds device information.
    freenect_device *m_freenect_device_ptr;

    //! Hold current tilt state.
    freenect_raw_tilt_state *m_current_tilt_state_ptr;

    //! Holds the resolution that the kinect is set to
    vector<unsigned short> m_resolution;

    //! Holds value which camera should be reset to
    double m_reset_camera_tilt;

    //! Holds step size of camera tilt.
    double m_increment;

    //! Holds number of devices connected to the PC.
    int m_number_of_devices;

    //! True if the depth buffer should be filled
    bool m_depth_image_bool;

    //! True if the rgb buffer should be filled
    bool m_rgb_image_bool;

    //! True if the resolution is small
    bool m_resolution_small_bool;

    //! True if the resolution is medium
    bool m_resolution_med_bool;

    //! True if the resolution is high
    bool m_resolution_high_bool;

    //! Called by destructor,
    //! other methods may call to destruct the class
    int destructor(bool);

    //! Call back for depth information
    static void depth_callback(freenect_device *, void *, unsigned int);

    //! Call back for video information
    static void video_callback(freenect_device *, void *, unsigned int);

    //! Updates the current state of the tilt motor
    int update_tilt_state();

    //! Gets device camera tilt
    inline double get_device_camera_tilt();

    //! Sets device camera tilt
    int set_device_camera_tilt(double);
};

#endif // KINECTBACKEND_H
