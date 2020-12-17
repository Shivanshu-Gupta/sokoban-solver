# PROJECT DRAFT

**ABSOLVERS**

1. Radhit Dedania, rdedania, 46802584
2. Rohith Reddy Gangam, rgangam, 82841937
3. Shivanshu Gupta, shivag5, 24883014

**INDEX**

[toc]

## 1. Problem Formulation

Data structures, their attributes and methods specific to the Sokoban problem.

### 1.1 `SokobanBoard` data structure

#### 1.1.1 Board

   The Sokoban Board data structure is a character matrix stored as a vector of strings(`vector<string>`). It represents the initial state of the Sokoban board. Each element (here, string) of the above vector stores a row of the given board. Each character within a vector element (string) stands for one of the following

| Character | Meaning                   |
| --------- | ------------------------- |
| `#`       | Wall                      |
| `@`       | Agent                     |
| `$`       | Box                       |
| `.`       | Storage location          |
| `+`       | Agent on storage location |
| `*`       | Box on storage location   |
| ``       | Vacant Spot               |

Thus, this data structure can be used as a template for outputting the current board at any time of the play by inserting the box and agent location appropriately to this data structure.

#### 1.1.2 **Goal and wall adjacency list: `goal_adj`,`wall_adj`**

   The goal(/wall) adjacency list is a `set` data structure that stores the y-coordinate of all goal(/wall) locations. Each row in this list stores the positions of goals(/walls) in the corresponding rws in Sokoban board. These data structures give space-efficient access to goal(/wall) locations to determine if the position `(x,y)` represents a goal(/wall) or getting the goal/wall positions in a particular row.

#### 1.1.3 **`n_rows` and `n_cols`**

   These store the number of rows and columns, respectively, in a given Sokoban board. The top left corner of the board is given by `(0,0)`, and the bottom right corner is provided by `(n_rows-1, n_cols -1)`.

All the data structures mentioned above are part of the `SokobanBoard` struct. It remains unchanged throughout the execution and forms the basis of  updates to `SokobanState` and `SokobanNode` structs

### 1.2 `Coord` data structure and enum `Move`

`Coord` struct focuses on the agent's position. It stores the current `x` and `y` coordinates of the agent’s location. It has various methods to assign/modify these variables and has functors to perform basic arithmetic operations `(+, -, *, /, +=, -=, *=, /=)` on them.

`Move` is an enumeration data type listing all the possible moves in Sokoban i.e., U(Up), D(Down), L(Left) and R(Right). This is used mainly for input/output.

### 1.3 States

#### 1.3.1 **`SokobanState` data structure**

Along with agents position and a board, it has following attributes and methods.

#### 1.3.2 **Box adjacency list: `box_adj`**

   `Box_adj` stores the y-coordinate of all box locations. Each `i`th node of the list preserves the y-coordinates of the boxes occurring in the `i`th row of the initial Sokoban board as a list in increasing order of the value of the y-coordinate. This data structure is dynamic - changes with each move of the agent - unlike the goal and wall adjacency lists and corresponds to a different state.

#### 1.3.3 **`HashKey` - hash value of state**

   The aggregate of wall, goal, box, and agent locations represent a Sokoban State. Naturally, an agent may need to pass through a particular location more than once during the gameplay. If that is allowed without any constraint, the agent may fool the system by going back and forth through a specific location, which may result in an infinite run time and hence, defeat the purpose of taking boxes to storage locations. For this reason, it is mandatory to keep track of Sokoban states that are visited. Disallowing the agent to traverse any visited state twice will push it to explore newer states that may further facilitate our bigger objective of moving a box to a goal location. Hash Key attribute aids in storing a Sokoban state in a compact and readily available format of HashMap. As wall and goal locations for every state remain the same, it would be more practical to map state to only agent and box locations to generate the HashMap key for that state. `setHashKey()` function transforms a state to its hashed key by applying a hash function to its agent and box locations. It also sets the value of the HashKey attribute to the newly generated hash key of that state.

