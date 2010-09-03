/*
 * Camera class
 */

#include "camera.h"

Camera::Camera()
{
  int i;
  snapCount = 0;
  camera_found = 0;
  capture = cvCaptureFromCAM( 0 );
  if(capture)
	camera_found = 1;
  for(i = 0; i < MAX_SNAPS; i++)
	snapshot[i] = NULL;
}

Camera::~Camera()
{
  int i;
  cvReleaseCapture(&capture);
  for(i = 0; i < snapCount; i++)
	SDL_FreeSurface(snapshot[i]);
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

/* Saves off a frame as a snapshot.  Up to MAX_SNAPS can be saved */
void Camera::addSnapshot()
{
    SDL_Surface *frame;

    if(snapCount >= MAX_SNAPS)
	return;

    frame = getFrame();
    snapshot[snapCount] = SDL_CreateRGBSurface( frame->flags, frame->w, frame->h,
				frame->format->BitsPerPixel,
				frame->format->Rmask,
				frame->format->Gmask,
				frame->format->Bmask,
				frame->format->Amask);
   SDL_BlitSurface(frame, NULL, snapshot[snapCount], NULL);

    snapCount++;
}

/* Returns the number of snap shots taken */
int Camera::shotsTaken()
{
    return snapCount;
}

