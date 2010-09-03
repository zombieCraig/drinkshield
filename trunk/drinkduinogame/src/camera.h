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

#define MAX_SNAPS	4

using namespace std;

class Camera
{
public:
  Camera();
  ~Camera();
  SDL_Surface *snapshot[MAX_SNAPS];
  bool hasWebCam();
  SDL_Surface *getFrame();
  void addSnapshot();
  int shotsTaken();
private:
  int snapCount;
  CvCapture *capture;
  bool camera_found;
};

#endif /* __CAMERA_H__ */
