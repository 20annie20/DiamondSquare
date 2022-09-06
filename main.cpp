#include <SDL.h>
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// would be good to make these interactive params in the window
#define ITERATIONS 7
#define ROUGHNESS 1.1

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;

unsigned int cell_size = 3;

unsigned int cellmap_width = 129;
unsigned int cellmap_height = 129;

unsigned int s_width = cellmap_width * cell_size;
unsigned int s_height = cellmap_height * cell_size;


void DrawPixel(unsigned int x, unsigned int y, unsigned int colour)
{
    Uint8* pixel_ptr = (Uint8*)surface->pixels + (y * cell_size * s_width + x * cell_size) * 4;

    for (unsigned int i = 0; i < cell_size; i++)
    {
        for (unsigned int j = 0; j < cell_size; j++)
        {
            *(pixel_ptr + j * 4) = colour / 2;
            *(pixel_ptr + j * 4 + 1) = colour / 1.2;
            *(pixel_ptr + j * 4 + 2) = colour;
        }
        pixel_ptr += s_width * 4;
    }
}

//no need for this anymore
constexpr int getSize(int x)
{
    int res = 2;
    for (int i = 1; i != x; ++i) {
        res *= 2;
    }
    return res + 1;
}

int avg(int** map, int size, int i, int j, int v, std::vector< std::array<int, 2>> offsets)
{
    int res = 0;
    float k = 0.0;
    int pp = NAN;
    int qq = NAN;
    for (std::vector<std::array<int, 2>>::iterator it = offsets.begin(); it != offsets.end(); ++it) {
        pp = i + it[0][0] * v;
        qq = j + it[0][1] * v;
        if (0 <= pp < size && 0 <= qq < size)
        {
            res += map[pp][qq];
            k += 1.0;
        }
    }
    if (k != 0)
    {
        int result = res / k;
        return result;
    }
    return 0;
}

int** diamond_square_step(int** map, int size, int w, int s)
{
    int v = w / 2;
    int n = size;
    srand(time(0));
    
    std::vector< std::array<int, 2>> diamond;
    diamond.push_back({ {-1, -1} });
    diamond.push_back({ {-1, 1} });
    diamond.push_back({ {1, 1} });
    diamond.push_back({ {1, -1} });
    
    std::vector< std::array<int, 2>> square;
    diamond.push_back({ {-1, 0} });
    diamond.push_back({ {0, -1} });
    diamond.push_back({ {1, 0} });
    diamond.push_back({ {0, 1} });

    // Diamond Step
    for (int i = v; i < n; i += w)
    {
        for (int j = v; j < n; j += w)
        {
            int rand = -s + std::rand() % s;
            map[i][j] = avg(map, n, i, j, v, diamond) + rand;
        }   
    }
            
    // Square Step, rows
    for (int i = v; i < n; i += w)
    {
        for (int j = 0; j < n; j += w) {
            int rand = -s + std::rand() % s;
            map[i][j] = avg(map, n, i, j, v, square) + rand;
        }
    }

    // Square Step, cols
    for (int i = 0; i < size; i += w)
    {
        for (int j = v; j < size; j += w) {
            int rand = -s + std::rand() % s;
            map[i][j] = avg(map, size, i, j, v, square) + rand;
        }
    }
    return map;
}

//move to another file to clean up
int** generateMap(int size) {
    int w = size - 1;
    int s = 80;
    int** heightMap = 0;
    heightMap = new int* [size];

    for (int h = 0; h < size; h++) {
        heightMap[h] = new int[size];
        for (int w = 0; w < size; w++)
            heightMap[h][w] = 255/3;
    }

    while (w > 1) {
        heightMap = diamond_square_step(heightMap, size, w, s);
        w /= 2;
        s *= ROUGHNESS;
    }

    int min = 1000;
    int max = -1000;

    for (int h = 0; h < size; h++)
    {
        for (int w = 0; w < size; w++)
        {
            if (heightMap[h][w] < min) {
                min = heightMap[h][w];
            }
            if (heightMap[h][w] > max) {
                max = heightMap[h][w];
            }
        }
    }

    for (int h = 0; h < getSize(ITERATIONS); h++)
    {
        for (int w = 0; w < getSize(ITERATIONS); w++)
        {
            DrawPixel(h, w, heightMap[h][w]);
        }
    }

    return heightMap;
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Diamond algorithm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        s_width, s_height, SDL_WINDOW_SHOWN);

    surface = SDL_GetWindowSurface(window);
    generateMap(getSize(ITERATIONS));

    SDL_Event e;

    bool quit = false;
    while (!quit)
    {
        SDL_UpdateWindowSurface(window);
    }

    return 0;
}