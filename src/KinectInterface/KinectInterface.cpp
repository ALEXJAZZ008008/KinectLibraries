#include "src/include/KinectInterface.h"

KinectInterface::KinectInterface():
    m_kinect_backend_ref(KinectBackend::getInstance()),
    m_kinect_input_output_ptr(new KinectInputOutput()),
    m_kinect_object_ptr(new KinectObject())
{
    m_kinect_backend_ref.set_kinect_object_ptr(m_kinect_object_ptr);
    m_kinect_input_output_ptr->set_kinect_object_ptr(m_kinect_object_ptr);
}

KinectInterface::~KinectInterface()
{
    destructor(true);
}

KinectInterface::KinectInterface(KinectInterface &kinect_interface_ref):
    m_kinect_backend_ref(kinect_interface_ref.get_kinect_backend_ref()),
    m_kinect_input_output_ptr(kinect_interface_ref.get_kinect_input_output_ptr()),
    m_kinect_object_ptr(kinect_interface_ref.get_kinect_object_ptr())
{

}

KinectInterface & KinectInterface::operator = (KinectInterface &kinect_interface_ref)
{
    m_kinect_input_output_ptr = kinect_interface_ref.get_kinect_input_output_ptr();
    m_kinect_object_ptr = kinect_interface_ref.get_kinect_object_ptr();

    return *this;
}

KinectInterface::KinectInterface(KinectInterface &&kinect_interface_ref_ref):
    m_kinect_backend_ref(kinect_interface_ref_ref.get_kinect_backend_ref()),
    m_kinect_input_output_ptr(kinect_interface_ref_ref.get_kinect_input_output_ptr()),
    m_kinect_object_ptr(kinect_interface_ref_ref.get_kinect_object_ptr())
{

}

KinectInterface & KinectInterface::operator = (KinectInterface &&kinect_interface_ref_ref)
{
    m_kinect_input_output_ptr = kinect_interface_ref_ref.get_kinect_input_output_ptr();
    m_kinect_object_ptr = kinect_interface_ref_ref.get_kinect_object_ptr();

    return *this;
}

int KinectInterface::kinect_interface_main()
{
    return 1;
}

int KinectInterface::kinect_interface_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KinectInterface::update()
{
    m_kinect_backend_ref.update();

    m_kinect_input_output_ptr->kinect_input_output_main();

    return 1;
}

int KinectInterface::destructor(bool hard)
{
    if(hard)
    {

    }

    if(m_kinect_input_output_ptr != nullptr)
    {
        m_kinect_input_output_ptr = nullptr;
    }

    if(m_kinect_object_ptr != nullptr)
    {
        m_kinect_object_ptr = nullptr;
    }

    return 1;
}
