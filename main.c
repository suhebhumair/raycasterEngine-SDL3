#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define PI 3.14159265359
#define DR 0.0174533 // one degree in rads

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

#define MOVESPEED 5

//ray definition
int numrays = WINDOW_WIDTH;
float rays[WINDOW_WIDTH];



//player struct
struct player
{
    float xPos, yPos,deltaX,deltaY,angle;
};

//def player struct
struct player p;


// KEYBOARD array
const bool* keys = SDL_GetKeyboardState(NULL);
 
 /* (track everything as parallel arrays instead of a array of structs,
    so we can pass the coordinates to the renderer in a single function call.) */
 
 /* Points are plotted as a set of X and Y coordinates.
    (0, 0) is the top left of the window, and larger numbers go down
    and to the right. This isn't how geometry works, but this is pretty
    standard in 2D graphics. */

//map and player coords taken from https://lodev.org/cgtutor/raycasting.html

//debug str
char *debug[32];

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
void drawPlayer(float px, float py, float pdX, float pdY)
{
    //draw player as blue
    SDL_SetRenderDrawColor(renderer, 0 ,0,255, SDL_ALPHA_OPAQUE);
    //take px and py and draw a square at the corresponding location on the renderer
    SDL_FRect playerRect = {px*mapSize,py*mapSize,mapSize/2,mapSize/2};
    //also draw a line corresponding to the angle of the player starting at player coords and ending a direction away from player
    float centerX = (px*(mapSize)+(mapSize/4));
    float centerY = (py*(mapSize)+(mapSize/4));
    SDL_RenderLine(renderer,centerX,centerY,centerX+(pdX*mapSize),centerY+(pdY*mapSize));
    SDL_RenderFillRect(renderer, &playerRect);

}

void drawRays(float playerX, float playerY, float rays[], int numrays)
{


    // create a rotation vector using players current angle
    // calculate many rays using the ray direction at different offsets
    float planeX = -p.deltaY * 0.6f; // 0.60 ~ 60 degree FOV using rotation matrix
    float planeY =  p.deltaX * 0.6f;

    for (int i = 0; i < numrays; i++)
        {
            float cameraX = 2.0f * i / numrays - 1.0f;

            //Ray direction (using player's current angle and offsets)
            float rayDirX = p.deltaX + planeX * cameraX;
            float rayDirY = p.deltaY + planeY * cameraX;

            //current position
            float rayPosX = playerX;
            float rayPosY = playerY;

            // Which box of the map we're in
            int mapX = (int)rayPosX;
            int mapY = (int)rayPosY;

            // length of ray from current position to x or y side
            float sideDistX, sideDistY;

            // calculate delta distances
            float deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
            float deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

            // What direction to step in x or y-direction (either +1 or -1)
            int stepX, stepY;

            int hit = 0; // was there a wall hit?
            int side; // was a NS or a EW wall hit?

            //calculate step and initial sideDist
            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (rayPosX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
            }
            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (rayPosY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
            }

            // Perform DDA
            while (hit == 0)
            {
                //jump to next map square, either in x-direction, or in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                // Check if ray has hit a wall
                if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight || worldMap[mapY][mapX] > 0)
                {
                    hit = 1;
                }
            }

            // Calculate distance
            float perpWallDist;
            if (side == 0)
                perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
            else
                perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

            // Calculate hit point
            float hitX = rayPosX + perpWallDist * rayDirX;
            float hitY = rayPosY + perpWallDist * rayDirY;
            // add distance to rays array
            rays[i] = perpWallDist;

            // Draw ray from player to hit point
        if (keys[SDL_SCANCODE_SPACE]) {
            float centerX = playerX * mapSize + mapSize / 4;
            float centerY = playerY * mapSize + mapSize / 4;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderLine(renderer,centerX, centerY, hitX * mapSize, hitY * mapSize);
        }
    }
}
void checkCollisions(float px, float py, float pdX, float pdY)
{
    //todo
}

