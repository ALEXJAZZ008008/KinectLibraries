#include "src/include/KinectOpenGL.h"

KinectOpenGL::KinectOpenGL()
{

}

KinectOpenGL::~KinectOpenGL()
{

}

KinectOpenGL & KinectOpenGL::getInstance( )
{
    static KinectOpenGL instance;

    return instance;
}

void KinectOpenGL::draw_scene()
{
    pthread_mutex_lock(&KinectOpenGL::getInstance().gl_backbuf_mutex);

    while (!KinectOpenGL::getInstance().got_depth || !KinectOpenGL::getInstance().got_rgb)
    {
        pthread_cond_wait(&KinectOpenGL::getInstance().gl_frame_cond, &KinectOpenGL::getInstance().gl_backbuf_mutex);
    }

    unsigned int *tmp;

    tmp = KinectOpenGL::getInstance().depth_front;
    KinectOpenGL::getInstance().depth_front = KinectOpenGL::getInstance().depth_mid;
    KinectOpenGL::getInstance().depth_mid = tmp;
    KinectOpenGL::getInstance().got_depth = 0;

    tmp = KinectOpenGL::getInstance().rgb_front;
    KinectOpenGL::getInstance().rgb_front = KinectOpenGL::getInstance().rgb_mid;
    KinectOpenGL::getInstance().rgb_mid = tmp;
    KinectOpenGL::getInstance().got_rgb = 0;

    pthread_mutex_unlock(&KinectOpenGL::getInstance().gl_backbuf_mutex);

    glBindTexture(GL_TEXTURE_2D, KinectOpenGL::getInstance().gl_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, KinectOpenGL::getInstance().depth_front);

    glLoadIdentity();
    glPushMatrix();
    glTranslatef((640.0 / 2.0), (480.0 / 2.0), 0.0);
    glRotatef(0.0, 0.0, 0.0, 1.0);
    glTranslatef(-(640.0 / 2.0), -(480.0 / 2.0), 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 1); glVertex3f(0, 0, 1.0);
    glTexCoord2f(1, 1); glVertex3f(640, 0, 1.0);
    glTexCoord2f(1, 0); glVertex3f(640, 480, 1.0);
    glTexCoord2f(0, 0); glVertex3f(0, 480, 1.0);
    glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, KinectOpenGL::getInstance().gl_rgb_tex);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, KinectOpenGL::getInstance().rgb_front);

    glPushMatrix();
    glTranslatef(640 + (640.0 / 2.0), (480.0 / 2.0), 0.0);
    glRotatef(0.0, 0.0, 0.0, 1.0);
    glTranslatef(-(640 + (640.0 / 2.0)), -(480.0 / 2.0), 0.0);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 1); glVertex3f(640, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(1280, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(1280, 480, 0);
    glTexCoord2f(0, 0); glVertex3f(640, 480, 0);
    glEnd();
    glPopMatrix();
    glutSwapBuffers();
}

void KinectOpenGL::resize_scene(int Width, int Height)
{
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, 1280, 0, 480, -5.0f, 5.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int KinectOpenGL::opengl_init(int Width, int Height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);

    glGenTextures(1, &gl_depth_tex);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &gl_rgb_tex);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    resize_scene(Width, Height);

    return 1;
}

int KinectOpenGL::kinect_opengl_main()
{
    glutInit(&g_argc, g_argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(1280, 480);
    glutInitWindowPosition(0, 0);

    window = glutCreateWindow("LibFreenect");

    glutDisplayFunc(KinectOpenGL::draw_scene);
    glutIdleFunc(KinectOpenGL::draw_scene);
    glutReshapeFunc(KinectOpenGL::resize_scene);

    opengl_init(1280, 480);

    glutMainLoop();

    return 1;
}
