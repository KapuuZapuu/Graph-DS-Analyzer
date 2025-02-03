# Graph Data Structure Analyzer
To use the program:
1. Download the .zip file onto your system, and decompress it.
2. Open your system's terminal application, and install g++ using homebrew (if not already installed).
   * You can install homebrew and g++ by typing the following into your command line:
      1. /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
      2. brew install gcc
3. Navigate to the location of the decompressed folder on your system.
   * Example: cd ~/Desktop/awadwidjaiwdjaiwdja/
4. Compile the program by running the following command:
   * g++ -o graph_application GraphAnalyzer.cpp
5. Run the program by running the following command:
   * ./graph_application

Once in the program, the user is presented with 7 options, which can be accessed by typing a # 1-7:
1. Program will read a graph ".txt" file (sample file provided), and then store its data within the program's internal variables
2. Prints the adjacency list representation of the graph.
3. Computes the shortest paths from a single source node using Dijkstra's algorithm method.
4. Computes the shortest path between two specified nodes.
5. Prints the shortest path between two specified nodes.
6. Prints the length between two specified nodes.
7. Ends program.

The program utilizes a manual implementation of the graph data structure and Djikstra's algorithm-- no external libraries were used to develop any relevant logic.

Sample files must adhere to the following format:
n m
i u v w
i u v w
i u v w
...

Key:
* n: # of verticies
* m: # of edges
* i: index # of specified edge (1, 2, 3, ... n)
* u: source vertex (labeled w/ an int) of specified edge
* v: destination vertex (labeled w/ an int) of specified edge
* w: defined edge weight (specified w/ a float)
