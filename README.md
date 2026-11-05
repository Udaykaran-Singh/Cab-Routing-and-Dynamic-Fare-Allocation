# Cab Routing and Dynamic Fare Allocation System

A C++17 simulation that models a city as an undirected weighted graph, finds minimum-cost routes using **Dijkstra's algorithm**, and calculates fares based on passenger demand and available seat capacity.

The project reads routes and passenger requests from `input.txt`, processes each request, and writes the selected path and calculated fare to `output.txt`.

## Features

- **Graph-based Route Modelling:** Locations are represented as vertices and direct cab routes as undirected edges.
- **Cost-aware Routing:** Each edge's travel cost is the sum of its base cost and congestion cost.
- **Shortest-path Calculation:** Dijkstra's algorithm finds a minimum-cost path between the requested source and destination.
- **Seat-capacity Modelling:** The number of active cabs on a route determines its active passenger capacity. Each cab has four seats.
- **Dynamic Fare Adjustment:** Fare multipliers depend on active capacity and inactive-route spare-seat availability.
- **File-based Simulation:** Input is read from `input.txt`, and results are written to `output.txt`.

## Repository Structure

```text
Cab-Routing-and-Dynamic-Fare-Allocation/
│
├── project_opt.cpp
├── input.txt
├── output.txt
├── Cab_Routing_and_Dynamic_Fare_Allocation_System.pdf
└── README.md
```

## How It Works

### 1. Route Cost Calculation

Each route has a base cost and congestion cost.

**Edge Cost = Base Cost + Congestion Cost**

For example:

```text
A B 10.0 2.0 2
```

- Base Cost = 10.0
- Congestion Cost = 2.0
- Active Cabs = 2
- Total Active Capacity = 2 × 4 = 8 seats

### 2. Shortest Path Using Dijkstra's Algorithm

The city network is represented using a nested `unordered_map`, which acts as an adjacency-list representation.

The `find_shortest_path()` function uses Dijkstra's algorithm with a min-priority queue to find the minimum-cost path between two locations.

**Time Complexity:**

O((V + E) log V)

Where:
- V = Number of vertices
- E = Number of edges

### 3. Dynamic Fare Calculation

The fare depends on passenger demand and available capacity.

| Condition | Fare |
|---|---|
| Active capacity is sufficient | Base Route Cost |
| Active capacity is insufficient but spare seats are sufficient | Base Route Cost × 1.25 |
| Both active and spare capacity are insufficient | Base Route Cost × 1.50 |

## Input Format

The program reads four sections from `input.txt`:

```text
NODES: A B C D

ROUTES:
A B 10.0 2.0 2
A C 15.0 3.0 1
B C 8.0 1.5 2
C D 12.0 2.5 1

INACTIVE:
C D 4
A C 2

REQUESTS:
A B 10
A C 3
B C 7
C D 5
```

### Input Parameters

- **NODES:** List of locations.
- **ROUTES:** Source, destination, base cost, congestion cost, and number of active cabs.
- **INACTIVE:** Source, destination, and available spare seats.
- **REQUESTS:** Source, destination, and passenger count.

Each cab has a capacity of four passengers.

## Sample Output

```text
Request: A -> B (10 passengers)
Path used: A -> B
Fare: 15

Request: A -> C (3 passengers)
Path used: A -> C
Fare: 18

Request: B -> C (7 passengers)
Path used: B -> C
Fare: 9.5

Request: C -> D (5 passengers)
Path used: C -> D
Fare: 18.125
```

### Example Fare Calculation

For the request A → B with 10 passengers:

- Base Route Cost = 10 + 2 = 12
- Active Capacity = 2 × 4 = 8 seats
- Required Passengers = 10
- Spare Capacity = 6 seats

Since active capacity is insufficient but spare capacity can cover the shortage:

**Final Fare = 12 × 1.25 = 15**

## Build and Run

### Prerequisites

- C++ compiler (GCC or G++)
- C++17 support

### Compilation

```bash
g++ -std=c++17 project_opt.cpp -o cab_routing
```

### Execution

```bash
./cab_routing
```

On Windows:

```bash
cab_routing.exe
```

The program reads `input.txt` and generates the results in `output.txt`.

**Note:** If your compiler reports that `INT_MAX` is undeclared, add `#include <climits>` to the source file.

## Main Code Components

| Component | Responsibility |
|---|---|
| Edge | Stores route costs, active cabs, and seat capacity |
| pair_hash | Hashes pairs of location names |
| make_key() | Normalizes route pairs |
| add_route() | Adds routes to the graph |
| set_inactive_route() | Records spare seats |
| find_shortest_path() | Implements Dijkstra's algorithm |
| assign_spare_seats() | Allocates spare seats |
| calculate_fare() | Calculates the final fare |
| process_input() | Parses input and processes requests |

## Current Implementation Limitations

This project is a routing and fare-allocation simulation rather than a production cab-booking platform.

1. Spare seats are pooled globally rather than being restricted to the selected route.
2. Active seats are not reserved or decremented after processing a request.
3. Dijkstra's algorithm considers route cost but does not account for passenger capacity.
4. Spare-seat allocation order is not deterministic because an `unordered_map` is used.
5. The NODES section is parsed but does not initialize graph vertices.
6. Input and output file error handling is limited.

## Future Improvements

- Implement route-specific spare-seat allocation.
- Track and update available cab seats after every request.
- Introduce capacity-aware shortest-path routing.
- Improve input validation and error handling.
- Add unit testing for different routing and capacity scenarios.
- Separate routing, capacity management, and fare calculation into independent modules.

## Technologies Used

- **Language:** C++
- **Standard:** C++17
- **Algorithm:** Dijkstra's Shortest Path Algorithm
- **Data Structures:** Graph, Adjacency List, Unordered Map, Vector, Pair, Priority Queue
- **Concepts:** Object-Oriented Programming, Hashing, File Handling, Graph Algorithms

## License

No license has currently been specified for this project.
