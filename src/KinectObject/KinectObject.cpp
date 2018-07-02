#include "src/include/KinectObject.h"

KinectObject::KinectObject():
    m_depth(),
    m_video(),
    m_resolution(),
    m_timestamp(0),
    m_got_depth(false),
    m_got_video(false)
{

}

KinectObject::~KinectObject()
{
    destructor(true);
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
