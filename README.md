# Flight Navigator

A C++ route planner that models airports as nodes and flights as directed
weighted edges, and finds the best path using Dijkstra's algorithm.

## Features
- **Passenger mode:** find the best route between two airports
- **Admin mode:** add or remove airports and flights
- Each flight stores distance, time and price
- Airports and flights are linked with pointers, so inserting or deleting
  is simple

## Build and run
    g++ -std=c++17 main.cpp -o flight_navigator
    ./flight_navigator

## Sample session
The model will give you output based on what user choose and how to travel, it will give you shortest most effecient way

## Complexity
 O((V+E) log V)
