/*
 * Camera class
 */

#include "camera.h"

Camera::Camera()
{
  camera_found = 0;
  capture = cvCaptureFromCAM( 0 );
  if(capture)
	camera_found = 1;
}

Camera::~Camera()
{
  cvReleaseCapture(&capture);
}

SDL_Surface *Camera::getFrame()
{
  if(!camera_found)
	return NULL;

  IplImage *frame = cvQueryFrame( capture );
  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)frame->imageData,
                frame->width,
                frame->height,
                frame->depth*frame->nChannels,
                frame->widthStep,
                0xff0000, 0x00ff00, 0x0000ff, 0
                );
  return surface;
}

bool Camera::hasWebCam()
{
	return camera_found;
}