#### 1.3.4 **`isWall`, `isBox`, `isPos`, `isGoal` functions**

`is<param>` method takes only coordinates as its argument. It returns `True` if the given coordinates represent `<param>` feature and `False` otherwise. Here, `<param>` can be one of the features of the following set `{wall, box, pos, goal}`. These functions are generally used to determine if a given pair of coordinates represent the agent’s location or a neighboring location is fit for locomotion.

All the data structures mentioned above are part of the `SokobanState` data structure. It represents a state in the state-space graph for a fixed value of agent and box locations. Every possible move `(U, D, L R)` on a state results in a state. Some of which may be new, while others may have been visited before.

### 1.4 Nodes

#### 1.4.1 **`SokobanNode` data structure**

`SokobanNode` is an umbrella data structure that encompasses `SokobanState` and various other attributes that assist in implementing informed search algorithms. It represents a node in a path search tree, and the metadata stored by it helps to find the path traversed from the start node to the current node. The following attributes are a part of Sokoban Node.

#### 1.4.2 **`SokobanNode` attributes**

- `state` is of type `SokobanState` and represents the state of a given Sokoban Node. It captures the Sokoban board’s state at any time instant (each move corresponds to a unit time step). A move from a given `SokobanNode` is equivalent to taking that move from the `SokobanState` of the given Node. The resulting state will be a part of the `SokobanNode` that results from taking the same action on the given `SokobanNode`.
- `parentNode`: stores the pointer to the Sokoban Node from which we reached the current Node in a single move.
- `parentMove`: saves the move `(U, D, L, R)` taken from parent Node that resulted in the current Node.
- `depth`: stores the level of the search tree at which the given Node is located. It is the same as the count of the moves performed from the initial Node to reach the given Node.
- `pathCost`: variable stores the cost of the path traversed in reaching the given Node from the initial Node. It is the sum of edge costs encountered in the state space graph during this journey.
- `heuristicCost` : the value of heuristic function computed on the given Node. It approximates the remaining distance from the given Node to a goal Node in terms of count of moves.
- `functionCost`: stores the sum of `pathCost` and `heuristicCost` variables of a given Node. It is an approximation of the cost of the entire path from initial Node to goal Node if the path includes the given Node.
- `usePathCost`, `useHeuristicCost`: As all the above data structures are shared by different search algorithms, it is a necessity to know if a particular algorithm would use PathCost/HeuristicCost to compute the functionCost that would be used for sorting the frontier(priority queue) used by the algorithm. These variables keep track of that. If set to `True`, the corresponding attribute(`pathCost`/`heuristicCost`) is used in calculating the `functionCost`. Otherwise, it is ignored. For UCS, only `pathCost` is used while for greedy BFS, only `heuristicCost` is utilized. As A* is a comprehensive search algorithm, it uses both.

### 1.5 Initial state and Node

All the features of the input Sokoban Board are encoded in the initial state. It is a root of the state space tree and is a part of the initial Node that forms the root of the search tree of the Sokoban game. `startNode` attribute notes if the given Node is an initial Node or an intermediateNode. Default value of the attribute is `False`. Its value is `True` if and only if the given Node is an initial Node.

### 1.6 Goal State and Node

A goal state has all the box locations overlapping with goal locations, i.e., all the boxes have been pushed to goal locations. It is the terminal node of the state space tree and is a part of Goal Node that forms the terminal node of the game search tree. The metadata of Goal Node represents the attribute values of the path from Initial Node to given Goal Node. Entire path can be obtained by following parent pointers from Goal Node till the initial Node is reached.

### 1.7 Goal Test

#### 1.7.1 Goal check function - `isGoalState()` for state

This function checks if all the boxes are present in the goal positions given. If yes, it returns `true`, else returns `false`.

### 1.8 Actions

#### 1.8.1 `doMove(Move)` function on `SokobanState`

