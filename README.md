# Route Planning with Dijkstra's Algorithm and A* Algorithm
This is our Group 30 final project for COP3530 (Data Structures and Algorithms) at UF, Summer 2025. It features a route planning algorithm with a UI to select the origin and destination, and the program finds an efficient route between those points and outputs a visualization.
## Data and Implementation
We utilized mapping data from OpenStreetMap made available through the BRIDGES project at UNCC.
We implemented the project using the BRIDGES API to retrieve the data from the BRIDGES OSM server and build the graph. We created our own implementations of Dijkstra's and the A* algorithm for the route planning. Our UI was built using SFML, and the final visualization is generated using the Leaflet library.
###  Platforms and dependencies
Windows and macOS ARM64 are supported. To compile on Windows, SFML version 2.5.1 must be installed. The appropriate macOS SFML library (2.6.0, ARM64) is included in the repo.
### Running the program
After launching the program, the user selects origin and destination points using the TAB and arrow keys. Clicking the Visualize button produces the visualization. Due to limitations of the BRIDGES API, locations in this program are selected from one city.