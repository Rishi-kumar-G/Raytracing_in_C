#include <stdio.h>
#include <SDL3/SDL.h>
#include <math.h>

#define HEIGHT 600
#define WIDTH 800
#define COLOR_WHITE 0xffffffff
#define RAY_COLOR 0xffffffff

#define RAY_LENGTH 1000
#define RAY_COUNT 200

struct Circle
{
    double x;
    double y;
    double r;
    int color;
    double velocityX;
    double velocityY;
};

struct Ray
{
    double x, y;
    double angle;
};

void FillCircle(SDL_Surface *surface, struct Circle circle)
{
    double radius_square = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distance_square = pow(x - circle.x, 2) + pow(y - circle.y, 2);

            if (distance_square <= radius_square)
            {
                SDL_Rect rect = (SDL_Rect){x, y, 1, 1};

                SDL_FillSurfaceRect(surface, &rect, COLOR_WHITE);
            }
        }
    }
}

double dist(double x1, double y1, double x2, double y2)
{

    double d_square = pow(x1 - x2, 2) + pow(y1 - y2, 2);
    return pow(d_square, 0.5);
}

bool isColiding(double x, double y, struct Circle *obstacles, int obstaclesCount)
{
    for (int i = 0; i < obstaclesCount; i++)
    {
        struct Circle obstacle = obstacles[i];

        if (dist(x, y, obstacle.x, obstacle.y) <= obstacle.r)
        {
            return true;
        }
    }

    return false;
}

void drawRay(SDL_Surface *surface, struct Ray ray, struct Circle *obstacles, int obstaclesCount)
{

    double start_x = ray.x;
    double start_y = ray.y;
    double step = 1;
    bool end_of_screen = false;
    bool ray_collision = false;

    while (!end_of_screen && !ray_collision)
    {

        start_x += step * cos(ray.angle);
        start_y += step * sin(ray.angle);

        if (isColiding(start_x, start_y, obstacles, obstaclesCount))
        {
            ray_collision = true;
            break;
        }
        SDL_Rect pixel = (SDL_Rect){start_x, start_y, 1, 1};

        SDL_FillSurfaceRect(surface, &pixel, RAY_COLOR);

        if (start_x < 0 || start_x > WIDTH)
            end_of_screen = true;
        if (start_y < 0 || start_y > HEIGHT)
            end_of_screen = true;
    }
}

void generate_rays(SDL_Surface *surface, struct Circle circle, struct Circle *obstacles, int obstaclesCount)
{

    for (int rays = 0; rays < RAY_COUNT; rays++)
    {
        double angle = ((double)rays / RAY_COUNT) * 2 * M_PI;
        // printf("angles %f\n", angle);

        struct Ray ray = {circle.x, circle.y, angle};

        drawRay(surface, ray, obstacles, obstaclesCount);
    }
}

void genrate_obstacles(SDL_Surface *surface, struct Circle *obstacles, int obstacleCount)
{

    for (int i = 0; i < obstacleCount; i++)
    {
        FillCircle(surface, obstacles[i]);
    }
}

void moveObstacle(struct Circle *obstacles, int obstacleCount)
{
    for (int i = 0; i < obstacleCount; i++)
    {
        struct Circle *obstacle = &obstacles[i];

        if (obstacle->x - obstacle->r <= 0 ||
            obstacle->x + obstacle->r >= WIDTH)
        {
            obstacle->velocityX *= -1;
        }

        if (obstacle->y - obstacle->r <= 0 ||
            obstacle->y + obstacle->r >= HEIGHT)
        {
            obstacle->velocityY *= -1;
        }

        obstacle->x += obstacle->velocityX;
        obstacle->y += obstacle->velocityY;
    }
}
int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Raytracing", WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect){200, 200, 200, 200};
    double circlePosX = 300;
    double circlePosY = 300;

    SDL_Event event;
    bool simulation_running = true;
    bool LeftMousePressed = false;

    struct Circle circleObstacle1 = {50, 50, 10, COLOR_WHITE, -2, 4};
    struct Circle circleObstacle2 = {400, 300, 20, COLOR_WHITE, 4, -3};

    struct Circle obstacles[] =
        {
            circleObstacle1,
            circleObstacle2};

    int obstacleCount = sizeof(obstacles) / sizeof(obstacles[0]);

    struct Circle circle = {circlePosX, circlePosY, 50, 0xffffffff};

    while (simulation_running)
    {

        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                simulation_running = false;
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    LeftMousePressed = true;
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    LeftMousePressed = false;
                }
                break;

            case SDL_EVENT_MOUSE_MOTION:
                if (LeftMousePressed)
                {
                    circle.x = event.motion.x;
                    circle.y = event.motion.y;
                }
                break;
            }
        }

        FillCircle(surface, circle);
        generate_rays(surface, circle, obstacles, obstacleCount);
        moveObstacle(obstacles, obstacleCount);
        genrate_obstacles(surface, obstacles, obstacleCount);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
        SDL_ClearSurface(surface, 0, 0, 0, 1);
    }

    return 0;
}
