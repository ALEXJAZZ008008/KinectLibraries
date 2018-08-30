#include "src/include/KinectObject.h"

KinectObject::KinectObject():
    m_depth(0, 0),
    m_video(0, 0),
    m_flags(2, false),
    m_resolution(2, 0),
    m_log(""),
    m_current_camera_tilt(0),
    m_epoch_timestamp(),
    m_kinect_timestamp(0)
{

}

KinectObject::~KinectObject()
{
    destructor(true);
}

KinectObject::KinectObject(KinectObject &kinect_object_ref):
    m_depth(kinect_object_ref.get_depth()),
    m_video(kinect_object_ref.get_video()),
    m_flags(kinect_object_ref.get_flags()),
    m_resolution(kinect_object_ref.get_resolution()),
    m_log(kinect_object_ref.get_log()),
    m_current_camera_tilt(kinect_object_ref.get_current_camera_tilt()),
    m_epoch_timestamp(kinect_object_ref.get_epoch_timestamp()),
    m_kinect_timestamp(kinect_object_ref.get_kinect_timestamp())
{

}

KinectObject & KinectObject::operator = (KinectObject &kinect_object_ref)
{
    m_depth = kinect_object_ref.get_depth();
    m_video = kinect_object_ref.get_video();
    m_flags = kinect_object_ref.get_flags();
    m_resolution = kinect_object_ref.get_resolution();
    m_log = kinect_object_ref.get_log();
    m_current_camera_tilt = kinect_object_ref.get_current_camera_tilt();
    m_epoch_timestamp = kinect_object_ref.get_epoch_timestamp();
    m_kinect_timestamp = kinect_object_ref.get_kinect_timestamp();

    return *this;
}

KinectObject::KinectObject(KinectObject &&kinect_object_ref_ref):
    m_depth(kinect_object_ref_ref.get_depth()),
    m_video(kinect_object_ref_ref.get_video()),
    m_flags(kinect_object_ref_ref.get_flags()),
    m_resolution(kinect_object_ref_ref.get_resolution()),
    m_log(kinect_object_ref_ref.get_log()),
    m_current_camera_tilt(kinect_object_ref_ref.get_current_camera_tilt()),
    m_epoch_timestamp(kinect_object_ref_ref.get_epoch_timestamp()),
    m_kinect_timestamp(kinect_object_ref_ref.get_kinect_timestamp())
{

}

KinectObject & KinectObject::operator = (KinectObject &&kinect_object_ref_ref)
{
    m_depth = kinect_object_ref_ref.get_depth();
    m_video = kinect_object_ref_ref.get_video();
    m_flags = kinect_object_ref_ref.get_flags();
    m_resolution = kinect_object_ref_ref.get_resolution();
    m_log = kinect_object_ref_ref.get_log();
    m_current_camera_tilt = kinect_object_ref_ref.get_current_camera_tilt();
    m_epoch_timestamp = kinect_object_ref_ref.get_epoch_timestamp();
    m_kinect_timestamp = kinect_object_ref_ref.get_kinect_timestamp();

    return *this;
}

int KinectObject::kinect_object_main()
{
    return 1;
}

int KinectObject::kinect_object_kill(bool hard)
{
    destructor(hard);

    return 1;
}

int KinectObject::destructor(bool hard)
{
    if(hard)
    {

    }

    return 1;

}
