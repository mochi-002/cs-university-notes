> **Merged Edition**: University Lecture Notes + CS50 AI (Harvard) Lectures 0–6  
> Every topic from CS50 AI mapped, expanded, coded, and cross-referenced with your university course.

---

> **Legend:**
> 
> - 📋 `[CS50 AI NOTES]` — Content from CS50 AI Keynote slides (your uploads)
> - 🔵 `[ENHANCED]` — Deep explanations, Python code, practice tasks
> - 🔗 `[CROSS-REF]` — Links back to your university notes document

---

# 📚 Table of Contents

| CS50 Lecture                                  | Topic                                  | Maps To                           |
| --------------------------------------------- | -------------------------------------- | --------------------------------- |
| [[#CS50 Lecture 0 — Search]]                  | Search                                 | Lecture 3 (Search Algorithms)     |
| [[#CS50 Lecture 1 — Knowledge & Logic]]       | Knowledge & Logic                      | Lecture 2 (Knowledge Engineering) |
| [[#CS50 Lecture 2 — Uncertainty { lec2}]]     | Uncertainty & Probability              | New — Extends Lecture 2           |
| [[#CS50 Lecture 3 — Optimization { lec3}]]    | Optimization & Constraint Satisfaction | New topic                         |
| [[#CS50 Lecture 4 — Learning { lec4}]]        | Learning (Supervised + RL)             | Lectures 4–6 (ML)                 |
| [[#CS50 Lecture 5 — Neural Networks { lec5}]] | Neural Networks                        | New — Extends ML lectures         |
| [[#CS50 Lecture 6 — Language (NLP) { lec6}]]  | Language (NLP)                         | New topic                         |

---

# CS50 Lecture 0 — Search

---

## 📋 [CS50 AI NOTES] — Search

### Core Concepts

- **Agent**: entity that perceives its environment and acts upon it
- **State**: configuration of the agent and environment
- **Initial State (S0)**: state where agent begins
- **Actions**: `Actions(s)` returns the set of actions that can be executed in state s
- **Transition Model**: `Result(s, a)` returns the state resulting from action a in state s
- **State Space**: all states reachable from initial state
- **Goal Test**: way to determine whether a given state is the goal
- **Path Cost function**: numerical cost associated with a path
- **Optimal Solution**: solution with lowest path cost among all solutions

### Node Data Structure

A **node** tracks:

- A state
- A parent (node that generated this node)
- An action applied to parent's state
- A path cost from initial state

### Frontier + Explored Set (Revised Approach)

```
Start with a frontier containing the initial state.
Start with an empty explored set.
Repeat:
  If frontier is empty → no solution.
  Remove a node from frontier.
  If node = goal → return solution.
  Add node to explored set.
  Expand node; add resulting nodes to frontier
    IF not already in frontier OR explored set.
```

### Uninformed Search

- **DFS**: last-in first-out (stack) — expands deepest node first
- **BFS**: first-in first-out (queue) — expands shallowest node first

### Informed Search

- **Greedy Best-First Search**: expands node closest to goal using heuristic `h(n)`. Example: Manhattan distance in a maze.
- **A* Search**: expands node with lowest `g(n) + h(n)`. Optimal if `h(n)` is **admissible** (never overestimates) and **consistent** (`h(n) ≤ h(n') + c` for successor n' with step cost c)

### Adversarial Search

- **Minimax**: MAX player maximizes utility (+1), MIN minimizes (-1)
    - `MAX picks action a in Actions(s) that produces highest value of Min-Value(Result(s, a))`
    - Total possible Tic-Tac-Toe games: **255,168**
    - Total possible chess games: **10^29,000** (lower bound) after 4 moves each
- **Alpha-Beta Pruning**: Optimization of minimax — prune branches that can't affect result
    - `alpha`: best value MAX can guarantee (init: -∞)
    - `beta`: best value MIN can guarantee (init: +∞)
- **Depth-Limited Minimax**: limit search depth + use **evaluation function** that estimates expected utility of game from a given state

---

## 🔵 [ENHANCED] — Search: Deep Dive with Python

### Why the Explored Set Matters

Without an explored set, search can loop forever in cyclic graphs. The explored set prevents re-visiting states — this is the crucial difference between CS50's "Revised Approach" and the naive approach.

```python
from collections import deque
import heapq
from typing import Any, Callable, Dict, List, Optional, Set, Tuple

# ============================================================
# GENERIC SEARCH FRAMEWORK (CS50 AI Style)
# Works for ANY search problem by abstracting the problem
# ============================================================

class SearchProblem:
    """
    Abstract base class for search problems.
    Matches CS50 AI's formulation exactly.
    """
    def initial_state(self) -> Any:
        raise NotImplementedError
    
    def actions(self, state: Any) -> List[Any]:
        """Actions(s): returns set of actions executable in state s."""
        raise NotImplementedError
    
    def result(self, state: Any, action: Any) -> Any:
        """Result(s, a): returns state resulting from action a in state s."""
        raise NotImplementedError
    
    def is_goal(self, state: Any) -> bool:
        """Goal Test: is this state the goal?"""
        raise NotImplementedError
    
    def path_cost(self, state: Any, action: Any, next_state: Any) -> float:
        """Path cost of taking action in state to reach next_state."""
        return 1  # uniform by default


class Node:
    """
    CS50 AI Node data structure.
    Tracks: state, parent, action, path_cost.
    """
    def __init__(self, state, parent=None, action=None, cost=0):
        self.state = state
        self.parent = parent
        self.action = action
        self.cost = cost
    
    def solution(self) -> List:
        """Reconstruct sequence of actions from root to this node."""
        actions = []
        node = self
        while node.parent is not None:
            actions.append(node.action)
            node = node.parent
        return list(reversed(actions))
    
    def path(self) -> List:
        """Reconstruct sequence of states from root to this node."""
        states = []
        node = self
        while node is not None:
            states.append(node.state)
            node = node.parent
        return list(reversed(states))
    
    def __lt__(self, other):
        return self.cost < other.cost  # for heapq comparison


# ============================================================
# SEARCH ALGORITHMS — CS50 AI formulation
# ============================================================

def bfs(problem: SearchProblem) -> Optional[Node]:
    """
    Breadth-First Search.
    Frontier: Queue (FIFO) → explores shallowest nodes first.
    Guaranteed optimal for uniform-cost problems.
    """
    frontier = deque([Node(problem.initial_state())])
    explored: Set = set()
    
    while frontier:
        node = frontier.popleft()          # FIFO
        
        if problem.is_goal(node.state):
            return node
        
        explored.add(node.state)
        
        for action in problem.actions(node.state):
            child_state = problem.result(node.state, action)
            if child_state not in explored:
                child = Node(child_state, parent=node, action=action,
                           cost=node.cost + problem.path_cost(node.state, action, child_state))
                if child_state not in [n.state for n in frontier]:
                    frontier.append(child)
    
    return None  # no solution


def dfs(problem: SearchProblem) -> Optional[Node]:
    """
    Depth-First Search.
    Frontier: Stack (LIFO) → explores deepest nodes first.
    NOT guaranteed optimal.
    """
    frontier = [Node(problem.initial_state())]
    explored: Set = set()
    
    while frontier:
        node = frontier.pop()              # LIFO
        
        if problem.is_goal(node.state):
            return node
        
        explored.add(node.state)
        
        for action in problem.actions(node.state):
            child_state = problem.result(node.state, action)
            if child_state not in explored:
                child = Node(child_state, parent=node, action=action)
                frontier.append(child)
    
    return None


def greedy_best_first(problem: SearchProblem,
                      heuristic: Callable) -> Optional[Node]:
    """
    Greedy Best-First Search.
    Expands node that LOOKS closest to goal via h(n).
    Fast but NOT guaranteed optimal.
    """
    start = Node(problem.initial_state())
    frontier = [(heuristic(start.state), start)]
    explored: Set = set()
    
    while frontier:
        _, node = heapq.heappop(frontier)
        
        if problem.is_goal(node.state):
            return node
        
        explored.add(node.state)
        
        for action in problem.actions(node.state):
            child_state = problem.result(node.state, action)
            if child_state not in explored:
                child = Node(child_state, parent=node, action=action)
                heapq.heappush(frontier, (heuristic(child_state), child))
    
    return None


def astar(problem: SearchProblem, heuristic: Callable) -> Optional[Node]:
    """
    A* Search.
    f(n) = g(n) + h(n): actual cost + estimated cost to goal.
    OPTIMAL if h is admissible and consistent.
    
    CS50: 'expands node with lowest value of g(n) + h(n)'
    """
    start = Node(problem.initial_state())
    frontier = [(heuristic(start.state), start)]
    explored: Dict = {}  # state → best cost seen
    
    while frontier:
        _, node = heapq.heappop(frontier)
        
        if problem.is_goal(node.state):
            return node
        
        if node.state in explored and explored[node.state] <= node.cost:
            continue
        
        explored[node.state] = node.cost
        
        for action in problem.actions(node.state):
            child_state = problem.result(node.state, action)
            step_cost = problem.path_cost(node.state, action, child_state)
            child_cost = node.cost + step_cost
            
            if child_state not in explored or explored[child_state] > child_cost:
                child = Node(child_state, parent=node, action=action, cost=child_cost)
                f = child_cost + heuristic(child_state)
                heapq.heappush(frontier, (f, child))
    
    return None


# ============================================================
# CONCRETE EXAMPLE: Maze Problem
# ============================================================

class MazeProblem(SearchProblem):
    """
    Simple grid maze.
    State: (row, col) tuple
    Actions: up, down, left, right
    """
    def __init__(self, grid: List[str]):
        self.grid = grid
        self.rows = len(grid)
        self.cols = len(grid[0]) if grid else 0
        self._start = self._find('A')
        self._goal = self._find('B')
    
    def _find(self, char: str) -> Tuple[int, int]:
        for r, row in enumerate(self.grid):
            for c, cell in enumerate(row):
                if cell == char:
                    return (r, c)
        raise ValueError(f"'{char}' not found in maze")
    
    def initial_state(self):
        return self._start
    
    def actions(self, state):
        r, c = state
        possible = []
        for dr, dc, name in [(-1,0,'up'), (1,0,'down'), (0,-1,'left'), (0,1,'right')]:
            nr, nc = r + dr, c + dc
            if 0 <= nr < self.rows and 0 <= nc < self.cols and self.grid[nr][nc] != '#':
                possible.append((name, (nr, nc)))
        return possible
    
    def result(self, state, action):
        return action[1]  # action is (name, new_state)
    
    def is_goal(self, state):
        return state == self._goal
    
    def manhattan_heuristic(self, state: Tuple) -> float:
        """Manhattan distance to goal — admissible for grid mazes."""
        return abs(state[0] - self._goal[0]) + abs(state[1] - self._goal[1])


# Demo
maze_grid = [
    "######",
    "#A...#",
    "#.##.#",
    "#....#",
    "#.##B#",
    "######"
]

maze = MazeProblem(maze_grid)

bfs_sol = bfs(maze)
astar_sol = astar(maze, maze.manhattan_heuristic)

print("=== Maze Search Results ===")
print(f"BFS solution path:  {bfs_sol.path()} (cost={bfs_sol.cost})")
print(f"A*  solution path:  {astar_sol.path()} (cost={astar_sol.cost})")
```

### Minimax + Alpha-Beta with CS50 Formulation

```python
import math
from typing import Optional

# ============================================================
# CS50 AI Minimax — exact function signatures from slides
# ============================================================

def minimax(state, game) -> int:
    """
    CS50 AI Minimax.
    Returns optimal utility value for current player.
    """
    if game.terminal(state):
        return game.utility(state)
    
    if game.player(state) == 'MAX':
        return max_value(state, game)
    else:
        return min_value(state, game)


def max_value(state, game) -> int:
    """MAX picks action producing highest Min-Value."""
    if game.terminal(state):
        return game.utility(state)
    
    v = -math.inf
    for action in game.actions(state):
        v = max(v, min_value(game.result(state, action), game))
    return v


def min_value(state, game) -> int:
    """MIN picks action producing lowest Max-Value."""
    if game.terminal(state):
        return game.utility(state)
    
    v = math.inf
    for action in game.actions(state):
        v = min(v, max_value(game.result(state, action), game))
    return v


def alpha_beta_search(state, game):
    """Alpha-Beta Pruning — CS50 AI optimization of minimax."""
    
    def max_val(state, alpha, beta):
        if game.terminal(state):
            return game.utility(state)
        v = -math.inf
        for action in game.actions(state):
            v = max(v, min_val(game.result(state, action), alpha, beta))
            alpha = max(alpha, v)
            if alpha >= beta:
                break  # Beta cutoff — MIN already has better
        return v
    
    def min_val(state, alpha, beta):
        if game.terminal(state):
            return game.utility(state)
        v = math.inf
        for action in game.actions(state):
            v = min(v, max_val(game.result(state, action), alpha, beta))
            beta = min(beta, v)
            if alpha >= beta:
                break  # Alpha cutoff — MAX already has better
        return v
    
    # MAX picks action with highest min_val
    best_action = None
    best_val = -math.inf
    for action in game.actions(state):
        val = min_val(game.result(state, action), -math.inf, math.inf)
        if val > best_val:
            best_val = val
            best_action = action
    return best_action


# ============================================================
# Depth-Limited Minimax (CS50 Lecture 0 concept)
# ============================================================

def depth_limited_minimax(state, game, depth: int,
                           eval_fn: callable, is_max: bool) -> float:
    """
    CS50: Use evaluation function when depth limit reached.
    eval_fn: 'estimates the expected utility of game from a given state'
    """
    if game.terminal(state):
        return game.utility(state)
    if depth == 0:
        return eval_fn(state)   # Heuristic evaluation at depth limit
    
    if is_max:
        return max(depth_limited_minimax(game.result(state, a), game,
                   depth-1, eval_fn, False)
                   for a in game.actions(state))
    else:
        return min(depth_limited_minimax(game.result(state, a), game,
                   depth-1, eval_fn, True)
                   for a in game.actions(state))
```

---

## 🔵 [ENHANCED] — CS50 Insight: Tic-Tac-Toe Scale vs Chess

CS50 highlights a critical point about scale:

- **Tic-Tac-Toe**: 255,168 possible games → full minimax is feasible
- **Chess**: 10^29,000 possible games after 4 moves each → impossible without pruning + depth limits

This is why **Alpha-Beta Pruning** (cuts branches) + **Depth-Limited Minimax** (caps depth) + **evaluation functions** (heuristic board scoring) are essential for real games.

```python
# The scale problem illustrated
tic_tac_toe_states = 255_168
chess_lower_bound_exponent = 29_000

print(f"Tic-Tac-Toe: {tic_tac_toe_states:,} states → full minimax OK")
print(f"Chess: ~10^{chess_lower_bound_exponent} states → need heuristics!")
print(f"Alpha-beta best case: O(b^(m/2)) vs minimax O(b^m)")
print(f"For b=35, m=10: minimax={35**10:,} vs alpha-beta≈{35**5:,} nodes")
```

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> Trace A* on this graph manually. Start=A, Goal=F. h values: A=4, B=2, C=4, D=1, E=3, F=0 Edges: A-B(1), A-C(3), B-D(3), B-E(5), C-D(1), D-F(2), E-F(1)

### Medium

```python
# Implement Depth-Limited DFS (used in iterative deepening)
def dls(problem: SearchProblem, limit: int) -> Optional[Node]:
    """
    Depth-Limited Search: DFS but stop at given depth.
    Returns node if found, None if no solution within limit.
    YOUR TASK: Implement this.
    """
    pass

# Then implement Iterative Deepening DFS:
def iddfs(problem: SearchProblem, max_depth: int) -> Optional[Node]:
    """
    Run DLS with depth 0, 1, 2, ... until solution found.
    Combines DFS memory efficiency with BFS optimality.
    YOUR TASK: Implement using dls().
    """
    pass
```

### Hard

> Implement a **Tic-Tac-Toe AI** using alpha-beta pruning that plays optimally. Count how many nodes are pruned vs. regular minimax. Plot node counts vs. game depth.

---

# CS50 Lecture 1 — Knowledge & Logic

---

## 📋 [CS50 AI NOTES] — Propositional Logic

- **Knowledge-based agents**: reason by operating on internal representations of knowledge
- **Propositional Symbols**: P, Q, R (e.g., "P: It is raining")
- **Logical Connectives**: ¬ (not), ∧ (and), ∨ (or), → (implies), ↔ (biconditional)
- **Model**: assignment of truth value to every propositional symbol ("possible world")
    - Example: `{P = true, Q = false}`
- **Knowledge Base (KB)**: set of sentences the agent knows to be true
- **Entailment**: `KB ⊨ α` — in every model where KB is true, α is also true
- **Inference**: process of deriving new sentences from old ones

### Inference Algorithms

**Model Checking**: Enumerate all possible models; if KB is true in all of them and α is also true → KB ⊨ α
**Modus Ponens**: If α → β and α, then β
**And Elimination**: If α ∧ β, then α (or β)
**De Morgan’s Law**: ¬(α ∧ β) ↔ ¬α ∨ ¬β, and ¬(α ∨ β) ↔ ¬α ∧ ¬β
**Double Negation Elimination**: ¬(¬α) ↔ α
**Implication Elimination**: α → β ↔ ¬α ∨ β
**Biconditional Elimination**: α ↔ β ↔ (α → β) ∧ (β → α)

### Inference by Resolution

- **Clause**: disjunction of literals
- **Conjunctive Normal Form (CNF)**: conjunction of clauses
- **Resolution**: if we have clause P ∨ Q and ¬P ∨ R, we can resolve to Q ∨ R
- To determine if KB ⊨ α: convert (KB ∧ ¬α) to CNF → resolve until contradiction or no new clauses

**CNF Conversion Steps:**

1. Eliminate biconditionals (α ↔ β → (α → β) ∧ (β → α))
2. Eliminate implications (α → β → ¬α ∨ β)
3. Move ¬ inwards using De Morgan's Laws
4. Use distributive law to get CNF

### First-Order Logic (FOL)

- **Constant Symbols**: specific objects (Minerva, Gryffindor)
- **Predicate Symbols**: relations/properties (Person, House, BelongsTo)
- **Universal Quantification**: `∀x. BelongsTo(x, Gryffindor) → ...` — "for all x..."
- **Existential Quantification**: `∃x. House(x) ∧ BelongsTo(Minerva, x)` — "there exists x..."

---

## 🔵 [ENHANCED] — Logic in Python

```python
from itertools import product
from typing import Dict, List, Set, Tuple, Optional

# ============================================================
# PROPOSITIONAL LOGIC ENGINE
# CS50 AI: Model Checking approach
# ============================================================

class Sentence:
    """Base class for propositional logic sentences."""
    def evaluate(self, model: Dict[str, bool]) -> bool:
        raise NotImplementedError
    
    def symbols(self) -> Set[str]:
        raise NotImplementedError
    
    def __repr__(self):
        return str(self)


class Symbol(Sentence):
    """Propositional symbol (P, Q, R, etc.)"""
    def __init__(self, name: str):
        self.name = name
    
    def evaluate(self, model):
        if self.name not in model:
            raise ValueError(f"Symbol '{self.name}' not in model")
        return model[self.name]
    
    def symbols(self):
        return {self.name}
    
    def __str__(self):
        return self.name


class Not(Sentence):
    """¬ (negation)"""
    def __init__(self, operand: Sentence):
        self.operand = operand
    
    def evaluate(self, model):
        return not self.operand.evaluate(model)
    
    def symbols(self):
        return self.operand.symbols()
    
    def __str__(self):
        return f"¬{self.operand}"


class And(Sentence):
    """∧ (conjunction)"""
    def __init__(self, *conjuncts: Sentence):
        self.conjuncts = list(conjuncts)
    
    def evaluate(self, model):
        return all(c.evaluate(model) for c in self.conjuncts)
    
    def symbols(self):
        return set().union(*[c.symbols() for c in self.conjuncts])
    
    def __str__(self):
        return " ∧ ".join(f"({c})" for c in self.conjuncts)


class Or(Sentence):
    """∨ (disjunction)"""
    def __init__(self, *disjuncts: Sentence):
        self.disjuncts = list(disjuncts)
    
    def evaluate(self, model):
        return any(d.evaluate(model) for d in self.disjuncts)
    
    def symbols(self):
        return set().union(*[d.symbols() for d in self.disjuncts])
    
    def __str__(self):
        return " ∨ ".join(f"({d})" for d in self.disjuncts)


class Implication(Sentence):
    """→ (implication): α → β ≡ ¬α ∨ β"""
    def __init__(self, antecedent: Sentence, consequent: Sentence):
        self.antecedent = antecedent
        self.consequent = consequent
    
    def evaluate(self, model):
        return not self.antecedent.evaluate(model) or self.consequent.evaluate(model)
    
    def symbols(self):
        return self.antecedent.symbols() | self.consequent.symbols()
    
    def __str__(self):
        return f"({self.antecedent} → {self.consequent})"


class Biconditional(Sentence):
    """↔ (biconditional): α ↔ β ≡ (α → β) ∧ (β → α)"""
    def __init__(self, left: Sentence, right: Sentence):
        self.left = left
        self.right = right
    
    def evaluate(self, model):
        return self.left.evaluate(model) == self.right.evaluate(model)
    
    def symbols(self):
        return self.left.symbols() | self.right.symbols()
    
    def __str__(self):
        return f"({self.left} ↔ {self.right})"


# ============================================================
# MODEL CHECKING (CS50 AI Algorithm)
# ============================================================

def model_check(knowledge: Sentence, query: Sentence) -> bool:
    """
    CS50 AI: 'To determine if KB ⊨ α, enumerate all possible models.
    If in every model where KB is true, α is also true → KB entails α.'
    
    Time: O(2^n) where n = number of symbols
    """
    all_symbols = knowledge.symbols() | query.symbols()
    
    def check_all(symbols, model):
        if not symbols:
            # Base case: all symbols assigned
            if knowledge.evaluate(model):
                # KB is true in this model — query must also be true
                return query.evaluate(model)
            else:
                return True  # KB false → no constraint on query
        
        remaining = list(symbols)
        first = remaining[0]
        rest = set(remaining[1:])
        
        # Try both truth assignments
        model_true = {**model, first: True}
        model_false = {**model, first: False}
        
        return check_all(rest, model_true) and check_all(rest, model_false)
    
    return check_all(all_symbols, {})


# ============================================================
# DEMO: Clue-style Logic Puzzle (from CS50 slides)
# ============================================================

# People: Mustard, Plum, Scarlet
# Rooms: Ballroom, Kitchen, Library
# Weapons: Knife, Revolver, Wrench

mustard = Symbol("mustard")
plum = Symbol("plum")
scarlet = Symbol("scarlet")
ballroom = Symbol("ballroom")
kitchen = Symbol("kitchen")
library = Symbol("library")
knife = Symbol("knife")
revolver = Symbol("revolver")
wrench = Symbol("wrench")

# Knowledge base
knowledge = And(
    # One of each category is true
    Or(mustard, plum, scarlet),
    Or(ballroom, kitchen, library),
    Or(knife, revolver, wrench),
    
    # Known cards (not the murderer's)
    Not(mustard),
    Not(kitchen),
    Not(revolver),
    
    # Exactly one suspect (if scarlet, not plum)
    Or(Not(scarlet), Not(plum)),
)

# Query: Is Scarlet the murderer?
result = model_check(knowledge, scarlet)
print(f"Is Scarlet the murderer? {result}")

# Find all entailed facts
queries = {
    "Mustard": mustard, "Plum": plum, "Scarlet": scarlet,
    "Ballroom": ballroom, "Kitchen": kitchen, "Library": library,
    "Knife": knife, "Revolver": revolver, "Wrench": wrench
}

print("\nWhat the KB tells us:")
for name, q in queries.items():
    if model_check(knowledge, q):
        print(f"  ✓ {name} is entailed")
```

### First-Order Logic in Python

```python
# ============================================================
# FIRST-ORDER LOGIC: Universal & Existential Quantification
# ============================================================

class FOLKnowledgeBase:
    """
    Simple First-Order Logic KB.
    Represents facts and rules, can query via unification.
    """
    def __init__(self):
        self.facts = set()       # ground facts: (predicate, *args)
        self.rules = []          # (conditions, conclusion) pairs
    
    def tell(self, fact: tuple):
        """Assert a ground fact."""
        self.facts.add(fact)
    
    def add_rule(self, conditions: List[tuple], conclusion: tuple):
        """Add a universally quantified rule."""
        self.rules.append((conditions, conclusion))
    
    def ask(self, query: tuple) -> bool:
        """Check if query is entailed by KB."""
        return query in self.facts or self._forward_chain(query)
    
    def _forward_chain(self, goal: tuple) -> bool:
        """Simple forward chaining for FOL."""
        changed = True
        while changed:
            changed = False
            for conditions, conclusion_template in self.rules:
                # Try to find bindings for variables
                for binding in self._match_conditions(conditions):
                    new_fact = self._apply_binding(conclusion_template, binding)
                    if new_fact not in self.facts:
                        self.facts.add(new_fact)
                        changed = True
        return goal in self.facts
    
    def _match_conditions(self, conditions):
        """Generate all variable bindings satisfying conditions."""
        # Simplified: only handle ground conditions
        if all(c in self.facts for c in conditions):
            yield {}
    
    def _apply_binding(self, template, binding):
        return tuple(binding.get(x, x) for x in template)


# Demo: Houses puzzle (CS50 slide: Minerva, Gilderoy, etc.)
kb = FOLKnowledgeBase()

# Ground facts
kb.tell(("Person", "Minerva"))
kb.tell(("Person", "Gilderoy"))
kb.tell(("House", "Gryffindor"))
kb.tell(("House", "Hufflepuff"))
kb.tell(("House", "Ravenclaw"))
kb.tell(("House", "Slytherin"))
kb.tell(("BelongsTo", "Minerva", "Gryffindor"))

print(f"Minerva in Gryffindor: {kb.ask(('BelongsTo', 'Minerva', 'Gryffindor'))}")
print(f"Gilderoy in Gryffindor: {kb.ask(('BelongsTo', 'Gilderoy', 'Gryffindor'))}")
```

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> Convert this sentence to CNF step by step: `(P → Q) ∧ (Q → R) → (P → R)`
> `∨` -> OR

(P → Q) ∧ (Q → R) → (P → R)
1. (*(¬P ∨ Q)* ∧ *(¬Q ∨ R)*) → *(¬P ∨ R)*
2. ¬(*(¬P ∨ Q)* ∧ *(¬Q ∨ R)*) ∨ *(¬P ∨ R)*
3. (¬ *(¬P ∨ Q)* ∨ ¬ *(¬Q ∨ R)*) ∨ *(¬P ∨ R)*
4. (*(P ∧ ¬Q)* ∨ *(Q ∧ ¬R)*) ∨ *(¬P ∨ R)*
5. *(P ∧ ¬Q)* ∨ *(Q ∧ ¬R)* ∨ *(¬P ∨ R) *
	1. *(P ∧ ¬Q)* ∨ *(¬P ∨ R)* = *(P ∨ ¬P ∨ R)* ∧ *(¬Q ∨ ¬P ∨ R)*
	2. *(Q ∧ ¬R)* ∨ *(¬P ∨ R)* = *(Q ∨ ¬P ∨ R)* ∧ *(¬R ∨ ¬P ∨ R)*
		1. P ∨ ¬P = True
		2. R ∨ R = True
	3. *(¬Q ∨ ¬P ∨ R)* ∧ *(Q ∨ ¬P ∨ R)*
6. *(¬P ∨ R)*

### Medium

```python
# Implement a Mastermind solver using propositional logic
# (CS50 references Mastermind as a logic puzzle)
# Given: 3 positions, colors {R, G, B, Y}
# After each guess, told: exact matches, color-only matches
# Use model_check to eliminate impossible worlds

def mastermind_solver(feedback_history: List[tuple]) -> List[tuple]:
    """
    Return all possible secret codes consistent with feedback history.
    Each feedback is ((guess_tuple), exact_matches, color_matches).
    YOUR TASK: Implement using propositional logic or constraint filtering.
    """
    pass
```

### Hard

> Implement a **full resolution theorem prover** in Python:
> 
> 1. Convert any propositional sentence to CNF
> 2. Apply resolution repeatedly until contradiction or fixpoint
> 3. Test on the houses puzzle from CS50

---

# CS50 Lecture 2 — Uncertainty {#lec2}

---

## 📋 [CS50 AI NOTES] — Probability & Bayesian Networks

### Probability Basics

- **Unconditional probability**: degree of belief in event in absence of other evidence — `P(a)`
- **Conditional probability**: degree of belief given some evidence — `P(a | b)`
- **Random variable**: variable with domain of possible values. E.g., `Weather = {sun, cloud, rain, wind, snow}`
- **Probability distribution**: `P(Flight) = {on time: 0.6, delayed: 0.3, cancelled: 0.1}`

### Probability Rules

```
P(¬a) = 1 - P(a)                                    [Negation]
P(a ∨ b) = P(a) + P(b) - P(a ∧ b)                   [Inclusion-Exclusion]
P(a ∧ b) = P(a)P(b | a)                              [Chain Rule]
P(a ∧ b) = P(a)P(b)    [if independent]
P(a) = P(a, b) + P(a, ¬b)                            [Marginalization]
P(X=xi) = Σj P(X=xi, Y=yj)                          [Marginalization over Y]
P(a | b) = P(a ∧ b) / P(b)                          [Conditional]
```

### Bayes' Rule

```
P(a | b) = P(b | a) * P(a) / P(b)
```

CS50 example: `P(rain | clouds) = P(clouds | rain) * P(rain) / P(clouds)` Medical: Knowing `P(test result | disease)` → compute `P(disease | test result)`

### Bayesian Networks

- **Directed graph**: each node = random variable; arrow from X to Y = X is parent of Y
- Each node has a **conditional probability distribution** `P(X | Parents(X))`
- Example from CS50: Rain → Maintenance → Train → Appointment

### Inference Types

- **Query variable X**: what we want to compute
- **Evidence variables E**: observed events
- **Hidden variables Y**: non-observed, non-query variables
- **Goal**: `P(X | e)`
- **Inference by Enumeration**: `P(X | e) = α * Σy P(X, e, y)` (normalize over hidden vars)

### Approximate Inference

- **Sampling**: generate samples from distribution, count
- **Rejection Sampling**: generate samples, reject those inconsistent with evidence
- **Likelihood Weighting**: fix evidence variables, weight samples by probability of evidence

### Markov Models

- **Markov assumption**: current state depends only on previous state
- **Markov Chain**: sequence of random variables `X0, X1, X2, ...`
- **Transition model**: `P(Xt | Xt-1)`
- **Sensor Markov assumption**: evidence depends only on current state
- **Hidden Markov Model (HMM)**: system with hidden states that generate observable events

---

## 🔵 [ENHANCED] — Probability & Bayesian Networks in Python

```python
import numpy as np
from typing import Dict, List, Tuple, Optional
from itertools import product
from collections import defaultdict

# ============================================================
# BAYESIAN NETWORK (CS50 AI formulation)
# ============================================================

class BayesianNode:
    """
    A node in a Bayesian Network.
    Stores CPT: P(X | Parents)
    """
    def __init__(self, name: str, parents: List[str], domain: List):
        self.name = name
        self.parents = parents
        self.domain = domain
        self.cpt = {}  # Dict: parent_values_tuple → probability_dict
    
    def set_cpt(self, parent_values: tuple, probs: Dict):
        """Set P(X = v | parents = parent_values) for all v in domain."""
        assert abs(sum(probs.values()) - 1.0) < 1e-6, "Probabilities must sum to 1"
        self.cpt[parent_values] = probs
    
    def probability(self, value, parent_values: tuple) -> float:
        return self.cpt[parent_values][value]


class BayesianNetwork:
    """
    CS50 AI: 'data structure that represents dependencies among random variables'
    """
    def __init__(self):
        self.nodes: Dict[str, BayesianNode] = {}
        self.order: List[str] = []  # topological order
    
    def add_node(self, node: BayesianNode):
        self.nodes[node.name] = node
        self.order.append(node.name)
    
    def joint_probability(self, assignment: Dict) -> float:
        """
        Compute joint probability P(X1=x1, X2=x2, ...) using chain rule.
        P(X1,...,Xn) = Π P(Xi | Parents(Xi))
        """
        prob = 1.0
        for name in self.order:
            node = self.nodes[name]
            value = assignment[name]
            parent_vals = tuple(assignment[p] for p in node.parents)
            prob *= node.probability(value, parent_vals)
        return prob
    
    def query(self, query_var: str, evidence: Dict) -> Dict:
        """
        Inference by Enumeration: P(Query | evidence)
        CS50: 'enumerate all possible models, normalize result'
        """
        query_node = self.nodes[query_var]
        hidden_vars = [n for n in self.order 
                      if n != query_var and n not in evidence]
        
        result = {}
        for q_val in query_node.domain:
            total = 0.0
            # Sum over all combinations of hidden variables
            hidden_domains = [self.nodes[h].domain for h in hidden_vars]
            for hidden_vals in product(*hidden_domains):
                assignment = {**evidence, query_var: q_val}
                for h, v in zip(hidden_vars, hidden_vals):
                    assignment[h] = v
                total += self.joint_probability(assignment)
            result[q_val] = total
        
        # Normalize (the α in CS50's formula)
        total_prob = sum(result.values())
        return {k: v / total_prob for k, v in result.items()}
    
    def sample(self, n: int = 1000) -> List[Dict]:
        """
        Approximate inference: generate n samples from the joint distribution.
        CS50: Sampling approach.
        """
        samples = []
        for _ in range(n):
            assignment = {}
            for name in self.order:
                node = self.nodes[name]
                parent_vals = tuple(assignment[p] for p in node.parents)
                probs = node.cpt[parent_vals]
                vals = list(probs.keys())
                weights = list(probs.values())
                assignment[name] = np.random.choice(vals, p=weights)
            samples.append(assignment)
        return samples
    
    def rejection_sample(self, query_var: str, evidence: Dict, n: int = 10000) -> Dict:
        """
        CS50: Rejection Sampling — generate samples, reject those
        inconsistent with evidence.
        """
        samples = self.sample(n)
        # Reject samples inconsistent with evidence
        consistent = [s for s in samples if all(s[k] == v for k, v in evidence.items())]
        
        if not consistent:
            return {v: 1/len(self.nodes[query_var].domain) 
                   for v in self.nodes[query_var].domain}
        
        counts = defaultdict(int)
        for s in consistent:
            counts[s[query_var]] += 1
        
        total = sum(counts.values())
        return {k: v/total for k, v in counts.items()}


# ============================================================
# DEMO: CS50's Rain-Train-Appointment Network
# ============================================================
# Rain → Maintenance → Train → Appointment

bn = BayesianNetwork()

# Rain node (no parents)
rain = BayesianNode("Rain", parents=[], domain=["none", "light", "heavy"])
rain.set_cpt((), {"none": 0.7, "light": 0.2, "heavy": 0.1})
bn.add_node(rain)

# Maintenance node (parent: Rain)
maint = BayesianNode("Maintenance", parents=["Rain"], domain=["yes", "no"])
maint.set_cpt(("none",),  {"yes": 0.4, "no": 0.6})
maint.set_cpt(("light",), {"yes": 0.2, "no": 0.8})
maint.set_cpt(("heavy",), {"yes": 0.1, "no": 0.9})
bn.add_node(maint)

# Train node (parents: Rain, Maintenance)
train = BayesianNode("Train", parents=["Rain", "Maintenance"], domain=["on time", "delayed"])
train.set_cpt(("none", "yes"),   {"on time": 0.8, "delayed": 0.2})
train.set_cpt(("none", "no"),    {"on time": 0.9, "delayed": 0.1})
train.set_cpt(("light", "yes"),  {"on time": 0.6, "delayed": 0.4})
train.set_cpt(("light", "no"),   {"on time": 0.7, "delayed": 0.3})
train.set_cpt(("heavy", "yes"),  {"on time": 0.4, "delayed": 0.6})
train.set_cpt(("heavy", "no"),   {"on time": 0.5, "delayed": 0.5})
bn.add_node(train)

# Appointment node (parent: Train)
appt = BayesianNode("Appointment", parents=["Train"], domain=["attend", "miss"])
appt.set_cpt(("on time",), {"attend": 0.9, "miss": 0.1})
appt.set_cpt(("delayed",), {"attend": 0.6, "miss": 0.4})
bn.add_node(appt)

# Query: P(Train = on time | Rain = light)
result = bn.query("Train", evidence={"Rain": "light"})
print("P(Train | Rain=light):")
for val, prob in result.items():
    print(f"  {val}: {prob:.4f}")

# Rejection sampling approximation
approx = bn.rejection_sample("Train", {"Rain": "light"}, n=50000)
print("\nApproximate (rejection sampling, n=50000):")
for val, prob in approx.items():
    print(f"  {val}: {prob:.4f}")
```

### Hidden Markov Model

```python
# ============================================================
# HIDDEN MARKOV MODEL (CS50 Lecture 2)
# CS50: 'system with hidden states that generate observed events'
# ============================================================

class HiddenMarkovModel:
    """
    HMM with:
    - Hidden states (e.g., weather: sunny/rainy)
    - Observations (e.g., umbrella: yes/no)
    - Transition model: P(Xt | Xt-1)
    - Sensor model: P(Et | Xt)
    """
    def __init__(self, states, observations, transition, sensor, initial):
        self.states = states                # Hidden states
        self.observations = observations    # Possible observations
        self.transition = transition        # P(next | current)
        self.sensor = sensor               # P(observation | state)
        self.initial = initial             # P(X0)
    
    def forward(self, obs_sequence: List) -> List[Dict]:
        """
        Forward algorithm: P(Xt | e1:t) for each t.
        CS50's filtering: 'what state are we likely in now?'
        """
        # Initialize
        current = {}
        for state in self.states:
            current[state] = self.initial[state] * self.sensor[state][obs_sequence[0]]
        
        # Normalize
        total = sum(current.values())
        current = {s: v/total for s, v in current.items()}
        
        history = [current]
        
        # Forward pass
        for obs in obs_sequence[1:]:
            next_dist = {}
            for next_state in self.states:
                # Sum over all previous states
                trans_prob = sum(
                    current[prev] * self.transition[prev][next_state]
                    for prev in self.states
                )
                next_dist[next_state] = trans_prob * self.sensor[next_state][obs]
            
            # Normalize
            total = sum(next_dist.values())
            next_dist = {s: v/total for s, v in next_dist.items()}
            current = next_dist
            history.append(current)
        
        return history
    
    def viterbi(self, obs_sequence: List) -> List:
        """
        Viterbi algorithm: most likely sequence of hidden states.
        """
        V = [{}]
        path = {}
        
        for state in self.states:
            V[0][state] = self.initial[state] * self.sensor[state][obs_sequence[0]]
            path[state] = [state]
        
        for obs in obs_sequence[1:]:
            V.append({})
            new_path = {}
            for curr_state in self.states:
                probs = [(V[-2][prev] * self.transition[prev][curr_state] * 
                         self.sensor[curr_state][obs], prev)
                        for prev in self.states]
                max_prob, best_prev = max(probs)
                V[-1][curr_state] = max_prob
                new_path[curr_state] = path[best_prev] + [curr_state]
            path = new_path
        
        best_final = max(V[-1], key=V[-1].get)
        return path[best_final]


# Demo: Weather HMM (CS50 umbrella example)
hmm = HiddenMarkovModel(
    states=["sunny", "rainy"],
    observations=["umbrella", "no umbrella"],
    transition={
        "sunny": {"sunny": 0.8, "rainy": 0.2},
        "rainy": {"rainy": 0.7, "sunny": 0.3}
    },
    sensor={
        "sunny": {"umbrella": 0.2, "no umbrella": 0.8},
        "rainy": {"umbrella": 0.9, "no umbrella": 0.1}
    },
    initial={"sunny": 0.5, "rainy": 0.5}
)

observations = ["umbrella", "umbrella", "no umbrella", "umbrella"]
forward_probs = hmm.forward(observations)
best_path = hmm.viterbi(observations)

print("Hidden Markov Model - Weather Inference:")
for t, (obs, dist) in enumerate(zip(observations, forward_probs)):
    print(f"  Day {t+1} (obs={obs}): P(sunny)={dist['sunny']:.3f}, P(rainy)={dist['rainy']:.3f}")

print(f"\nMost likely weather sequence: {' → '.join(best_path)}")
```

---

# CS50 Lecture 3 — Optimization {#lec3}

---

## 📋 [CS50 AI NOTES] — Optimization

### Local Search

- **Local search**: algorithms that maintain a single node and search by moving to a neighbor
- Used when we care about the **solution** (state), not the path

### Hill Climbing

```python
function Hill-Climb(problem):
    current = initial state of problem
    repeat:
        neighbor = best valued neighbor of current
        if neighbor not better than current: return current
        current = neighbor
```

**Problems**: gets stuck at **local maxima/minima**, **flat local maxima**, **shoulders**

**Variants**:

- Steepest ascent, stochastic, first-choice, random-restart, local beam search

### Simulated Annealing

- Early on: high "temperature" → more likely to accept worse neighbors
- Later: low temperature → behave more like hill climbing
- Key: allows escaping local optima probabilistically

### Linear Programming

- **Minimize**: `c1x1 + c2x2 + ...` (cost function)
- **Constraints**: `ai1x1 + ai2x2 + ... ≤ bi`, `xi ≥ 0`
- **Algorithms**: Simplex, Interior-Point
- CS50 Example: two machines X1, X2; minimize `50x1 + 80x2` subject to labor/output constraints

### Constraint Satisfaction Problems (CSP)

- **Variables**: `{X1, X2, ..., Xn}`
- **Domains**: `Di` for each variable
- **Constraints**: sets of allowable combinations
- **Hard constraints**: must be satisfied
- **Soft constraints**: express preference
- **Arc consistency**: when all values in a variable's domain satisfy binary constraints
    - `AC-3`: queue-based algorithm to enforce arc consistency
- **Backtracking Search**: DFS through assignments, prune early
- **Heuristics**:
    - **MRV (Minimum Remaining Values)**: pick variable with smallest domain
    - **Degree heuristic**: pick variable involved in most constraints
    - **Least-constraining value**: pick value that rules out fewest neighbors
- **Inference**: Maintaining Arc Consistency (MAC) — run AC-3 after each assignment

---

## 🔵 [ENHANCED] — Optimization Algorithms in Python

```python
import random
import math
from typing import Callable, List, Tuple, Optional

# ============================================================
# HILL CLIMBING (CS50 Lecture 3)
# ============================================================

def hill_climbing(initial_state, 
                  neighbors_fn: Callable,
                  objective_fn: Callable,
                  maximize: bool = True) -> Tuple:
    """
    Hill Climbing: iteratively move to best neighbor.
    CS50: 'maintain single node, search by moving to neighbor'
    
    Returns: (best_state, best_value)
    """
    current = initial_state
    current_val = objective_fn(current)
    
    while True:
        neighbors = neighbors_fn(current)
        if not neighbors:
            break
        
        best_neighbor = max(neighbors, key=objective_fn) if maximize \
                       else min(neighbors, key=objective_fn)
        best_val = objective_fn(best_neighbor)
        
        # Stop if no improvement (stuck at local optima)
        if (maximize and best_val <= current_val) or \
           (not maximize and best_val >= current_val):
            break
        
        current = best_neighbor
        current_val = best_val
    
    return current, current_val


# ============================================================
# SIMULATED ANNEALING (CS50: escape local optima)
# ============================================================

def simulated_annealing(initial_state,
                        neighbors_fn: Callable,
                        objective_fn: Callable,
                        temperature_fn: Callable,
                        max_iter: int = 10000,
                        maximize: bool = True) -> Tuple:
    """
    Simulated Annealing.
    CS50: 'early on, higher temperature: more likely to accept worse neighbors'
    
    temperature_fn(t) → temperature at iteration t
    Accept worse solution with probability e^(ΔE/T)
    """
    current = initial_state
    current_val = objective_fn(current)
    best = current
    best_val = current_val
    
    for t in range(1, max_iter + 1):
        T = temperature_fn(t)
        if T <= 0:
            break
        
        # Pick random neighbor
        neighbors = neighbors_fn(current)
        if not neighbors:
            break
        neighbor = random.choice(neighbors)
        neighbor_val = objective_fn(neighbor)
        
        # Change in objective
        delta = (neighbor_val - current_val) if maximize else (current_val - neighbor_val)
        
        # Accept better always; accept worse with probability e^(delta/T)
        if delta > 0 or random.random() < math.exp(delta / T):
            current = neighbor
            current_val = neighbor_val
        
        # Track global best
        if (maximize and current_val > best_val) or \
           (not maximize and current_val < best_val):
            best = current
            best_val = current_val
    
    return best, best_val


# ============================================================
# CONSTRAINT SATISFACTION: N-Queens with AC-3 + Backtracking
# ============================================================

class CSP:
    """
    General Constraint Satisfaction Problem.
    CS50 formulation: variables, domains, constraints.
    """
    def __init__(self):
        self.variables = []
        self.domains = {}
        self.constraints = []  # list of (var1, var2, constraint_fn)
    
    def add_variable(self, var, domain: List):
        self.variables.append(var)
        self.domains[var] = list(domain)
    
    def add_constraint(self, var1, var2, constraint_fn: Callable):
        """constraint_fn(val1, val2) → True if consistent."""
        self.constraints.append((var1, var2, constraint_fn))
    
    def is_consistent(self, var, value, assignment: Dict) -> bool:
        """Check if assigning value to var is consistent with current assignment."""
        for v1, v2, fn in self.constraints:
            if v1 == var and v2 in assignment:
                if not fn(value, assignment[v2]):
                    return False
            if v2 == var and v1 in assignment:
                if not fn(assignment[v1], value):
                    return False
        return True
    
    def ac3(self) -> bool:
        """
        AC-3 Algorithm: enforce arc consistency.
        CS50: 'when all values in X's domain satisfy binary constraint with Y'
        Returns False if domain becomes empty (no solution possible).
        """
        queue = [(v1, v2) for v1, v2, _ in self.constraints] + \
                [(v2, v1) for v1, v2, _ in self.constraints]
        
        while queue:
            (X, Y) = queue.pop(0)
            if self._revise(X, Y):
                if len(self.domains[X]) == 0:
                    return False  # No solution
                # Add all neighbors of X back to queue
                neighbors = [v1 for v1, v2, _ in self.constraints if v2 == X and v1 != Y] + \
                           [v2 for v1, v2, _ in self.constraints if v1 == X and v2 != Y]
                for Z in neighbors:
                    queue.append((Z, X))
        return True
    
    def _revise(self, X, Y) -> bool:
        """Remove values from X's domain inconsistent with any value in Y's domain."""
        revised = False
        for x_val in self.domains[X][:]:  # copy to allow deletion
            # Is there any y_val in Y's domain consistent with x_val?
            consistent = False
            for y_val in self.domains[Y]:
                for v1, v2, fn in self.constraints:
                    if (v1 == X and v2 == Y and fn(x_val, y_val)) or \
                       (v1 == Y and v2 == X and fn(y_val, x_val)):
                        consistent = True
                        break
                if consistent:
                    break
            if not consistent:
                self.domains[X].remove(x_val)
                revised = True
        return revised
    
    def backtrack(self, assignment: Dict = None) -> Optional[Dict]:
        """
        Backtracking search with MRV heuristic.
        CS50: 'select variable with smallest domain (MRV)'
        """
        if assignment is None:
            assignment = {}
        
        if len(assignment) == len(self.variables):
            return assignment  # Complete assignment
        
        # MRV: select unassigned variable with smallest domain
        unassigned = [v for v in self.variables if v not in assignment]
        var = min(unassigned, key=lambda v: len(self.domains[v]))
        
        for value in self.domains[var]:
            if self.is_consistent(var, value, assignment):
                assignment[var] = value
                result = self.backtrack(assignment)
                if result is not None:
                    return result
                del assignment[var]
        
        return None  # No solution found


# ============================================================
# DEMO: Course Scheduling CSP (CS50 example)
# ============================================================
# Variables: courses {A, B, C, D, E, F, G}
# Domain: days {Monday, Tuesday, Wednesday}
# Constraints: certain pairs can't be on same day

csp = CSP()
courses = ['A', 'B', 'C', 'D', 'E', 'F', 'G']
days = ['Monday', 'Tuesday', 'Wednesday']

for course in courses:
    csp.add_variable(course, days)

# Conflict constraints (courses that can't be on same day)
conflicts = [('A','B'),('A','C'),('B','C'),('B','D'),('C','E'),('D','E'),('D','F'),('E','G'),('F','G')]
for c1, c2 in conflicts:
    csp.add_constraint(c1, c2, lambda v1, v2: v1 != v2)

# Enforce arc consistency first
csp.ac3()
print("After AC-3, domain sizes:", {v: len(d) for v, d in csp.domains.items()})

# Solve with backtracking
solution = csp.backtrack()
print(f"\nSchedule: {solution}")
```

---

# CS50 Lecture 4 — Learning {#lec4}

---

## 📋 [CS50 AI NOTES] — Machine Learning (CS50 Extension)

### Supervised Learning

- **Classification**: map input to discrete category
- **Regression**: map input to continuous value
- **Nearest-Neighbor**: given input, choose class of nearest data point
- **k-Nearest-Neighbor**: choose most common class among k nearest points
- **Perceptron learning rule**: `wi = wi + α(y - hw(x)) * xi`
- **Support Vector Machines**: find maximum margin separator (boundary maximizing distance from data points)
- **Soft threshold (sigmoid)** vs **Hard threshold (step function)**

### Loss Functions

- **0-1 loss**: `L(actual, predicted) = 0 if same, 1 if different`
- **L1 loss**: `L(actual, predicted) = |actual - predicted|`
- **L2 loss**: `L(actual, predicted) = (actual - predicted)²`

### Regularization

`cost(h) = loss(h) + λ * complexity(h)` — penalize complex hypotheses

### Validation

- **Holdout cross-validation**: split into train/test
- **k-fold cross-validation**: split into k sets, experiment with each as validation

### Reinforcement Learning

- **Goal**: given rewards/punishments, learn what actions to take
- **Markov Decision Process**: states, actions, reward function, transition model `P(s' | s, a)`
- **Q-learning**: learn `Q(s, a)` = estimate of value of taking action a in state s
    - Start: `Q(s, a) = 0` for all s, a
    - Update: `Q(s, a) ← Q(s, a) + α((r + max_a' Q(s', a')) - Q(s, a))`
    - Greedy: in state s, choose action a with highest Q(s, a)
- **ε-greedy**: with probability ε explore randomly, else exploit best known action

### Unsupervised Learning

- **k-means clustering**: assign points to k clusters, update centers, repeat

---

## 🔵 [ENHANCED] — Q-Learning from Scratch

```python
import numpy as np
import random
from collections import defaultdict

# ============================================================
# Q-LEARNING (CS50 Lecture 4)
# 'learn Q(s,a): estimate of value of action a in state s'
# ============================================================

class QLearningAgent:
    """
    Q-Learning Agent.
    CS50: 'Every time we take action a in state s and observe reward r, update Q(s,a)'
    """
    def __init__(self, alpha: float = 0.5, gamma: float = 0.9, epsilon: float = 0.1):
        self.alpha = alpha      # learning rate
        self.gamma = gamma      # discount factor (future reward weight)
        self.epsilon = epsilon  # exploration rate (ε-greedy)
        self.Q = defaultdict(float)   # Q(state, action) → value
    
    def get_q(self, state, action) -> float:
        return self.Q[(state, action)]
    
    def best_action(self, state, actions: List):
        """Greedy: pick action with highest Q value."""
        return max(actions, key=lambda a: self.get_q(state, a))
    
    def choose_action(self, state, actions: List) -> Any:
        """
        ε-greedy policy:
        - With probability ε: explore (random action)
        - With probability 1-ε: exploit (best known action)
        """
        if random.random() < self.epsilon:
            return random.choice(actions)   # explore
        return self.best_action(state, actions)  # exploit
    
    def update(self, state, action, reward: float, next_state, next_actions: List):
        """
        CS50 Q-learning update:
        Q(s,a) ← Q(s,a) + α * ((r + γ * max_a' Q(s',a')) - Q(s,a))
        
        Breakdown:
        - r + γ * max Q(s',a') = new value estimate
        - Q(s,a) = old value estimate  
        - difference = "surprise" (temporal difference error)
        """
        old_q = self.get_q(state, action)
        
        if next_actions:
            future_reward = self.gamma * max(self.get_q(next_state, a) 
                                            for a in next_actions)
        else:
            future_reward = 0  # terminal state
        
        new_q = old_q + self.alpha * ((reward + future_reward) - old_q)
        self.Q[(state, action)] = new_q
    
    def train(self, env, episodes: int = 10000):
        """Train agent in environment for given number of episodes."""
        rewards_history = []
        
        for episode in range(episodes):
            state = env.reset()
            total_reward = 0
            done = False
            
            while not done:
                actions = env.available_actions(state)
                action = self.choose_action(state, actions)
                next_state, reward, done = env.step(state, action)
                next_actions = env.available_actions(next_state) if not done else []
                
                self.update(state, action, reward, next_state, next_actions)
                state = next_state
                total_reward += reward
            
            rewards_history.append(total_reward)
        
        return rewards_history


# ============================================================
# DEMO: Simple Grid World Environment
# ============================================================

class GridWorld:
    """
    3x4 grid world. Agent navigates from (0,0) to goal (0,3).
    Obstacles at (1,1). Penalty at (0,3) or reward.
    """
    def __init__(self):
        self.rows, self.cols = 3, 4
        self.goal = (0, 3)
        self.trap = (1, 3)
        self.wall = (1, 1)
    
    def reset(self):
        return (2, 0)  # start position
    
    def available_actions(self, state):
        r, c = state
        actions = []
        for dr, dc, a in [(-1,0,'up'),(1,0,'down'),(0,-1,'left'),(0,1,'right')]:
            nr, nc = r+dr, c+dc
            if 0 <= nr < self.rows and 0 <= nc < self.cols and (nr,nc) != self.wall:
                actions.append(a)
        return actions
    
    def step(self, state, action):
        r, c = state
        moves = {'up':(-1,0),'down':(1,0),'left':(0,-1),'right':(0,1)}
        dr, dc = moves[action]
        next_state = (r+dr, c+dc)
        
        if next_state == self.goal:
            return next_state, +1.0, True
        elif next_state == self.trap:
            return next_state, -1.0, True
        else:
            return next_state, -0.04, False   # small step penalty

env = GridWorld()
agent = QLearningAgent(alpha=0.5, gamma=0.9, epsilon=0.3)
rewards = agent.train(env, episodes=5000)

# Show learned policy
print("=== Learned Q-Learning Policy ===")
grid = [['.' for _ in range(4)] for _ in range(3)]
grid[1][1] = '#'  # wall
grid[0][3] = 'G'  # goal
grid[1][3] = 'T'  # trap

for r in range(3):
    for c in range(4):
        state = (r, c)
        if grid[r][c] in '.':
            actions = env.available_actions(state)
            if actions:
                best = agent.best_action(state, actions)
                arrows = {'up':'↑','down':'↓','left':'←','right':'→'}
                grid[r][c] = arrows[best]

for row in grid:
    print('  ' + ' '.join(row))

print(f"\nAverage reward (last 500 episodes): {np.mean(rewards[-500:]):.3f}")
```

---

# CS50 Lecture 5 — Neural Networks {#lec5}

---

## 📋 [CS50 AI NOTES] — Neural Networks

### Neuron Model

- **Activation functions**:
    - **Step**: `g(x) = 1 if x≥0 else 0`
    - **Logistic sigmoid**: `g(x) = eˣ / (1 + eˣ)`
    - **ReLU**: `g(x) = max(0, x)` — most common in deep learning
- **Perceptron**: `g(w0 + w1*x1 + w2*x2 + ... + wn*xn)`

### Gradient Descent

- **Batch**: use all data points → most stable
- **Stochastic**: use one data point → faster but noisy
- **Mini-batch**: use small batch → best balance

### Backpropagation

- CS50: "Algorithm for training neural networks with hidden layers"
- Calculate error for output layer → propagate error back, layer by layer

### Architecture Types

- **Multilayer neural network**: input layer + hidden layers + output layer
- **Deep neural networks**: multiple hidden layers
- **Feed-forward**: connections only in one direction

### Overfitting Prevention

- **Dropout**: temporarily remove random units during training
    - CS50: "prevent over-reliance on certain neurons"

### Computer Vision

- **Image convolution**: apply kernel filter to image
- **Pooling**: reduce size by sampling (max-pooling: take maximum in region)
- **Convolutional Neural Network (CNN)**: uses convolution + pooling layers → detects edges, curves, shapes
- **Computer vision**: "methods for analyzing and understanding digital images"

### Recurrent Neural Networks

- Output feeds back as input
- CS50: "generates output that feeds back into its own inputs"
- Used for sequences: text, speech, time series

---

## 🔵 [ENHANCED] — Neural Networks in Python

```python
import numpy as np
from typing import List, Tuple, Callable

# ============================================================
# ACTIVATION FUNCTIONS (CS50 Lecture 5)
# ============================================================

def step(x: np.ndarray) -> np.ndarray:
    """CS50: g(x) = 1 if x >= 0, else 0"""
    return (x >= 0).astype(float)

def sigmoid(x: np.ndarray) -> np.ndarray:
    """CS50 logistic sigmoid: g(x) = e^x / (1 + e^x)"""
    return 1 / (1 + np.exp(-np.clip(x, -500, 500)))

def sigmoid_derivative(x: np.ndarray) -> np.ndarray:
    s = sigmoid(x)
    return s * (1 - s)

def relu(x: np.ndarray) -> np.ndarray:
    """CS50 ReLU: g(x) = max(0, x) — most common in deep learning"""
    return np.maximum(0, x)

def relu_derivative(x: np.ndarray) -> np.ndarray:
    return (x > 0).astype(float)


# ============================================================
# NEURAL NETWORK WITH BACKPROPAGATION (CS50 Lecture 5)
# ============================================================

class NeuralNetwork:
    """
    Multilayer neural network trained via backpropagation.
    CS50: 'input layer + at least one hidden layer + output layer'
    """
    
    def __init__(self, layer_sizes: List[int], learning_rate: float = 0.01):
        """
        layer_sizes: e.g., [2, 4, 4, 1] = 2 inputs, two hidden layers of 4, 1 output
        """
        self.lr = learning_rate
        self.weights = []
        self.biases = []
        
        # Initialize weights (He initialization for ReLU)
        for i in range(len(layer_sizes) - 1):
            w = np.random.randn(layer_sizes[i], layer_sizes[i+1]) * np.sqrt(2/layer_sizes[i])
            b = np.zeros((1, layer_sizes[i+1]))
            self.weights.append(w)
            self.biases.append(b)
    
    def forward(self, X: np.ndarray) -> Tuple[List, List]:
        """
        Forward pass: compute activations at each layer.
        Returns: (pre_activations, activations) for each layer.
        """
        pre_acts = []   # z = Xw + b (before activation)
        acts = [X]      # a = g(z) (after activation); a[0] = input
        
        for i, (W, b) in enumerate(zip(self.weights, self.biases)):
            z = acts[-1] @ W + b
            pre_acts.append(z)
            
            # ReLU for hidden, sigmoid for output
            if i < len(self.weights) - 1:
                a = relu(z)
            else:
                a = sigmoid(z)
            acts.append(a)
        
        return pre_acts, acts
    
    def backward(self, X: np.ndarray, y: np.ndarray,
                 pre_acts: List, acts: List):
        """
        CS50 Backpropagation:
        'Calculate error for output layer.
         For each layer, starting from output, propagate error back.'
        """
        m = X.shape[0]
        
        # Output layer error (binary cross-entropy gradient)
        delta = acts[-1] - y.reshape(-1, 1)   # (m, output_size)
        
        # Backpropagate through layers
        for i in reversed(range(len(self.weights))):
            # Gradient of weights and biases
            dW = (acts[i].T @ delta) / m
            db = np.mean(delta, axis=0, keepdims=True)
            
            # Update weights
            self.weights[i] -= self.lr * dW
            self.biases[i] -= self.lr * db
            
            if i > 0:
                # Propagate error to previous layer
                delta = (delta @ self.weights[i].T) * relu_derivative(pre_acts[i-1])
    
    def train(self, X: np.ndarray, y: np.ndarray,
              epochs: int = 1000, batch_size: int = 32) -> List[float]:
        """Mini-batch gradient descent training."""
        losses = []
        m = X.shape[0]
        
        for epoch in range(epochs):
            # Shuffle data
            idx = np.random.permutation(m)
            X_s, y_s = X[idx], y[idx]
            
            epoch_loss = 0
            for i in range(0, m, batch_size):
                X_b = X_s[i:i+batch_size]
                y_b = y_s[i:i+batch_size]
                
                pre_acts, acts = self.forward(X_b)
                self.backward(X_b, y_b, pre_acts, acts)
                
                # Binary cross-entropy loss
                y_pred = acts[-1].flatten()
                y_true = y_b
                eps = 1e-15
                y_pred = np.clip(y_pred, eps, 1-eps)
                loss = -np.mean(y_true * np.log(y_pred) + (1-y_true) * np.log(1-y_pred))
                epoch_loss += loss
            
            losses.append(epoch_loss)
        
        return losses
    
    def predict(self, X: np.ndarray, threshold: float = 0.5) -> np.ndarray:
        _, acts = self.forward(X)
        return (acts[-1].flatten() >= threshold).astype(int)


# ============================================================
# DROPOUT LAYER (CS50: prevent over-reliance on certain neurons)
# ============================================================

class DropoutLayer:
    """
    CS50: 'temporarily removing units selected at random from network
    to prevent over-reliance on certain neurons'
    """
    def __init__(self, dropout_rate: float = 0.5):
        self.rate = dropout_rate
        self.mask = None
    
    def forward(self, X: np.ndarray, training: bool = True) -> np.ndarray:
        if training:
            # Random mask: keep neuron with prob (1 - rate)
            self.mask = (np.random.rand(*X.shape) > self.rate) / (1 - self.rate)
            return X * self.mask
        return X  # No dropout during inference
    
    def backward(self, dout: np.ndarray) -> np.ndarray:
        return dout * self.mask


# ============================================================
# IMAGE CONVOLUTION (CS50 Lecture 5)
# ============================================================

def convolve2d(image: np.ndarray, kernel: np.ndarray) -> np.ndarray:
    """
    CS50: 'applying a filter that adds each pixel value to its neighbors,
    weighted according to a kernel matrix'
    """
    h, w = image.shape
    kh, kw = kernel.shape
    out_h = h - kh + 1
    out_w = w - kw + 1
    output = np.zeros((out_h, out_w))
    
    for i in range(out_h):
        for j in range(out_w):
            output[i, j] = np.sum(image[i:i+kh, j:j+kw] * kernel)
    
    return output


def max_pool(feature_map: np.ndarray, pool_size: int = 2) -> np.ndarray:
    """
    CS50: 'max-pooling: reducing size by choosing maximum value in each region'
    """
    h, w = feature_map.shape
    ph, pw = h // pool_size, w // pool_size
    output = np.zeros((ph, pw))
    
    for i in range(ph):
        for j in range(pw):
            region = feature_map[i*pool_size:(i+1)*pool_size,
                                j*pool_size:(j+1)*pool_size]
            output[i, j] = np.max(region)
    
    return output


# Demo: Edge detection with convolution
image = np.array([
    [0, 0, 0, 0, 0],
    [0, 1, 1, 1, 0],
    [0, 1, 1, 1, 0],
    [0, 1, 1, 1, 0],
    [0, 0, 0, 0, 0],
], dtype=float)

# Sobel edge detection kernel
edge_kernel = np.array([
    [-1, 0, 1],
    [-2, 0, 2],
    [-1, 0, 1],
])

edges = convolve2d(image, edge_kernel)
pooled = max_pool(edges)
print("Original image:\n", image.astype(int))
print("After edge kernel:\n", edges.astype(int))
print("After max-pooling:\n", pooled.astype(int))
```

---

# CS50 Lecture 6 — Language (NLP) {#lec6}

---

## 📋 [CS50 AI NOTES] — Natural Language Processing

- **NLP tasks**: automatic summarization, information extraction, machine translation, text classification, sentiment analysis

### Word Representation

- **one-hot representation**: vector with single 1, rest 0s. Problem: doesn't capture meaning/similarity
- **distributed representation**: meaning distributed across multiple values (dense vector)
- **word2vec**: CS50 says "model for generating word vectors" — famous examples:
    - `king - man + woman ≈ queen`
    - `breakfast`, `capital of Massachusetts is Boston`
- J. R. Firth (1957): _"You shall know a word by the company it keeps."_

### n-gram Language Models

- **n-gram**: contiguous sequence of n items from a sample of text
- Used to estimate probability of next word given previous n-1 words

### Text Classification

- **Bag-of-words model**: represent text as unordered collection of words
- **Naive Bayes**: `P(class | words) ∝ P(class) × Π P(word | class)`
- **Additive/Laplace smoothing**: add 1 to each count to avoid zero probabilities
    - `P(word | class) = (count + 1) / (total_words_in_class + vocab_size)`

### Formal Grammar

- **Context-Free Grammar (CFG)**: system of rules for generating sentences
    - Rules like: `S → NP VP`, `NP → N | D N`, `VP → V | V NP`

### Attention & Transformers

- **Hidden state**: encodes context in RNN
- **Attention**: allows model to focus on different parts of input when generating output
- CS50 references Bahdanau et al. 2015: "Neural machine translation by jointly learning to align and translate"
- **Transformers**: use attention without RNN — enables parallelization (basis of GPT, BERT)
- **Positional representations**: since transformers have no recurrence, position must be encoded explicitly

---

## 🔵 [ENHANCED] — NLP in Python

```python
import re
import math
from collections import Counter, defaultdict
from typing import Dict, List, Tuple

# ============================================================
# NAIVE BAYES TEXT CLASSIFIER (CS50 Lecture 6)
# ============================================================

class NaiveBayesClassifier:
    """
    CS50: Bag-of-words + Naive Bayes for text classification.
    P(class | words) ∝ P(class) × Π P(word | class)
    """
    
    def __init__(self, laplace_smoothing: float = 1.0):
        self.k = laplace_smoothing  # CS50 calls this additive/Laplace smoothing
        self.class_priors = {}
        self.word_probs = {}         # class → word → P(word | class)
        self.vocab = set()
    
    def tokenize(self, text: str) -> List[str]:
        """Simple tokenization."""
        return re.findall(r'\b[a-z]+\b', text.lower())
    
    def train(self, documents: List[Tuple[str, str]]):
        """
        Train on (text, label) pairs.
        CS50: 'bag-of-words: represent text as unordered collection of words'
        """
        class_counts = Counter()
        word_counts = defaultdict(Counter)
        
        for text, label in documents:
            words = self.tokenize(text)
            class_counts[label] += 1
            word_counts[label].update(words)
            self.vocab.update(words)
        
        total_docs = sum(class_counts.values())
        self.class_priors = {c: count/total_docs for c, count in class_counts.items()}
        
        # Compute P(word | class) with Laplace smoothing
        # CS50: 'padding 1 to each value... one more time than we actually have'
        V = len(self.vocab)
        for label, counts in word_counts.items():
            total = sum(counts.values())
            self.word_probs[label] = {
                word: (counts[word] + self.k) / (total + self.k * V)
                for word in self.vocab
            }
            self.word_probs[label]['<UNK>'] = self.k / (total + self.k * V)
    
    def classify(self, text: str) -> Dict[str, float]:
        """
        Compute P(class | text) for each class.
        Work in log space to avoid underflow.
        """
        words = self.tokenize(text)
        scores = {}
        
        for label, prior in self.class_priors.items():
            # Log probability
            score = math.log(prior)
            for word in words:
                prob = self.word_probs[label].get(word, 
                       self.word_probs[label]['<UNK>'])
                score += math.log(prob)
            scores[label] = score
        
        # Convert log probs to probabilities (normalized)
        max_score = max(scores.values())
        exp_scores = {k: math.exp(v - max_score) for k, v in scores.items()}
        total = sum(exp_scores.values())
        return {k: v/total for k, v in exp_scores.items()}
    
    def predict(self, text: str) -> str:
        scores = self.classify(text)
        return max(scores, key=scores.get)


# Demo: Sentiment analysis (CS50 movie review example)
training_data = [
    ("One of the best games I've played in a long time", "positive"),
    ("Amazing experience, highly recommend", "positive"),
    ("Wonderful, loved every moment", "positive"),
    ("cheap and flimsy, not worth it", "negative"),
    ("terrible product, complete waste of money", "negative"),
    ("Awful, broke after one day", "negative"),
    ("My grandson loved it", "positive"),
    ("Do not buy this, very disappointing", "negative"),
]

clf = NaiveBayesClassifier(laplace_smoothing=1.0)
clf.train(training_data)

test_cases = [
    "loved this amazing product",
    "terrible, not worth buying",
    "great experience overall",
]

print("=== Naive Bayes Sentiment Analysis ===")
for text in test_cases:
    scores = clf.classify(text)
    pred = clf.predict(text)
    print(f"'{text}'")
    print(f"  → {pred} (pos={scores.get('positive',0):.3f}, neg={scores.get('negative',0):.3f})")


# ============================================================
# N-GRAM LANGUAGE MODEL (CS50: 'contiguous sequence of n items')
# ============================================================

class NGramModel:
    """
    CS50: n-gram = 'contiguous sequence of n items from a sample of text'
    Used to estimate P(next_word | previous_words).
    """
    def __init__(self, n: int = 2):
        self.n = n
        self.ngram_counts = Counter()
        self.context_counts = Counter()
    
    def train(self, text: str):
        tokens = ['<START>'] * (self.n-1) + self.tokenize(text) + ['<END>']
        for i in range(len(tokens) - self.n + 1):
            ngram = tuple(tokens[i:i+self.n])
            context = ngram[:-1]
            self.ngram_counts[ngram] += 1
            self.context_counts[context] += 1
    
    def tokenize(self, text: str) -> List[str]:
        return re.findall(r'\b\w+\b', text.lower())
    
    def probability(self, word: str, context: Tuple[str, ...]) -> float:
        """P(word | context) with Laplace smoothing."""
        ngram = context + (word,)
        count = self.ngram_counts.get(ngram, 0) + 1  # Laplace
        ctx_count = self.context_counts.get(context, 0) + len(self.ngram_counts)
        return count / ctx_count
    
    def generate(self, max_words: int = 20) -> str:
        """Generate text by sampling from the model."""
        context = tuple(['<START>'] * (self.n-1))
        result = []
        
        for _ in range(max_words):
            # Find all possible next words
            possible = [ngram[-1] for ngram in self.ngram_counts 
                       if ngram[:-1] == context]
            if not possible or '<END>' in (possible if len(possible) == 1 else []):
                break
            
            # Weight by probability
            probs = [self.probability(w, context) for w in possible]
            total = sum(probs)
            probs = [p/total for p in probs]
            
            next_word = possible[int(math.log(sum(probs)) * len(probs)) % len(possible)]
            if next_word == '<END>':
                break
            result.append(next_word)
            context = context[1:] + (next_word,)
        
        return ' '.join(result)


# ============================================================
# WORD EMBEDDINGS: word2vec intuition (CS50 Lecture 6)
# ============================================================

def cosine_similarity(a: np.ndarray, b: np.ndarray) -> float:
    """Measure similarity between word vectors."""
    return np.dot(a, b) / (np.linalg.norm(a) * np.linalg.norm(b) + 1e-10)

# CS50 famous analogy: king - man + woman ≈ queen
# Conceptual demo with fake embeddings
word_vectors = {
    'king':  np.array([0.9, 0.1, 0.8, 0.1]),
    'man':   np.array([0.8, 0.1, 0.1, 0.1]),
    'woman': np.array([0.1, 0.9, 0.1, 0.1]),
    'queen': np.array([0.2, 0.9, 0.8, 0.1]),
    'royal': np.array([0.6, 0.6, 0.9, 0.1]),
}

# word2vec analogy: king - man + woman
query = word_vectors['king'] - word_vectors['man'] + word_vectors['woman']

print("\n=== Word2Vec Analogy: king - man + woman ≈ ? ===")
similarities = {word: cosine_similarity(query, vec) 
               for word, vec in word_vectors.items() if word != 'king'}
for word, sim in sorted(similarities.items(), key=lambda x: -x[1]):
    print(f"  {word}: {sim:.4f}")
```

---

## 🔵 [ENHANCED] — The Transformer Attention Mechanism

```python
import numpy as np

# ============================================================
# SELF-ATTENTION (basis of Transformers, CS50 Lecture 6)
# CS50: 'Attention' and 'Transformers' — Bahdanau et al. 2015
# ============================================================

def scaled_dot_product_attention(Q: np.ndarray, K: np.ndarray, 
                                  V: np.ndarray) -> np.ndarray:
    """
    Core of the Transformer: 'Attention is All You Need' (Vaswani et al. 2017)
    
    Q = Query, K = Key, V = Value
    Attention(Q,K,V) = softmax(QK^T / sqrt(d_k)) V
    
    Intuition: 
    - Q: what am I looking for?
    - K: what do I have to offer?  
    - V: what I actually contain
    - QK^T: how relevant is each position to each query?
    - /sqrt(d_k): scale to prevent vanishing gradients
    """
    d_k = Q.shape[-1]
    
    # Attention scores
    scores = Q @ K.T / np.sqrt(d_k)
    
    # Softmax (convert to probabilities)
    scores_exp = np.exp(scores - np.max(scores, axis=-1, keepdims=True))
    attention_weights = scores_exp / scores_exp.sum(axis=-1, keepdims=True)
    
    # Weighted sum of values
    output = attention_weights @ V
    return output, attention_weights


# Demo: Simplified attention on a sentence
# "The cat sat on the mat"
# Each word gets a 4-dim embedding
np.random.seed(42)
sentence = ["The", "cat", "sat", "on", "the", "mat"]
seq_len = len(sentence)
d_model = 4

# Fake embeddings (in reality, learned or from word2vec)
embeddings = np.random.randn(seq_len, d_model)

# Projection matrices (learned during training)
W_Q = np.random.randn(d_model, d_model)
W_K = np.random.randn(d_model, d_model)
W_V = np.random.randn(d_model, d_model)

Q = embeddings @ W_Q
K = embeddings @ W_K
V = embeddings @ W_V

output, weights = scaled_dot_product_attention(Q, K, V)

print("=== Self-Attention Weights ===")
print("(Row i, Col j = how much word i attends to word j)")
print(f"\n{'':8}", end='')
for w in sentence: print(f"{w:8}", end='')
print()
for i, word in enumerate(sentence):
    print(f"{word:8}", end='')
    for j in range(len(sentence)):
        print(f"{weights[i,j]:.4f}  ", end='')
    print()
```

---

## 🔵 [ENHANCED] — Cross-Topic Big Picture: CS50 AI + Your University Course

```
CS50 AI Complete Map
│
├── Lecture 0: Search ────────────────────────────── Lecture 3 (Uni)
│   ├── BFS (Queue/FIFO) → shortest path
│   ├── DFS (Stack/LIFO) → memory efficient
│   ├── Greedy Best-First → fast, h(n) only
│   ├── A* → optimal, f(n)=g(n)+h(n)
│   └── Minimax + Alpha-Beta → game playing
│
├── Lecture 1: Knowledge ─────────────────────────── Lecture 2 (Uni)
│   ├── Propositional Logic → model checking
│   ├── Inference Rules → resolution
│   └── First-Order Logic → universal/existential quantification
│
├── Lecture 2: Uncertainty ─────────────────────────── NEW (extends Uni L2)
│   ├── Bayes' Rule
│   ├── Bayesian Networks
│   └── Hidden Markov Models
│
├── Lecture 3: Optimization ──────────────────────────── NEW
│   ├── Hill Climbing (local search)
│   ├── Simulated Annealing
│   ├── Linear Programming
│   └── CSP + Arc Consistency + Backtracking
│
├── Lecture 4: Learning ─────────────────────────── Lectures 4-6 (Uni)
│   ├── Supervised: SVM, kNN, Perceptron
│   ├── Reinforcement: Q-learning, MDP
│   └── Unsupervised: k-means
│
├── Lecture 5: Neural Networks ──────────────────────── NEW (extends L6 Uni)
│   ├── Backpropagation
│   ├── ReLU, Sigmoid, Step
│   ├── Dropout, Regularization
│   └── CNNs: Convolution + Pooling
│
└── Lecture 6: Language ─────────────────────────────── NEW
    ├── Naive Bayes (text classification)
    ├── n-gram models
    ├── word2vec embeddings
    └── Attention + Transformers
```

---

## 🔵 [ENHANCED] — Master Practice Problem Set

### Tier 1: Foundational

1. Implement BFS on a word-ladder puzzle (e.g., COLD → WARM in single-letter steps)
2. Build a propositional logic truth table generator for any formula
3. Write a Naive Bayes spam filter trained on 20 emails

### Tier 2: Intermediate

```python
# Implement the 8-puzzle solver using A*
# State: tuple of 9 numbers (0 = blank)
# e.g., (1,2,3,4,0,6,7,5,8) → goal: (1,2,3,4,5,6,7,8,0)

class EightPuzzle(SearchProblem):
    """
    YOUR TASK: Implement this classic AI problem.
    
    Heuristic ideas:
    - Misplaced tiles (admissible)
    - Manhattan distance sum (better admissible heuristic)
    """
    def initial_state(self): ...
    def actions(self, state): ...
    def result(self, state, action): ...
    def is_goal(self, state): ...
    
    def manhattan_heuristic(self, state) -> int:
        """Sum of Manhattan distances of each tile from its goal position."""
        pass  # YOUR TASK
```

### Tier 3: Advanced

> **Project**: Build a complete **Minesweeper AI** that:
> 
> 1. Uses propositional logic to infer safe squares (KB + model checking)
> 2. Uses probability to estimate mine locations when logic alone is insufficient
> 3. Falls back to random safe choice when uncertain This is literally CS50's Problem Set 1!

---

## 🔵 [ENHANCED] — Key Equations Reference Sheet

```
SEARCH
  A*:         f(n) = g(n) + h(n)
  Alpha-Beta: prune if alpha ≥ beta

PROBABILITY
  Chain Rule: P(a ∧ b) = P(a)P(b|a)
  Bayes:      P(a|b) = P(b|a)P(a) / P(b)
  Total Prob: P(a) = Σb P(a,b)

ML: LINEAR REGRESSION
  Loss:    E(w) = (1/2m) Σ(yt - yp)²
  Update:  w ← w + η * (1/m) Σ(yt - yp)x

ML: LOGISTIC REGRESSION
  Sigmoid: σ(z) = 1/(1+e^-z)
  Loss:    J = -(1/m) Σ[y*log(yp) + (1-y)*log(1-yp)]

NEURAL NETWORKS
  Forward:  z = Xw + b,  a = g(z)
  Error:    δ = ∂L/∂z
  Backprop: ∂L/∂W = X^T δ

Q-LEARNING
  Update: Q(s,a) ← Q(s,a) + α[(r + γ*max Q(s',a')) - Q(s,a)]

NLP: NAIVE BAYES
  Classify: argmax_c P(c) Π P(wi|c)
  Smoothed: P(w|c) = (count(w,c)+1) / (count(c)+|V|)
```

---

_CS50 AI (Harvard) content extracted from Keynote lecture slides (lecture0.key – lecture6.key)._  
_Enhanced with Python implementations, deep explanations, and practice tasks._  
_Cross-referenced with university AI course notes._