void drawMiniMap()
{
   int x,y,xo,yo,spacing; //x y xoffset yoffset and spacing

   for (int x = 0; x < mapHeight; x++)
   {
      for (int y = 0; y < mapWidth; y++)
      {
         //render each number differently
          int row = y;
          int col = x;

         switch (worldMap[row][col])
         {
               case 1: SDL_SetRenderDrawColor(renderer, 255,0,0, SDL_ALPHA_OPAQUE); break;
               case 2: SDL_SetRenderDrawColor(renderer, 255,51,153, SDL_ALPHA_OPAQUE); break;
               case 3: SDL_SetRenderDrawColor(renderer, 0,255,0, SDL_ALPHA_OPAQUE); break;
               case 4: SDL_SetRenderDrawColor(renderer, 255,128,0, SDL_ALPHA_OPAQUE); break;
               case 5: SDL_SetRenderDrawColor(renderer, 128,255,0, SDL_ALPHA_OPAQUE); break;
               default: SDL_SetRenderDrawColor(renderer, 255,255,255, SDL_ALPHA_OPAQUE); break;
         }


         spacing = 1;
         xo = col*mapSize+spacing;
         yo = row*mapSize+spacing;
         

         //define rectangle to be drawn 
         SDL_FRect mapRect = {xo,yo,mapSize - spacing,mapSize - spacing};
         // draw rectangle with gridlines by offsetting each rectangle one pixel right
         SDL_RenderFillRect(renderer, &mapRect);
      }
      
   }
}
void renderWalls(float rays[])
{
    for (int i = 0; i < WINDOW_WIDTH;i++)
    {
        // for each ray render a line from the center that is bigger the smaller the ray is
        // convert each ray distance to a line
        int lineheight =  (int)(WINDOW_HEIGHT / rays[i]);
        int starty = WINDOW_HEIGHT/2 - (lineheight/2);
        int endy = WINDOW_HEIGHT/2 + (lineheight/2);

        //draw above the line a different color
        SDL_SetRenderDrawColor(renderer, 200,200,200, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer,i,0,i,starty);
        //draw the actual line a different color (can change based on what number the walls are)
        SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer,i,starty,i,endy);
        //draw below the line a different color
        SDL_SetRenderDrawColor(renderer, 100,100,100, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer,i,endy,i,WINDOW_HEIGHT);
    }
}

// create rectangles for background color, player, etc. here
SDL_FRect firstbgRect = {0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

 /* This function runs once at startup. */
 SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
 {
     SDL_SetAppMetadata("3D Raycasting Engine", "1.0", "com.suhebhumair.raycaster-engine");
 
     if (!SDL_Init(SDL_INIT_VIDEO)) {
         SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }
 
     if (!SDL_CreateWindowAndRenderer("Raycasting Engine", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
         SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
         return SDL_APP_FAILURE;
     }

     //init player struct vars
     p.xPos = XSTART;
     p.yPos = YSTART;
     p.angle = 0.0f;
     p.deltaX = cosf(p.angle);
     p.deltaY = sinf(p.angle);


 
     last_time = SDL_GetTicks();
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs when a new event (mouse input, key presses, etc) occurs. */
 SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
 {
     if (event->type == SDL_EVENT_QUIT) {
         return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
     }
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once per frame, and is the heart of the program. */

 SDL_AppResult SDL_AppIterate(void *appstate)
 {
     const Uint64 now = SDL_GetTicks();
     const float deltaTime = ((float) (now - last_time)) / 1000.0f;   /* seconds since last iteration */
     last_time = now;

     SDL_GetKeyboardState(NULL); /* Get the state of all keypresses */

     /* You can also draw single points with SDL_RenderPoint(), but it's
        cheaper (sometimes significantly so) to do them all at once. */

     // draw background
     SDL_SetRenderDrawColor(renderer, 192,192,192, SDL_ALPHA_OPAQUE);
     SDL_RenderFillRect(renderer, &firstbgRect);

     

     // movement logic
     if (keys[SDL_SCANCODE_W]) {p.yPos += p.deltaY * MOVESPEED * deltaTime; p.xPos += p.deltaX * MOVESPEED * deltaTime;}
    if (keys[SDL_SCANCODE_S]) {p.yPos -= p.deltaY * MOVESPEED * deltaTime; p.xPos -= p.deltaX * MOVESPEED * deltaTime;}
    if (keys[SDL_SCANCODE_A]) p.angle -= 0.001; if(p.angle < 0 ) {p.angle +=2*PI;};  // Turn Left
    if (keys[SDL_SCANCODE_D]) p.angle += 0.001; if(p.angle>2*PI) {p.angle -=2*PI;};  // Turn Right

     //collisions
     checkCollisions(p.xPos,p.yPos,p.deltaX,p.deltaY);


     //direction vector change
     p.deltaX = cosf(p.angle);
     p.deltaY = sinf(p.angle);


     // raycast logic goes here:
     drawRays(p.xPos,p.yPos,rays,numrays);
     renderWalls(rays);

     // minimap logic (DEBUG)
     if (keys[SDL_SCANCODE_SPACE])
     {
         drawMiniMap();
         drawPlayer(p.xPos,p.yPos,p.deltaX,p.deltaY);
         SDL_RenderDebugText(renderer,(WINDOW_WIDTH/2)-mapSize,WINDOW_HEIGHT-mapSize,"DEBUGGING SCREEN");
     }
 
     SDL_RenderPresent(renderer);  /* put it all on the screen! */
 
     return SDL_APP_CONTINUE;  /* carry on with the program! */
 }
 
 /* This function runs once at shutdown. */
 void SDL_AppQuit(void *appstate, SDL_AppResult result)
 {
     /* SDL will clean up the window/renderer for us. */
 }
 