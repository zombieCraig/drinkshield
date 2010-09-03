/*
 * Webcam class
 *
 * Uses OpenCV to handle taking pics from a webcam
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <SDL/SDL.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

class Camera
{
public:
  Camera();
  ~Camera();
  bool hasWebCam();
  SDL_Surface *getFrame();
private:
  CvCapture *capture;
  bool camera_found;
};

#endif /* __CAMERA_H__ */