This function takes move as an argument and returns the state that results from applying the given move on the current state. To do this, it first computes the next consecutive position and the next-to-next consecutive position of the agent in the direction of the move. If the next position was out of bounds or contained a wall, `null` pointer was returned instead of a state. Else, a new state with agent position set to the next position was created. If the next position was a box and the next-to-next position was either a wall or a box, then again the `null` pointer was returned. Otherwise, a new state was created and filled with appropriate values of the next agent position and the `box_adj` was accordingly modified to reflect motion of the box. Whenever a new state was created, it was hashed by `setHashKey()` and a new key was generated for future lookup.

#### 1.8.2 `doMove(Move)` function on `SokobanNode`

Returns the node that results from applying the given move on the current node. Applies `doMove()` function on the `state` attribute and assigns the resulting state to the new node. Then fills the `parentNode`, `parentMove`, `depth`, and `pathCost` attributes for the new Node and returns it.

#### 1.8.3 `isValidMove(Move)` function

Checks if the doMove function on that state results in a new state. If yes, it returns `true`, else returns `false`.

#### 1.8.4 `getChildrenNode(Node)` function

Calls `doMove()` function on the given node with all possible moves `(U, D, L, R)`. Returns the list of all resulting nodes originating by applying all valid moves on the current node.

## 2. Algorithms and Data structures for search problems

Algorithms and Data structures, their attributes and methods used for AI or search algorithms.

### 2.1 Solution data structure

`Solution` data structure is an envelope that contains `Goalfound` and `GoalNode` attributes. If a goal is eventually reached, i.e., all boxes have been pushed to the storage locations, this struct is filled appropriately to distinguish a potential solution from a failure to find a solution.

#### 2.1.1 **`Goalfound`**

`Goalfound` is set to `True` if a goal state is reached. Its default value is `False`. After executing the algorithm, to print the output path we need to first check if the algorithm resulted in a solution in which case we display the path or a failure in which case we output a failure message.

#### 2.1.2 **`goalNode`, of type SokobanNode if goal found**

If a goal state is reached, we set `SokobanNode` `GoalNode` to the current goal node so that we could access its metadata to get the `pathCost`, `depth`, sequence of moves, etc. of the entire run of algorithm.

### 2.2 Frontier and Reached data structures

#### 2.2.1 **Frontier data structure**

Frontier is a priority queue sorted on functionCost(of a given Node) which is a sum of pathCost and heuristicCost. It keeps track of nodes that are yet to be expanded but their parent Nodes have already been expanded. The top of the frontier is the node having minimum value of functionCost amongst all its companion nodes in the frontier.

#### 2.2.2 **Reached data structure**

Reached data structure is a Hash Map of states whose key is determined by running `setHashKey()` function on a given state and pointer to that state is stored as a value corresponding to that key. This data structure assists in the implementation of Hashing Pruning Technique wherein we do not allow traversal of the same state again for the sake of optimality. Basically, reached hash map servers as a lookup table for a given state to figure out its exploration(visited) status.

## 3. AI algorithms and pseudo-code

### 3.1 A* search

#### 3.1.1 **Pseudo Code for A<sup>*</sup> Algorithm**

```
def Func aStarSearch(initialNode, graphSearch)
    Frontier ←- initialNode
    initialNode->functionCost ←- computeHeuristic(initalNode)
        
    if (graphSearch)
        reached[hash(initialState)] ←- initialNode
    endif
    
    While not isempty(frontier)
        current ←- pop(frontier)
        if(isGoalState(current->state))
            solution.goalFound ←- true
            solution.goalNode ←- current
            return solution
     endif
        
        For each child_node in getChildrenNode(current)
            if(graphSearch)
                if(child_node->state is not in reached) 
                    reached[hash(child_node->state)] ←- child_node
                elseif (child_node->pathCost < reached[hash(child_node->state)]->pathCost)
                    reached[hash(child_node->state)] ←- child_node                                     
                endif
            endif
            frontier.push(child_node)
        endFor
    endWhile    
    return solution
endFunc
```

#### 3.1.2 **Explanation for pseudo code of A<sup>*</sup> Search**

