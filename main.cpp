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
 #define mapSize 12

 //define player data

 #define XSTART 22
 #define YSTART 12
 #define MOVESPEED 10
 


 //player struct
 struct player 
 {
    double xPos, yPos;
 };
//def player struct
struct player p;


// KEYBOARD array
const bool* keys = SDL_GetKeyboardState(nullptr);
 
 /* (track everything as parallel arrays instead of a array of structs,
    so we can pass the coordinates to the renderer in a single function call.) */
 
 /* Points are plotted as a set of X and Y coordinates.
    (0, 0) is the top left of the window, and larger numbers go down
    and to the right. This isn't how geometry works, but this is pretty
    standard in 2D graphics. */

//map and player coords taken from https://lodev.org/cgtutor/raycasting.html

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

// any functions go here
void drawPlayer(float px, float py)
{
   //take px and py and draw a square at the corresponding location on the 
   SDL_FRect playerRect = {px*mapSize,py*mapSize,mapSize/3,mapSize/3};
   SDL_SetRenderDrawColor(renderer, 255,0,0, SDL_ALPHA_OPAQUE);
   SDL_RenderFillRect(renderer, &playerRect);

}

void drawMiniMap()
{
   int x,y,xo,yo;

   for (int x = 0; x < mapHeight; x++)
   {
      for (int y = 0; y < mapWidth; y++)
      {
         //render each number differently

         if(worldMap[y][x] == 1) // if map contains 
         {
            SDL_SetRenderDrawColor(renderer, 255,255,255, SDL_ALPHA_OPAQUE);
         } 
         else if (worldMap[y][x] == 2)
         {
            SDL_SetRenderDrawColor(renderer, 255,51,153, SDL_ALPHA_OPAQUE);
         }
         else if (worldMap[y][x] == 3)
         {
            SDL_SetRenderDrawColor(renderer, 0,255,0, SDL_ALPHA_OPAQUE);
         }
         else if (worldMap[y][x] == 4)
         {
            SDL_SetRenderDrawColor(renderer, 255,128,0, SDL_ALPHA_OPAQUE);
         }
         else if (worldMap[y][x] == 5)
         {
            SDL_SetRenderDrawColor(renderer, 128,255,0, SDL_ALPHA_OPAQUE);
         }
         else
         {
            SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_OPAQUE);
         }

         xo = x*mapSize;
         yo = y*mapSize;
         //define rectangle to be drawn 
         SDL_FRect mapRect = {xo,yo,mapSize,mapSize};
         // draw rectangle
         SDL_RenderFillRect(renderer, &mapRect);
      }
      
   }
   
}

// create rectangles for background color, player, etc here
SDL_FRect firstbgRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

 /* This function runs once at startup. */
 SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
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

     //init player struct vars
     p.xPos = XSTART;
     p.yPos = YSTART;

     SDL_Log("%f",p.xPos);

 
     last_time = SDL_GetTicks();
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs when a new event (mouse input, keypresses, etc) occurs. */
 SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
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
    First draw bg color top and bottom, then draw all walls
 */
 SDL_AppResult SDL_AppIterate(void *appstate)
 {
     const Uint64 now = SDL_GetTicks();
     const float deltaTime = ((float) (now - last_time)) / 1000.0f;   /* seconds since last iteration */
     last_time = now;

     SDL_GetKeyboardState(nullptr); /* Get the state of all keypresses */

     /* You can also draw single points with SDL_RenderPoint(), but it's
        cheaper (sometimes significantly so) to do them all at once. */

     // draw background
     SDL_SetRenderDrawColor(renderer, 192,192,192, SDL_ALPHA_OPAQUE);
     SDL_RenderFillRect(renderer, &firstbgRect);

     //minimap logic
     if (keys[SDL_SCANCODE_SPACE])
    {
      drawMiniMap();
      drawPlayer(p.xPos,p.yPos);
    }
     

     // movement logic
     if (keys[SDL_SCANCODE_W]) p.yPos -= MOVESPEED * deltaTime;  // Up
    if (keys[SDL_SCANCODE_S]) p.yPos += MOVESPEED * deltaTime;  // Down  
    if (keys[SDL_SCANCODE_A]) p.xPos -= MOVESPEED * deltaTime;  // Left
    if (keys[SDL_SCANCODE_D]) p.xPos += MOVESPEED * deltaTime;  // Right
    

     // raycast logic goes here:
     SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); //set rendering color to red
     

     




 
     SDL_RenderPresent(renderer);  /* put it all on the screen! */
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once at shutdown. */
 void SDL_AppQuit(void *appstate, SDL_AppResult result)
 {
     /* SDL will clean up the window/renderer for us. */
 }
 