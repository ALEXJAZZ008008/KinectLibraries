#ifndef KINECTINTERFACE_H
#define KINECTINTERFACE_H

#include <memory>

#include "src/include/KinectBackend.h"
#include "src/include/KinectInputOutput.h"
#include "src/include/KinectObject.h"

//!
//! \class KinectInterface
//! \brief The Kinect Interface class.
//! Holds pointers to the backend functions of the Kinect library.
//! Frontend can access the backend through the interface.
//!
class KinectInterface
{
public:

    //! Constructor,
    explicit KinectInterface();

    //! Destructor,
    ~KinectInterface();

    //! Copy and move constructos and assignment opperators,
    KinectInterface(KinectInterface &);
    KinectInterface & operator = (KinectInterface &);
    KinectInterface(KinectInterface &&);
    KinectInterface & operator = (KinectInterface &&);

    //! Gets kinect backend ref
    inline KinectBackend & get_kinect_backend_ref()
    {
        return m_kinect_backend_ref;
    }

    //! Sets kinect backend ref
    //! only here to provide delete function for singleton
    inline int set_kinect_backend_ref(KinectBackend &kinect_backend_ref) = delete;

    //! Gets kinect input output ptr
    inline shared_ptr<KinectInputOutput> & get_kinect_input_output_ptr()
    {
        return m_kinect_input_output_ptr;
    }

    //! Sets kinect input output ptr
    inline int set_kinect_input_output_ptr(shared_ptr<KinectInputOutput> &kinect_input_output_ptr)
    {
        m_kinect_input_output_ptr = kinect_input_output_ptr;

        return 1;
    }

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

    //! Create connections
    int kinect_interface_main();

    //! Disconnect or destruct remotely
    int kinect_interface_kill(bool);

    //! Calls the update funstions of the child classes,
    //! must be called at regular intervals
    int update();

private:

    //! Holds a reference to the kinect backend class,
    //! this allows the frontend to access the backend
    KinectBackend &m_kinect_backend_ref;

    //! Holds a pointer to the kinect input output class,
    //! this allows the frontend to access the input output functions
    shared_ptr<KinectInputOutput> m_kinect_input_output_ptr;

    //! Holds a pointer to the kinect object class,
    //! this allows all classes to access the kinect data
    shared_ptr<KinectObject> m_kinect_object_ptr;

    //! Called by destructor
    //! and any other methods aimign to destruct the class
    int destructor(bool);
};

#endif // KINECTINTERFACE_H