At the start of the algorithm, `initialNode` is pushed to the frontier. `heuristicCost`, `functionCost` and `useheuristicCost` attributes are updated for the `initialNode`. If graph search has to be performed, hashmap entry for `initialNode` is added. While the `frontier` is not empty, topmost node of the `frontier` (priority queue sorted in non-decreasing order of `functionCost`) is popped out. If that node is a goal state, then solution struct is filled accordingly(`goalFound = true`, `goalNode = current`) and the function returns. Else, for each child of the current node, a hash map is looked up to verify if the child node has been visited in the past. If the hashed child node state is not found in the lookup table, we insert the child node into the hash map. If the child node state is equivalent to some state in the lookup table and if its `pathCost` is less than the `pathCost` of the state in hash map, then the lookup entry is modified to store the current child node. We then iterate over the `frontier` to remove nodes that have the same state as the current child node to eliminate redundant node expansion. At last, the child node is pushed to the `frontier` and the loop iteration goes on till the `frontier` becomes empty. Due to this, two scenarios arise. Either the goal state is reached during the loop execution in which case the function returns with appropriately filled solution struct which can later be used to construct the solution path or the loop execution terminates without yielding any goal state in which case there will not be any solution and the solution struct will reflect the default values which pertain to no solution case.

#### 3.1.3 **Pseudo Code for `getChildrenNode(node)` Method**

```
def Func getChildrenNode(node)
    For each move in {U, D, L, R}
        child_node ←- doMove(node, move)
        if(child_node is not NULL)
            child_queue.push(child_node)
        endif
    endFor
    return child_queue
endFunc
```

Please refer to **Section 1.8.4** for explanation

#### 3.1.4 **Pseudo Code for `doMove(node, move)` Method**

```
def Func doMove(node, move)
    nextState ←- doMove(node->state, move)
    if (nextState is not NULL)
        Allocate memory for a new node newNode
        newNode->startNode ←- false
        newNode->state ←- nextState
        newNode->parentNode ←- node
        newNode->parentMove ←- move
        newNode->depth ←- node->depth + 1
        newNode->pathCost ←- node->pathCost + 1
        newNode->heuristicCost ←- computeHeuristic(newNode)
        return newNode
    endif
    return NULL
endFunc
```

Please refer to **Section 1.8.2** for explanation

#### 3.1.5 **Pseudo Code for `doMove(state, move)` Method**

```
def Func doMove(state, move)
    diff ←- moves[move]
    nextPos ←- state->pos + diff
    next2Pos ←- state->pos + 2*diff
    Allocate memory for a new state newState
    if (not inBounds(nextPos) || isWall(nextPos))
        return NULL
    endif
    if(isBox(nextPos))
        if(isWall(next2Pos) || isBox(next2Pos))
            return NULL
        endif
        newState->pos ←- nextPos
        newState->box_adj[nextPos.x].erase(nextPos.y)
        newState->box_adj[next2Pos.x].insert(next2Pos.y)
        newState->hashKey = hash(newState)
        return newState
    endif
    newState->pos ←- nextPos
    newState->hashKey = hash(newState)
    return newState
endFunc
```

Please refer to **Section 1.8.1** for explanation

#### 3.1.6 **Pseudo Code for `computeHeuristic(node)` Method [Heuristic Computation]**

```
def Func computeHeuristic(node)
    x ←- 0
    box_pos ←- 0
    heuristicCost ←- 0
    board ←- node->state->board
    min_dist_box[no_of_boxes] ←- MAX_VALUE
    While x < board->n_rows
        For each y in board->box_adj[x]
            box_pos.x ←- x
            box_pos.y ←- y
            min_dist_box[x] ←- computeHeuristicPerBox(node, box_pos)
        endFor
        x ←- x + 1
    endWhile
    
    For each cost in min_dist_box
        heuristicCost ←- heuristicCost + cost
    endFor
    return heuristicCost
endFunc
```

