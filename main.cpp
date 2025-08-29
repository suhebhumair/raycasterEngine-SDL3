 #define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
 #include <SDL3/SDL.h>
 #include <SDL3/SDL_main.h>
 #include <vector>
 #include <stdio.h>
 using std::vector;
 
 /* We will use this renderer to draw into this window every frame. */
 static SDL_Window *window = NULL;
 static SDL_Renderer *renderer = NULL;
 static Uint64 last_time = 0;
 
 #define WINDOW_WIDTH 640
 #define WINDOW_HEIGHT 480

 // Define map data

 #define mapWidth 24
 #define mapHeight 24
 
 /* (track everything as parallel arrays instead of a array of structs,
    so we can pass the coordinates to the renderer in a single function call.) */
 
 /* Points are plotted as a set of X and Y coordinates.
    (0, 0) is the top left of the window, and larger numbers go down
    and to the right. This isn't how geometry works, but this is pretty
    standard in 2D graphics. */

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

 /* This function runs once at startup. */
 SDL_AppResult SDL_AppInit(void)
 {
     SDL_SetAppMetadata("Example Renderer Points", "1.0", "com.example.renderer-points");
 
     if (!SDL_Init(SDL_INIT_VIDEO)) {
         SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }
 
     if (!SDL_CreateWindowAndRenderer("examples/renderer/points", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
         SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }
 
     last_time = SDL_GetTicks();
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs when a new event (mouse input, keypresses, etc) occurs. */
 SDL_AppResult SDL_AppEvent(SDL_Event *event)
 {
     if (event->type == SDL_EVENT_QUIT) {
         return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
     }
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once per frame, and is the heart of the program. */
 /*
    TODO:
    1. Drawing logic for Map and Player coord.
 */
 SDL_AppResult SDL_AppIterate(void)
 {
     const Uint64 now = SDL_GetTicks();
     const float deltaTime = ((float) (now - last_time)) / 1000.0f;   /* seconds since last iteration */

     /* You can also draw single points with SDL_RenderPoint(), but it's
        cheaper (sometimes significantly so) to do them all at once. */
 
     SDL_RenderPresent(renderer);  /* put it all on the screen! */
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once at shutdown. */
 void SDL_AppQuit()
 {
     /* SDL will clean up the window/renderer for us. */
 }
 