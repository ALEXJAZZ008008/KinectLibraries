#include "src/include/KinectObject.h"

KinectObject::KinectObject():
    m_depth(),
    m_video(),
    m_resolution(),
    m_point_cloud_buffer(1, vector<float>(3932160, 0.0f)),
    m_point_cloud_buffer_iterator(m_point_cloud_buffer.begin()),
    m_timestamp(0),
    m_output_offset(0),
    m_got_depth(false),
    m_got_video(false)
{

}

KinectObject::~KinectObject()
{
    destructor(true);
}

KinectObject::KinectObject(KinectObject &kinect_object_ref):
    m_depth(kinect_object_ref.get_depth()),
    m_video(kinect_object_ref.get_video()),
    m_resolution(kinect_object_ref.get_resolution()),
    m_point_cloud_buffer(kinect_object_ref.get_point_cloud_buffer()),
    m_point_cloud_buffer_iterator(kinect_object_ref.get_point_cloud_buffer_iterator()),
    m_timestamp(kinect_object_ref.get_timestamp()),
    m_output_offset(kinect_object_ref.get_output_offset()),
    m_got_depth(kinect_object_ref.get_got_depth()),
    m_got_video(kinect_object_ref.get_got_video())
{

}

KinectObject & KinectObject::operator = (KinectObject &kinect_object_ref)
{
    m_depth = kinect_object_ref.get_depth();
    m_video = kinect_object_ref.get_video();
    m_resolution = kinect_object_ref.get_resolution();
    m_point_cloud_buffer = kinect_object_ref.get_point_cloud_buffer();
    m_point_cloud_buffer_iterator = kinect_object_ref.get_point_cloud_buffer_iterator();
    m_timestamp = kinect_object_ref.get_timestamp();
    m_output_offset = kinect_object_ref.get_output_offset();
    m_got_depth = kinect_object_ref.get_got_depth();
    m_got_video = kinect_object_ref.get_got_video();

    return *this;
}

KinectObject::KinectObject(KinectObject &&kinect_object_ref_ref):
    m_depth(kinect_object_ref_ref.get_depth()),
    m_video(kinect_object_ref_ref.get_video()),
    m_resolution(kinect_object_ref_ref.get_resolution()),
    m_point_cloud_buffer(kinect_object_ref_ref.get_point_cloud_buffer()),
    m_point_cloud_buffer_iterator(kinect_object_ref_ref.get_point_cloud_buffer_iterator()),
    m_timestamp(kinect_object_ref_ref.get_timestamp()),
    m_output_offset(kinect_object_ref_ref.get_output_offset()),
    m_got_depth(kinect_object_ref_ref.get_got_depth()),
    m_got_video(kinect_object_ref_ref.get_got_video())
{

}

KinectObject & KinectObject::operator = (KinectObject &&kinect_object_ref_ref)
{
    m_depth = kinect_object_ref_ref.get_depth();
    m_video = kinect_object_ref_ref.get_video();
    m_resolution = kinect_object_ref_ref.get_resolution();
    m_point_cloud_buffer = kinect_object_ref_ref.get_point_cloud_buffer();
    m_point_cloud_buffer_iterator = kinect_object_ref_ref.get_point_cloud_buffer_iterator();
    m_timestamp = kinect_object_ref_ref.get_timestamp();
    m_output_offset = kinect_object_ref_ref.get_output_offset();
    m_got_depth = kinect_object_ref_ref.get_got_depth();
    m_got_video = kinect_object_ref_ref.get_got_video();

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
    return 1;
}