```
def Func computeHeuristicPerBox(node, box_pos)
    x ←- 0
    goal_pos ←- 0
    board ←- node->state->board
    min_dist ←- MAX_VALUE
    While x < board->n_rows
        For each y in board->goal_adj[x]
            goal_pos.x ←- x
            goal_pos.y ←- y
            temp_val ←- calculateDistance(box_pos, goal_pos)
            min_dist ←- min(temp_val, min_dist)
        endFor
        x ←- x + 1
    endWhile    
    return min_dist
endFunc
```

#### 3.1.2 **Explanation for pseudo code of `computeHeuristic(node)` Function**

`computeHeuristic(node)` calculates the value of `heuristicCost` of a given node in a search tree. It iterates over every box location and calls `computeHeuristicPerBox(node, box_pos)` on that box to get the least distance between that box and any goal location which is stored at `x`<sup>th</sup> index in `min_dist_box` array for box `x`. After the loop terminates, we add the computed least distances for every box to `heuristicCost`(sum of the elements of `min_dist_box` array) and return it.`computeHeuristicPerBox(node, box_pos)` computes the minimum of the distance values(Manhattan/Euclidean) of a box to all goal locations.

**[NOTE]**

1. hash(state) generates the hash map key of a given state
2. calculateDistance(x, y) computes either the Manhattan/Euclidean distance depending on the heuristic used
3. min(x, y) returns the minimum of the values x and y

### 3.2 Heuristics

#### 3.2.1 Manhattan Heuristic

Minimum distance between the boxes and the storage locations(distance each box to its nearest goal location) using Manhattan distance as the distance metric. It is a consistent as well as admissible heuristic and hence, leads to an optimal solution for the game. For mathematical explanation on admissibility/consistency, refer to the link in **Section 4**

#### 3.2.2 Euclidean Heuristic

Minimum distance between the boxes and the storage locations(distance each box to its nearest goal location) using Euclidean distance as the distance metric. It is a consistent as well as admissible heuristic and hence, leads to an optimal solution for the game. For mathematical explanation on admissibility/consistency, refer to the link in **Section 4**

### 3.3 A* search general properties

| Property            |   Analysis          |
| --------------------| -------------       |
| Time Complexity     | O(b<sup>m</sup>)    |
| Space Complexity    | O(b<sup>m</sup>)    |
| Completeness        | Yes                 |
| Optimality          | Yes                 |

A* search with these heuristics is complete as the step cost is finite and it is also optimal because the heuristics used are consistent.

## 4. Github Repository

To know more about the pruning techniques used, mathematical explanation of admissibilty/consistency of heuristics, related work, future work, please **README.md** file at our code repository stated below.
    Team Absolvers - [github](https://github.com/Shivanshu-Gupta/sokoban-solver)

## 5. References

1. CS271 class lecture slides and videos by Professor Kalev Kask - [website](https://canvas.eee.uci.edu/courses/30466/pages/readings-and-lecture-videos?module_item_id=975749)
2. Sokoban Wikipedia - [Sokoban Wiki](http://www.sokobano.de/wiki/index.php?title=Solver)
3. Sokoban Solver (Groundai) - [report](https://www.groundai.com/project/ai-in-game-playing-sokoban-solver/1) | [codalab](https://worksheets.codalab.org/worksheets/0x2412ae8944eb449db74ce9bc0b9463fe/)
4. Sokoban Solver by Pavel Klavík - [website](https://pavel.klavik.cz/projekty/solver.html) | [code](https://pavel.klavik.cz/projekty/solver/solver.tar.gz) | [documentation](https://pavel.klavik.cz/projekty/solver/solver.pdf)
5. Sokoban Solver by James Hyun Seung Hong - [github](https://github.com/jameshong92/sokoban-solver)
6. Boxoban Level by Deepmind - [github](https://github.com/deepmind/boxoban-levels)
7. OpenAI Gym Environemnt: [github](https://github.com/mpSchrader/gym-sokoban)
8. A Sokoban Solver Using Multiple Search Algorithms and Q-learning: [website](https://home.cse.ust.hk/~yqsong/teaching/comp3211/projects/2017Fall/G14.pdf)
9. Sokoban Solver using Q-Learning: [github](https://github.com/zeertzjq/SokobanQLearning)
