#include "menus.h"
#include <cstdio>
#include <iostream>

using namespace std;

int displayMenu()
{
    int option;

    printf("Welcome to Rathnayaka Gym.\n");
    printf("\n");
    printf("<1> View fitness package details.\n");
    printf("<2> View food supplement details.\n");
    printf("<3> View food supplement details.\n");
    printf("<4> Purchase packages.\n");
    printf("<5> More about Rathnayaka Gyms.\n");
    printf("\n");
    printf("Please choose an option: ");
    cin >> option;

    return option;
}
