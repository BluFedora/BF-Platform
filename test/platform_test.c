//
// Minimal Example of Creating A Window
//

#include "bf/Platform.h" /* Platform API */

#include <stdio.h> /* printf */

static void OnWindowEvent(bfWindow* window, bfEvent* evt);
static void OnWindowUpdate(bfWindow* window);

int main(int argc, char* argv[])
{
  bfPlatformAllocator allocator       = NULL;  // Use the default realloc and free, will be assigned to 'bfPlatformDefaultAllocator'.
  void*               global_userdata = NULL;  // Could be anything you want globally accessible.

  if (!bfPlatformInit((bfPlatformInitParams){argc, argv, allocator, global_userdata}))
  {
    printf("Failed to initialize the platform.\n");
    return 1;
  }

  bfWindow* const main_window = bfPlatformCreateWindow("Title", 1280, 720, k_bfWindowFlagsDefault);

  if (!main_window)
  {
    printf("Failed to create the window.\n");
    bfPlatformQuit();
    return 2;
  }

  // Initialization Here

  main_window->user_data     = NULL /* some user data here        */;
  main_window->renderer_data = NULL /* another slot for user data */;

  main_window->event_fn = &OnWindowEvent;
  main_window->frame_fn = &OnWindowUpdate;

  // Starts Main Loop, this is a blocking call.
  bfPlatformDoMainLoop(main_window);

  //                      or...
  // You can also handle the main loop yourself but that will
  // be less cross-platform with Emscripten and mobile devices.
  //
  // EX:
  //   while(!bfWindow_wantsToClose(main_window))
  //   {
  //     bfPlatformPumpEvents();
  //     main_window->frame_fn(main_window);
  //   }
  //

  // Shutdown Code Here

  bfPlatformDestroyWindow(main_window);
  bfPlatformQuit();
}

static void OnWindowEvent(bfWindow* window, bfEvent* evt)
{
  // Handle Events Here
}

static void OnWindowUpdate(bfWindow* window)
{
  // Handle Updating / Drawing Here
}
