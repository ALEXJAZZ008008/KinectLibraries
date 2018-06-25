#ifndef KINECTOPENGL_H
#define KINECTOPENGL_H

#include <GL/glut.h>
#include <pthread.h>

//!
//! \class
//! \brief The Kinect OpenGL class.
//! Attempts to visualise the depth
//! and video output of the Kinect camera
//! \details The class is a singleton. Which means that only a single object across
//! the application life span, can exist.
//!
class KinectOpenGL
{
public:
    //! Return a static reference to the singleton object.
    static KinectOpenGL &getInstance( );

    KinectOpenGL(KinectOpenGL &) = delete;
    void operator = (KinectOpenGL &) = delete;
    KinectOpenGL(KinectOpenGL &&) = delete;
    void operator = (KinectOpenGL &&) = delete;

    int kinect_opengl_main();

private:

    explicit KinectOpenGL();
    ~KinectOpenGL();

    unsigned int *depth_mid, *depth_front;
    unsigned int *rgb_mid, *rgb_front;

    GLuint gl_depth_tex;
    GLuint gl_rgb_tex;

    int g_argc;
    char **g_argv;

    int window;

    pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;

    int got_rgb = 0;
    int got_depth = 0;

    static void draw_scene();
    static void resize_scene(int Width, int Height);

    int opengl_init(int Width, int Height);
};

#endif // KINECTOPENGL_H
