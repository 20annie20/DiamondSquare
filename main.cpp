#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <array>
#include <vector>

using namespace std;

// would be good to make these interactive params in the window
#define ITERATIONS 9
#define ROUGHNESS 1.0

//no need for this anymore
constexpr int getSize(int x)
{
    int res = 2;
    for (int i = 1; i != x; ++i) {
        res *= 2;
    }
    return res + 1;
}

//move to another file to clean up
int** generateMap(int size)
{
    int w = size - 1;
    float s = ROUGHNESS;

    int** heightMap = 0;
    heightMap = new int* [size];

    for (int h = 0; h < size; h++)
    {
        heightMap[h] = new int[size];

        for (int w = 0; w < size; w++)
        {
            heightMap[h][w] = w + size * h;
        }
    }

    while (w > 0)
    {
        //diamond_step
        //square_step
        w /= 2;
        s *= ROUGHNESS;
    }

    return heightMap;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    auto heightMap = generateMap(ITERATIONS);

    const int size = getSize(ITERATIONS);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // move to separate cleaning func
    printf("\n");
    printf("Cleaning up memory...\n");
    for (int h = 0; h < size; h++) // loop variable wasn't declared
    {
        delete[] heightMap[h];
    }
    delete[] heightMap;
    heightMap = 0;

    glfwTerminate();
    return 0;
}