# 🧠 AI Course — Complete Enhanced Learning Resource

> **CS Student Edition** | Based on university lecture notes (Lectures 1–6)  
> Enhanced with deep explanations, Python code, practice tasks & real-world applications.

---

> **How to read this document:**
> 
> - 📋 **"[YOUR NOTES]"** sections = your original lecture content, kept intact
> - 🔵 **"[ENHANCED]"** sections = added explanations, code, and insights
> - Each lecture is self-contained but cross-references others

---

# 📚 Table of Contents

1. [[#Lecture 1 — AI Overview & Foundations]]
2. [[#Lecture 2 — Knowledge Engineering & Rule-Based Systems]]
3. [[#Lecture 3 — Search Algorithms]]
4. [[#Lectures 4 & 5 — Machine Learning Linear Regression]]
5. [[#Lecture 6 — Machine Learning Logistic Regression]]

---

# Lecture 1 — AI Overview & Foundations

---

## 📋 [YOUR NOTES] — What is AI?

> **Alan Turing (1950):** "I propose to consider the question, 'Can machines think?'"  
> **John McCarthy (1956):** "The branch of computer science concerned with making computers behave like humans."  
> **Marvin Minsky:** "The science of making machines do things that would require intelligence if done by men."

**AI** is a technical science that studies and develops theories, methods, techniques, and application systems for simulating and extending human intelligence.

**Three Major Schools of Thought:**

- **Symbolism** — Intelligence = symbol manipulation, knowledge representation & inference (→ Expert Systems)
- **Connectionism** — Intelligence = neural networks, inspired by the brain (→ Deep Learning)
- **Behaviorism** — Intelligence = perception + action, evolving through environment interaction (→ Genetic Algorithms, Reinforcement Learning)

**AI ⊃ Machine Learning ⊃ Deep Learning** (nested relationship)

---

## 🔵 [ENHANCED] — The AI Landscape Explained

### Why Does the Definition Matter?

The three definitions above represent three _different philosophies_ about what intelligence is:

|Philosopher|Core Idea|Modern Equivalent|
|---|---|---|
|Turing|Can it _behave_ intelligently?|Chatbots, LLMs|
|McCarthy|Engineering intelligent systems|Compilers, planners|
|Minsky|Simulating human cognition|Cognitive architectures|

This isn't just history — it determines _which tools you pick_ for a problem.

### The Three Schools, Mapped to Python

```python
# ============================================================
# SYMBOLISM: Rule-based reasoning
# ============================================================
def is_bird_can_fly(animal: str, can_fly: bool) -> str:
    """Symbolic AI: explicit rules encode knowledge."""
    if animal == "bird" and can_fly:
        return "This bird can fly."
    elif animal == "bird" and not can_fly:
        return "This bird cannot fly (e.g., penguin)."
    return "Not a bird."

# ============================================================
# CONNECTIONISM: Neural network (conceptual)
# ============================================================
import math

def sigmoid(x: float) -> float:
    """A single neuron's activation — building block of deep learning."""
    return 1 / (1 + math.exp(-x))

# ============================================================
# BEHAVIORISM: Agent acting in environment
# ============================================================
class SimpleAgent:
    """Behaviorist AI: perceive → act → learn from reward."""
    def __init__(self):
        self.score = 0

    def perceive(self, environment_state: str) -> str:
        """Map perception to action."""
        if environment_state == "obstacle":
            return "turn_left"
        return "move_forward"

    def act(self, action: str) -> int:
        """Receive reward from environment."""
        reward = 1 if action == "move_forward" else -1
        self.score += reward
        return reward

agent = SimpleAgent()
state = "clear"
action = agent.perceive(state)
reward = agent.act(action)
print(f"Action: {action}, Reward: {reward}, Score: {agent.score}")
```

### The AI Hierarchy — Visualized

```
AI (Broad field)
│
├── Symbolic AI        ← Rules, Logic, Expert Systems
│
├── Machine Learning   ← Learning from data
│   │
│   ├── Supervised     ← Labeled data (regression, classification)
│   ├── Unsupervised   ← No labels (clustering, PCA)
│   ├── Semi-supervised← Mix of both
│   └── Reinforcement  ← Reward-based learning
│       │
│       └── Deep Learning ← Neural Networks with many layers
│
└── Behaviorism        ← Evolutionary algorithms, genetic algorithms
```

---

## 🔵 [ENHANCED] — Real-World Applications Mapped to Techniques

|Application|AI School|Python Library|
|---|---|---|
|Medical diagnosis|Symbolism (Expert System)|`experta`, `CLIPS`|
|Face recognition|Connectionism (CNN)|`tensorflow`, `pytorch`|
|Game-playing (Chess)|Symbolism + Search|Custom + `python-chess`|
|Self-driving cars|Connectionism + RL|`pytorch`, `gym`|
|Spam detection|ML (Supervised)|`scikit-learn`|
|Customer segmentation|ML (Unsupervised)|`scikit-learn`|

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> List 3 real-world AI applications and classify each as Symbolism, Connectionism, or Behaviorism. Justify your answer.
```txt
Medical diagnosis -> Symbolism --> debends on (if-then rules)
Face recognition -> Connectionism --> debends on (Artificial Neural Networks)
Sweeper robot -> Behaviorism -> debends on (Action-Reaction)
```
### Medium

```python
# Task: Implement a simple AI classifier that:
# 1. Takes a problem description as input
# 2. Recommends which AI approach to use (symbolic/ML/RL)
# Hint: Use keyword matching as a first step

def recommend_ai_approach(problem: str) -> str:
	"""
	YOUR TASK: Implement this function.
	Examples:
	- "I have 10,000 labeled images of cats and dogs" → "Supervised ML"
	- "I need to define rules for loan approval" → "Expert System (Symbolic)"
	- "I want a robot to navigate a maze" → "Reinforcement Learning"
	"""
	description = problem.lower()
	rules_keywords = ["rules", "expert", "logic", "defined", "laws", "symbolic"]
	ml_keywords = ["labeled", "images", "dataset", "prediction", "historical", "data"]
	rl_keywords = ["robot", "maze", "navigate", "agent", "reward", "game", "environment"]
	if any(word in description for word in rl_keywords):
		return "Reinforcement Learning"
	elif any(word in description for word in ml_keywords):
		return "Machine Learning (Supervised/Unsupervised)"
	elif any(word in description for word in rules_keywords):
		return "Expert System (Symbolic AI)"
	else:
		return "Hybrid or Undetermined Approach"
```

### Hard

> Research and write a 1-page comparison between GPT (Connectionist) and a classical expert system. What are the tradeoffs in interpretability, data requirements, and failure modes?
```
كنكل
```
---

# Lecture 2 — Knowledge Engineering & Rule-Based Systems

---

## 📋 [YOUR NOTES] — Knowledge Representation Techniques

**Four main techniques:**

1. **Logical Representation** — Uses formal logic, no ambiguity. Example: `∀x bird(x) → fly(x)`
2. **Semantic Networks** — Graph-based; vertices = concepts, edges = relations (is-a, can, part-of)
3. **Frame Representation** — Object-oriented style with inheritance. `Bird` inherits from `Animal` and adds `can-fly = true`
4. **Production Rules** — IF-THEN structure. The most used in expert systems.

**Production Rule Syntax:**

```
IF  <antecedent>
THEN <consequent>
```

**Example rules:**

```
IF 'traffic light' is green → THEN action is go
IF 'age' < 18 AND 'withdrawal' > 1000 → THEN 'signature of parent' is required
```

---

## 📋 [YOUR NOTES] — Expert Systems

- **Expert System** = computer program emulating a human expert's decision-making
- **Team**: domain expert, knowledge engineer, programmer, project manager, end-user
- **Structure**: Knowledge Base + Database + Inference Engine + Explanation Facility + User Interface
- **Inference Engine**: compares rules to facts, fires matching rules

### Inference Chaining

**Forward Chaining** (data-driven):

- Start from known facts → derive new facts → reach goal
- "Gather info, then infer"
- Uses a **stack (LIFO)** internally

**Backward Chaining** (goal-driven):

- Start from goal → work backward to find supporting facts
- "Hypothesize, then verify"

### Conflict Resolution Methods

1. **Priority** — fire rule with highest priority
2. **Specificity** — fire most specific (longest matching) rule
3. **Recency** — fire rule using most recently added data

### Certainty Factors

```
cf(H, e) = cf(E, e) × cf(H, E)

Conjunctive (AND): cf = min(cf1, cf2, ...) × cf_rule
Disjunctive (OR):  cf = max(cf1, cf2, ...) × cf_rule

Combined (two rules → same conclusion):
  If cf1 ≥ 0 and cf2 ≥ 0: cf = cf1 + cf2(1 - cf1)
  If cf1 < 0 and cf2 < 0: cf = cf1 + cf2(1 + cf1)
  If opposite signs:       cf = (cf1 + cf2) / (1 - min(|cf1|, |cf2|))
```

---

## 🔵 [ENHANCED] — Building an Expert System in Python

### Concept: Knowledge Base as a Dictionary

```python
from typing import Dict, List, Tuple, Optional

# ============================================================
# KNOWLEDGE BASE: List of (conditions, conclusion, certainty_factor)
# ============================================================
KnowledgeBase = [
    # Rule 1: sunny weather → wear sunglasses
    ({"sky": "clear", "temperature": "hot"}, "wear sunglasses", 0.9),
    
    # Rule 2: rainy weather → take umbrella
    ({"sky": "overcast", "forecast": "rain"}, "take umbrella", 0.85),
    
    # Rule 3: cold day → wear coat
    ({"temperature": "cold"}, "wear coat", 0.95),
    
    # Rule 4: clear + sunny → sunscreen
    ({"sky": "clear", "season": "summer"}, "apply sunscreen", 0.8),
]

def match_rule(conditions: Dict, facts: Dict) -> bool:
    """Check if all conditions of a rule match the current facts."""
    return all(facts.get(key) == value for key, value in conditions.items())

def forward_chain(facts: Dict) -> List[Tuple[str, float]]:
    """
    Forward chaining inference engine.
    
    Start from known facts, fire all matching rules,
    add conclusions to facts, repeat until no new rules fire.
    
    Returns: list of (conclusion, certainty_factor)
    """
    conclusions = []
    fired_rules = set()
    changed = True
    
    while changed:
        changed = False
        for i, (conditions, conclusion, cf) in enumerate(KnowledgeBase):
            if i not in fired_rules and match_rule(conditions, facts):
                conclusions.append((conclusion, cf))
                fired_rules.add(i)
                # Add conclusion as new fact (simplified)
                facts[conclusion] = True
                changed = True
                print(f"  ✓ Rule {i+1} fired → '{conclusion}' (CF={cf})")
    
    return conclusions


# ============================================================
# DEMO: Forward Chaining
# ============================================================
print("=== Forward Chaining Expert System ===\n")

current_facts = {
    "sky": "clear",
    "temperature": "hot",
    "season": "summer",
}

print(f"Initial facts: {current_facts}\n")
print("Firing rules:")
results = forward_chain(current_facts)

print(f"\n📋 Conclusions reached:")
for conclusion, cf in results:
    print(f"  → {conclusion} (Certainty: {cf:.0%})")
```

### Backward Chaining

```python
def backward_chain(goal: str, facts: Dict, kb: List) -> Optional[float]:
    """
    Backward chaining: given a goal, try to prove it.
    
    Returns certainty factor if proved, None otherwise.
    """
    # Check if goal is directly in facts
    if goal in facts:
        return 1.0
    
    print(f"  🔍 Trying to prove: '{goal}'")
    
    for conditions, conclusion, cf in kb:
        if conclusion == goal:
            # Try to prove all conditions of this rule
            min_cf = 1.0
            all_proved = True
            
            for cond_key, cond_val in conditions.items():
                if facts.get(cond_key) == cond_val:
                    condition_cf = 1.0
                else:
                    # Try to prove this condition recursively
                    condition_cf = backward_chain(cond_key, facts, kb)
                    if condition_cf is None:
                        all_proved = False
                        break
                min_cf = min(min_cf, condition_cf)
            
            if all_proved:
                result_cf = min_cf * cf
                print(f"  ✓ Proved '{goal}' with CF={result_cf:.2f}")
                return result_cf
    
    print(f"  ✗ Cannot prove '{goal}'")
    return None


# ============================================================
# DEMO: Certainty Factor Combination
# ============================================================
def combine_certainty_factors(cf1: float, cf2: float) -> float:
    """Combine two certainty factors reaching the same conclusion."""
    if cf1 >= 0 and cf2 >= 0:
        return cf1 + cf2 * (1 - cf1)
    elif cf1 < 0 and cf2 < 0:
        return cf1 + cf2 * (1 + cf1)
    else:
        return (cf1 + cf2) / (1 - min(abs(cf1), abs(cf2)))

# Example: two rules both suggest "take umbrella"
cf_rule1 = 0.8   # from "sky is overcast"
cf_rule2 = 0.6   # from "forecast is rain"
combined = combine_certainty_factors(cf_rule1, cf_rule2)
print(f"\nCombined CF for 'take umbrella': {combined:.3f}")
# Result: 0.8 + 0.6*(1-0.8) = 0.8 + 0.12 = 0.92
```

---

## 🔵 [ENHANCED] — Semantic Networks in Python

```python
# Semantic Network as a Graph (using adjacency dict)
class SemanticNetwork:
    def __init__(self):
        self.nodes = {}   # node → properties
        self.edges = []   # (from, relation, to)
    
    def add_concept(self, name: str, **properties):
        self.nodes[name] = properties
    
    def add_relation(self, source: str, relation: str, target: str):
        self.edges.append((source, relation, target))
    
    def query(self, node: str, relation: str) -> list:
        """Find all targets connected to node via relation."""
        return [t for s, r, t in self.edges if s == node and r == relation]
    
    def inherit(self, node: str) -> dict:
        """Collect all properties via 'is_a' chain (inheritance)."""
        props = dict(self.nodes.get(node, {}))
        for parent in self.query(node, "is_a"):
            parent_props = self.inherit(parent)
            parent_props.update(props)  # child overrides parent
            props = parent_props
        return props

# Build a semantic network
sn = SemanticNetwork()
sn.add_concept("Animal", has_life=True, can_move=True)
sn.add_concept("Bird", has_wings=True, can_fly=True)
sn.add_concept("Penguin", can_fly=False, lives_in="Antarctica")
sn.add_concept("Eagle", size="large")

sn.add_relation("Bird", "is_a", "Animal")
sn.add_relation("Penguin", "is_a", "Bird")
sn.add_relation("Eagle", "is_a", "Bird")
sn.add_relation("Bird", "can", "lay_eggs")

# Query
print("Eagle's full properties (with inheritance):")
print(sn.inherit("Eagle"))
# Output: {'has_life': True, 'can_move': True, 'has_wings': True, 'can_fly': True, 'size': 'large'}

print("\nPenguin's full properties (can_fly overridden to False):")
print(sn.inherit("Penguin"))
# Output: {'has_life': True, 'can_move': True, 'has_wings': True, 'can_fly': False, 'lives_in': 'Antarctica'}
```

---

## 🔵 [ENHANCED] — References to Review

- **Prerequisite**: Boolean logic (AND, OR, NOT), basic Python dictionaries and lists
- **Related Topics**: Prolog (logic programming language), CLIPS (expert system shell)
- **Connected Concepts**:
    - Bayesian Networks (probabilistic extension of expert systems)
    - Decision Trees (ML version of rule-based systems)
    - Forward/Backward chaining ↔ BFS/DFS in search (Lecture 3)

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> Write 5 production rules for a simple medical diagnosis system (e.g., fever + cough → flu).

### Medium

```python
# Implement a simple expert system for recommending a programming language:
# Rules:
# - IF task = "web" AND priority = "speed" → JavaScript
# - IF task = "data" AND priority = "simplicity" → Python
# - IF task = "system" AND priority = "performance" → C++
# - IF task = "mobile" → Swift OR Kotlin (based on platform)
# 
# Your task: implement with certainty factors and forward chaining

def language_recommender(task: str, priority: str, platform: str = None):
    """Recommend a programming language based on requirements."""
    # TODO: implement
    pass
```

### Hard

> Implement a **backward chaining** engine that can handle chains of 3+ rules, and visualize the proof tree using a dictionary structure. Test it with a medical diagnosis knowledge base with at least 10 rules.

---

# Lecture 3 — Search Algorithms

---

## 📋 [YOUR NOTES] — Search Problem Components

- **Agent**: entity that perceives its environment and acts upon it
- **State**: configuration of agent and environment
- **Initial State**: where the agent begins
- **Actions**: choices available in a state
- **Transition Model**: result of performing an action in a state
- **State Space**: all states reachable from initial state
- **Goal Test**: condition that determines if state is a goal
- **Path Cost**: cost of the path taken
- **Solution**: sequence of actions from initial state to goal
- **Optimal Solution**: solution with lowest path cost

**Frontier**: the mechanism that manages exploration (implemented as stack or queue)

---

## 📋 [YOUR NOTES] — Search Algorithms

### Uninformed Search

**DFS (Depth-First Search)**

- Uses a **Stack (LIFO)**
- Explores as deep as possible first
- ✅ Best case: fastest if lucky
- ❌ May not find optimal solution; worst case explores everything

**BFS (Breadth-First Search)**

- Uses a **Queue (FIFO)**
- Explores level by level
- ✅ Guaranteed to find optimal solution (minimum steps)
- ❌ Uses more memory; slow for deep solutions

### Informed Search

**Greedy Best-First Search**

- Expands node closest to goal using heuristic `h(n)`
- `h(n)` = estimated distance to goal (e.g., Manhattan distance)
- Fast but not guaranteed optimal

**A* Search**

- Expands node with lowest `f(n) = g(n) + h(n)`
- `g(n)` = cost to reach node, `h(n)` = estimated cost to goal
- ✅ Optimal if `h(n)` is **admissible** (never overestimates) and **consistent**

### Adversarial Search

**Minimax Algorithm**

- Two players: MAX (wants +1) and MIN (wants -1)
- Recursively simulates all possible moves
- Time complexity: `O(b^m)` where `b` = branching factor, `m` = max depth
- Space complexity: `O(bm)` (DFS-based)

**Alpha-Beta Pruning**

- Optimization of Minimax — prunes branches that can't affect the result
- `alpha` = best value MAX can guarantee (starts at -∞)
- `beta` = best value MIN can guarantee (starts at +∞)
- Prune when `alpha ≥ beta`

---

## 🔵 [ENHANCED] — All Search Algorithms in Python

```python
from collections import deque
import heapq
from typing import Dict, List, Tuple, Set, Callable, Optional

# ============================================================
# GRAPH REPRESENTATION
# Example graph for all algorithms
# ============================================================
#
#     A --1-- B --2-- E
#     |       |       |
#     3       4       1
#     |       |       |
#     C --2-- D --3-- F
#
Graph = Dict[str, List[Tuple[str, int]]]  # node → [(neighbor, cost)]

example_graph: Graph = {
    'A': [('B', 1), ('C', 3)],
    'B': [('A', 1), ('D', 4), ('E', 2)],
    'C': [('A', 3), ('D', 2)],
    'D': [('B', 4), ('C', 2), ('F', 3)],
    'E': [('B', 2), ('F', 1)],
    'F': [('D', 3), ('E', 1)],
}

# ============================================================
# HELPER: Reconstruct path from parent dictionary
# ============================================================
def reconstruct_path(came_from: dict, start: str, goal: str) -> List[str]:
    path = []
    current = goal
    while current is not None:
        path.append(current)
        current = came_from.get(current)
    return list(reversed(path))


# ============================================================
# 1. BFS — Breadth-First Search
# ============================================================
def bfs(graph: Graph, start: str, goal: str) -> Optional[List[str]]:
    """
    BFS finds the shortest path in terms of NUMBER OF STEPS.
    Uses a QUEUE (FIFO) → explores level by level.
    
    Time: O(V + E), Space: O(V)
    """
    queue = deque([start])           # frontier: queue
    came_from = {start: None}        # tracks path
    
    while queue:
        node = queue.popleft()       # FIFO: take from front
        
        if node == goal:
            return reconstruct_path(came_from, start, goal)
        
        for neighbor, _ in graph[node]:
            if neighbor not in came_from:
                came_from[neighbor] = node
                queue.append(neighbor)
    
    return None  # no path found


# ============================================================
# 2. DFS — Depth-First Search
# ============================================================
def dfs(graph: Graph, start: str, goal: str) -> Optional[List[str]]:
    """
    DFS explores as deep as possible before backtracking.
    Uses a STACK (LIFO).
    
    Time: O(V + E), Space: O(V)
    Does NOT guarantee shortest path.
    """
    stack = [start]                  # frontier: stack
    came_from = {start: None}
    
    while stack:
        node = stack.pop()           # LIFO: take from top
        
        if node == goal:
            return reconstruct_path(came_from, start, goal)
        
        for neighbor, _ in graph[node]:
            if neighbor not in came_from:
                came_from[neighbor] = node
                stack.append(neighbor)
    
    return None


# ============================================================
# 3. Greedy Best-First Search
# ============================================================
def greedy_bfs(graph: Graph, start: str, goal: str,
               heuristic: Callable[[str], float]) -> Optional[List[str]]:
    """
    Greedy: always expand the node that LOOKS closest to goal.
    Uses a PRIORITY QUEUE ordered by h(n).
    
    Fast but NOT guaranteed optimal.
    """
    # Priority queue: (h(n), node)
    heap = [(heuristic(start), start)]
    came_from = {start: None}
    
    while heap:
        _, node = heapq.heappop(heap)
        
        if node == goal:
            return reconstruct_path(came_from, start, goal)
        
        for neighbor, _ in graph[node]:
            if neighbor not in came_from:
                came_from[neighbor] = node
                heapq.heappush(heap, (heuristic(neighbor), neighbor))
    
    return None


# ============================================================
# 4. A* Search
# ============================================================
def a_star(graph: Graph, start: str, goal: str,
           heuristic: Callable[[str], float]) -> Optional[Tuple[List[str], float]]:
    """
    A*: expands node with lowest f(n) = g(n) + h(n).
    
    g(n) = actual cost from start to n
    h(n) = estimated cost from n to goal (must be admissible)
    
    OPTIMAL if h is admissible and consistent.
    Time/Space: O(b^d) where d = depth of optimal solution.
    """
    # Priority queue: (f(n), node)
    heap = [(heuristic(start), start)]
    came_from = {start: None}
    g_cost = {start: 0}
    
    while heap:
        f, node = heapq.heappop(heap)
        
        if node == goal:
            path = reconstruct_path(came_from, start, goal)
            return path, g_cost[goal]
        
        for neighbor, edge_cost in graph[node]:
            new_g = g_cost[node] + edge_cost
            
            if neighbor not in g_cost or new_g < g_cost[neighbor]:
                g_cost[neighbor] = new_g
                came_from[neighbor] = node
                f_cost = new_g + heuristic(neighbor)
                heapq.heappush(heap, (f_cost, neighbor))
    
    return None


# ============================================================
# DEMO: Run all algorithms
# ============================================================

# Simple heuristic: alphabetical distance to 'F'
def heuristic(node: str) -> float:
    distances = {'A': 5, 'B': 3, 'C': 4, 'D': 2, 'E': 1, 'F': 0}
    return distances.get(node, 0)

start, goal = 'A', 'F'
print(f"Finding path from {start} to {goal}:\n")

bfs_path = bfs(example_graph, start, goal)
print(f"BFS path:    {' → '.join(bfs_path)}")

dfs_path = dfs(example_graph, start, goal)
print(f"DFS path:    {' → '.join(dfs_path)}")

gbfs_path = greedy_bfs(example_graph, start, goal, heuristic)
print(f"Greedy path: {' → '.join(gbfs_path)}")

astar_result = a_star(example_graph, start, goal, heuristic)
if astar_result:
    astar_path, cost = astar_result
    print(f"A* path:     {' → '.join(astar_path)} (cost={cost})")
```

### Minimax with Alpha-Beta Pruning

```python
import math

# ============================================================
# MINIMAX ALGORITHM
# ============================================================
def minimax(node, depth: int, is_maximizing: bool,
            get_children, evaluate, get_terminal_value) -> float:
    """
    Minimax algorithm for two-player zero-sum games.
    
    Args:
        node: current game state
        depth: remaining depth to explore
        is_maximizing: True if current player is MAX
        get_children: function(node) → list of child states
        evaluate: function(node) → heuristic value (for non-terminal)
        get_terminal_value: function(node) → value if terminal, else None
    
    Returns: best score achievable from this node
    """
    terminal_val = get_terminal_value(node)
    if terminal_val is not None or depth == 0:
        return terminal_val if terminal_val is not None else evaluate(node)
    
    children = get_children(node)
    
    if is_maximizing:
        best = -math.inf
        for child in children:
            score = minimax(child, depth - 1, False, 
                          get_children, evaluate, get_terminal_value)
            best = max(best, score)
        return best
    else:
        best = math.inf
        for child in children:
            score = minimax(child, depth - 1, True,
                          get_children, evaluate, get_terminal_value)
            best = min(best, score)
        return best


# ============================================================
# ALPHA-BETA PRUNING
# ============================================================
def alpha_beta(node, depth: int, alpha: float, beta: float,
               is_maximizing: bool, get_children, evaluate, 
               get_terminal_value) -> float:
    """
    Alpha-Beta Pruning: optimized Minimax.
    
    alpha = best score MAX can guarantee (initialized to -∞)
    beta  = best score MIN can guarantee (initialized to +∞)
    
    Prune branch when alpha ≥ beta:
    - MAX won't visit this if MIN already has a better option
    - MIN won't visit this if MAX already has a better option
    
    Best case: O(b^(m/2)) — same quality as Minimax with half the nodes
    """
    terminal_val = get_terminal_value(node)
    if terminal_val is not None or depth == 0:
        return terminal_val if terminal_val is not None else evaluate(node)
    
    children = get_children(node)
    
    if is_maximizing:
        value = -math.inf
        for child in children:
            score = alpha_beta(child, depth - 1, alpha, beta, False,
                             get_children, evaluate, get_terminal_value)
            value = max(value, score)
            alpha = max(alpha, value)
            if alpha >= beta:
                break  # ✂️ Beta cutoff: MIN already has better option
        return value
    else:
        value = math.inf
        for child in children:
            score = alpha_beta(child, depth - 1, alpha, beta, True,
                             get_children, evaluate, get_terminal_value)
            value = min(value, score)
            beta = min(beta, value)
            if alpha >= beta:
                break  # ✂️ Alpha cutoff: MAX already has better option
        return value


# ============================================================
# DEMO: Tic-Tac-Toe with Minimax
# ============================================================
class TicTacToe:
    """Simple Tic-Tac-Toe to demonstrate Minimax."""
    
    def __init__(self, board=None):
        self.board = board or [' '] * 9  # 3x3 grid, indices 0-8
    
    def get_children(self) -> List['TicTacToe']:
        """Generate all possible next states."""
        symbol = 'X' if self.board.count('X') == self.board.count('O') else 'O'
        children = []
        for i, cell in enumerate(self.board):
            if cell == ' ':
                new_board = self.board[:]
                new_board[i] = symbol
                children.append(TicTacToe(new_board))
        return children
    
    def get_winner(self) -> Optional[str]:
        """Return winner ('X', 'O') or None."""
        lines = [(0,1,2),(3,4,5),(6,7,8),  # rows
                 (0,3,6),(1,4,7),(2,5,8),  # columns
                 (0,4,8),(2,4,6)]           # diagonals
        for a, b, c in lines:
            if self.board[a] == self.board[b] == self.board[c] != ' ':
                return self.board[a]
        return None
    
    def terminal_value(self) -> Optional[float]:
        winner = self.get_winner()
        if winner == 'X': return 1.0
        if winner == 'O': return -1.0
        if ' ' not in self.board: return 0.0  # draw
        return None  # not terminal
    
    def display(self):
        b = self.board
        print(f" {b[0]} | {b[1]} | {b[2]}")
        print("---+---+---")
        print(f" {b[3]} | {b[4]} | {b[5]}")
        print("---+---+---")
        print(f" {b[6]} | {b[7]} | {b[8]}")

# Demo: find best move for X
game = TicTacToe()
game.board = ['X', 'O', 'X',
              ' ', 'O', ' ',
              ' ', ' ', ' ']
print("\nCurrent board:")
game.display()

best_score = -math.inf
best_child = None
for child in game.get_children():
    score = alpha_beta(
        child, depth=5, alpha=-math.inf, beta=math.inf,
        is_maximizing=False,  # After X's move, it's O's turn (minimizing)
        get_children=lambda n: n.get_children(),
        evaluate=lambda n: 0,
        get_terminal_value=lambda n: n.terminal_value()
    )
    if score > best_score:
        best_score = score
        best_child = child

print(f"\nBest move for X (score={best_score}):")
best_child.display()
```

---

## 🔵 [ENHANCED] — Algorithm Comparison Table

|Algorithm|Data Structure|Optimal?|Complete?|Time|Space|Notes|
|---|---|---|---|---|---|---|
|BFS|Queue (FIFO)|✅ (uniform cost)|✅|O(b^d)|O(b^d)|Best for fewest steps|
|DFS|Stack (LIFO)|❌|✅ (finite)|O(b^m)|O(bm)|Memory efficient|
|Greedy|Priority Queue (h)|❌|❌|O(b^m)|O(b^m)|Fast, not reliable|
|A*|Priority Queue (f=g+h)|✅|✅|O(b^d)|O(b^d)|Best informed search|
|Minimax|Stack (DFS)|✅|✅|O(b^m)|O(bm)|Two-player games|
|Alpha-Beta|Stack (DFS)|✅|✅|O(b^m/2)|O(bm)|Minimax + pruning|

---

## 🔵 [ENHANCED] — Real-World Applications

|Algorithm|Real Application|
|---|---|
|BFS|GPS shortest-route, social network degrees of separation|
|DFS|Maze solving, topological sort, cycle detection|
|A*|Google Maps, game pathfinding (Unity NavMesh)|
|Greedy|Huffman encoding, Prim's MST|
|Minimax|Chess engines (Stockfish base), checkers|
|Alpha-Beta|All modern chess/go engines|

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> Trace BFS and DFS manually on this graph. Write the order nodes are visited. Graph: A→B, A→C, B→D, B→E, C→F. Start=A, Goal=F

### Medium

```python
# Implement Dijkstra's Algorithm (weighted BFS / uniform-cost search)
# It's A* with h(n) = 0 for all nodes.

def dijkstra(graph: Graph, start: str, goal: str) -> Tuple[List[str], float]:
    """
    Find shortest WEIGHTED path.
    This is A* with h(n)=0, also known as Uniform Cost Search.
    
    YOUR TASK: Implement this.
    Hint: Use heapq with (cost, node) tuples.
    """
    # TODO: implement
    pass

# Test: path from 'A' to 'F' in example_graph above
# Expected: A → B → E → F, cost = 1+2+1 = 4
```

### Hard

> Implement a complete **N-Queens solver** using:
> 
> 1. DFS (backtracking)
> 2. BFS (state space search) Compare their performance for N=8. Which is faster? Why?

---

# Lectures 4 & 5 — Machine Learning: Linear Regression

---

## 📋 [YOUR NOTES] — Machine Learning Basics

**ML Definition**: A core AI research field. Computers obtain new knowledge by simulating human learning behavior, reorganizing existing knowledge to improve performance.

**Key distinction from rule-based systems**:

- Rule-based: explicit programming, rules manually specified
- Machine learning: samples for training, rules automatically learned

**Three learning types:**

- **Supervised**: labeled data → learn mapping from features to labels
- **Unsupervised**: no labels → find patterns (clustering)
- **Semi-supervised**: mix of labeled + unlabeled
- **Reinforcement**: agent learns by reward/punishment from environment

**Key concepts:**

- **Dataset**: collection of data; each record = **sample**, each column = **feature**
- **Training set**: used to learn the model
- **Test set**: used to evaluate generalization
- **Epoch**: one complete pass through the training data

---

## 📋 [YOUR NOTES] — Linear Regression

**Definition**: Statistical method to determine quantitative relationships between variables. A type of **supervised learning**.

**Loss Function (MSE)**: $$E(w) = \frac{1}{2m}\sum_{i=1}^{m}(y_t - y_p)^2 = \frac{1}{2m}\sum_{i=1}^{m}(y_t - ax - b)^2$$

**Gradient Descent Update Rules**: $$a_{i+1} = a_i + \eta \cdot \frac{1}{m}\sum(y_t - y_p) \cdot x$$ $$b_{i+1} = b_i + \eta \cdot \frac{1}{m}\sum(y_t - y_p)$$

**Three Gradient Descent Variants:**

- **BGD** (Batch): all samples → most stable, slowest
- **SGD** (Stochastic): one sample → fastest per update, noisy
- **MBGD** (Mini-Batch): n samples → best balance

---

## 📋 [YOUR NOTES] — Model Validity & Overfitting

**Error decomposition**: $$\text{Total Error} = \text{Bias}^2 + \text{Variance} + \text{Irreducible Error}$$

- **Underfitting**: high bias, low variance → model too simple
- **Good fit**: low bias, low variance → ideal
- **Overfitting**: low bias, high variance → model too complex

**Preventing Overfitting:**

- L1 Regularization (Lasso): `J(w) = J(w) + α||w||₁` → sparse model, feature selection
- L2 Regularization (Ridge): `J(w) = J(w) + ½α||w||₂²` → penalizes large weights
- Dropout, Early Stopping, Dataset Augmentation

**Hyperparameter Search:**

- Grid Search: exhaustive, expensive, good for small spaces
- Random Search: better for large spaces

---

## 🔵 [ENHANCED] — Linear Regression: Full Python Implementation

```python
import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple, List

# ============================================================
# LINEAR REGRESSION FROM SCRATCH
# ============================================================

class LinearRegression:
    """
    Linear Regression: y = ax + b
    Trained via gradient descent.
    
    This class supports all three gradient descent variants:
    - Batch (BGD): all samples per update
    - Stochastic (SGD): one sample per update  
    - Mini-Batch (MBGD): n samples per update
    """
    
    def __init__(self, learning_rate: float = 0.0001, epochs: int = 1000):
        self.lr = learning_rate
        self.epochs = epochs
        self.a = 1.0   # slope (weight)
        self.b = 1.0   # intercept (bias)
        self.loss_history = []
    
    def predict(self, x: np.ndarray) -> np.ndarray:
        """Forward pass: yp = ax + b"""
        return self.a * x + self.b
    
    def mse_loss(self, y_true: np.ndarray, y_pred: np.ndarray) -> float:
        """Mean Squared Error loss."""
        return np.mean((y_true - y_pred) ** 2) / 2
    
    def fit_bgd(self, x: np.ndarray, y: np.ndarray) -> 'LinearRegression':
        """Batch Gradient Descent: use ALL samples each update."""
        m = len(x)
        for epoch in range(self.epochs):
            y_pred = self.predict(x)
            
            # Gradients
            error = y - y_pred
            grad_a = -np.mean(error * x)
            grad_b = -np.mean(error)
            
            # Update
            self.a -= self.lr * grad_a
            self.b -= self.lr * grad_b
            
            loss = self.mse_loss(y, self.predict(x))
            self.loss_history.append(loss)
        
        return self
    
    def fit_sgd(self, x: np.ndarray, y: np.ndarray) -> 'LinearRegression':
        """Stochastic Gradient Descent: use ONE sample per update."""
        m = len(x)
        for epoch in range(self.epochs):
            indices = np.random.permutation(m)  # shuffle
            epoch_loss = 0
            
            for i in indices:
                y_pred = self.predict(x[i])
                error = y[i] - y_pred
                
                self.a += self.lr * error * x[i]
                self.b += self.lr * error
                epoch_loss += (error ** 2) / 2
            
            self.loss_history.append(epoch_loss / m)
        
        return self
    
    def fit_mbgd(self, x: np.ndarray, y: np.ndarray,
                 batch_size: int = 5) -> 'LinearRegression':
        """Mini-Batch Gradient Descent."""
        m = len(x)
        for epoch in range(self.epochs):
            indices = np.random.permutation(m)
            x_shuffled, y_shuffled = x[indices], y[indices]
            epoch_loss = 0
            
            for i in range(0, m, batch_size):
                x_batch = x_shuffled[i:i + batch_size]
                y_batch = y_shuffled[i:i + batch_size]
                
                y_pred = self.predict(x_batch)
                error = y_batch - y_pred
                
                self.a += self.lr * np.mean(error * x_batch)
                self.b += self.lr * np.mean(error)
                epoch_loss += np.sum(error ** 2) / 2
            
            self.loss_history.append(epoch_loss / m)
        
        return self
    
    def r_squared(self, x: np.ndarray, y: np.ndarray) -> float:
        """R² score: how much variance is explained (1 = perfect)."""
        y_pred = self.predict(x)
        ss_res = np.sum((y - y_pred) ** 2)
        ss_tot = np.sum((y - np.mean(y)) ** 2)
        return 1 - (ss_res / ss_tot)


# ============================================================
# DEMO: Your lecture's dataset
# ============================================================
x = np.array([3, 21, 22, 34, 54, 34, 55, 67, 89, 99], dtype=float)
y = np.array([1, 10, 14, 34, 44, 36, 22, 67, 79, 90], dtype=float)

# BGD
model_bgd = LinearRegression(learning_rate=0.0001, epochs=10000)
model_bgd.fit_bgd(x, y)
print(f"BGD Results:")
print(f"  a = {model_bgd.a:.4f}, b = {model_bgd.b:.4f}")
print(f"  R² = {model_bgd.r_squared(x, y):.4f}")
print(f"  Final loss = {model_bgd.loss_history[-1]:.4f}")

# Predict for x=50
print(f"  Prediction for x=50: {model_bgd.predict(50):.2f}")
```

### Ridge vs Lasso Regression

```python
# ============================================================
# REGULARIZED LINEAR REGRESSION
# ============================================================

class RidgeRegression(LinearRegression):
    """L2 Regularization: penalizes sum of squared weights."""
    
    def __init__(self, learning_rate=0.0001, epochs=1000, lambda_=0.1):
        super().__init__(learning_rate, epochs)
        self.lambda_ = lambda_  # regularization strength
    
    def fit_bgd(self, x, y):
        m = len(x)
        for epoch in range(self.epochs):
            y_pred = self.predict(x)
            error = y - y_pred
            
            # L2 regularization adds λ*w to gradient
            grad_a = -np.mean(error * x) + self.lambda_ * self.a
            grad_b = -np.mean(error)  # bias is not regularized
            
            self.a -= self.lr * grad_a
            self.b -= self.lr * grad_b
            
            # L2 loss = MSE + λ/2 * ||w||²
            loss = self.mse_loss(y, self.predict(x)) + (self.lambda_ / 2) * self.a**2
            self.loss_history.append(loss)
        return self


class LassoRegression(LinearRegression):
    """L1 Regularization: penalizes sum of absolute weights → sparse solutions."""
    
    def __init__(self, learning_rate=0.0001, epochs=1000, lambda_=0.1):
        super().__init__(learning_rate, epochs)
        self.lambda_ = lambda_
    
    def fit_bgd(self, x, y):
        m = len(x)
        for epoch in range(self.epochs):
            y_pred = self.predict(x)
            error = y - y_pred
            
            # L1 gradient: sign(w) instead of w
            grad_a = -np.mean(error * x) + self.lambda_ * np.sign(self.a)
            grad_b = -np.mean(error)
            
            self.a -= self.lr * grad_a
            self.b -= self.lr * grad_b
            
            # L1 loss = MSE + λ * ||w||₁
            loss = self.mse_loss(y, self.predict(x)) + self.lambda_ * abs(self.a)
            self.loss_history.append(loss)
        return self

print("\nRidge vs Lasso comparison:")
ridge = RidgeRegression(lambda_=0.1, epochs=5000).fit_bgd(x, y)
lasso = LassoRegression(lambda_=0.1, epochs=5000).fit_bgd(x, y)
print(f"Ridge: a={ridge.a:.4f}")
print(f"Lasso: a={lasso.a:.4f}")
```

---

## 🔵 [ENHANCED] — The Bias-Variance Tradeoff Visualized

```python
# Conceptual demonstration of underfitting vs overfitting

def polynomial_regression_demo():
    """
    Fit polynomials of different degrees to show:
    - degree 1: underfitting (high bias)
    - degree 3: good fit
    - degree 9: overfitting (high variance)
    """
    np.random.seed(42)
    x_train = np.linspace(0, 1, 20)
    y_train = np.sin(2 * np.pi * x_train) + np.random.normal(0, 0.3, 20)
    
    for degree in [1, 3, 9]:
        # Build polynomial features
        X = np.vstack([x_train**i for i in range(degree + 1)]).T
        # Least squares solution: w = (X^T X)^{-1} X^T y
        w = np.linalg.lstsq(X, y_train, rcond=None)[0]
        y_pred = X @ w
        mse = np.mean((y_train - y_pred)**2)
        
        status = {1: "UNDERFITTING ↓", 3: "GOOD FIT ✓", 9: "OVERFITTING ↑"}
        print(f"Degree {degree}: MSE = {mse:.4f} → {status[degree]}")

polynomial_regression_demo()
```

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> Given the update formula: `a_new = a + 0.0001 * mean((y_true - y_pred) * x)` Manually compute 2 iterations starting from a=0, b=0 for x=[1,2,3], y=[2,4,6].

### Medium

```python
# Implement k-Fold Cross Validation from scratch
def k_fold_cross_validation(x: np.ndarray, y: np.ndarray, 
                            k: int, model_class, **model_params) -> float:
    """
    Split data into k folds.
    Train on k-1 folds, validate on 1.
    Repeat k times. Return mean R² across all folds.
    
    YOUR TASK: Implement this.
    """
    # TODO: implement
    pass

# Test with k=5 on the lecture dataset
```

### Hard

> Implement **Polynomial Regression** from scratch (transform features, apply gradient descent). Then implement **Grid Search** to find the best (degree, learning_rate, lambda_) combination using cross-validation.

---

# Lecture 6 — Machine Learning: Logistic Regression

---

## 📋 [YOUR NOTES] — Logistic Regression

**Purpose**: Solves **binary classification** problems.

**Sigmoid Function (Logistic Function)**: $$P(Y|x) = \frac{1}{1 + e^{-(wx+b)}}$$

- Maps any real number to (0, 1)
- If P > 0.5 → Class 1; else → Class 0

**Loss Function** (Cross-Entropy / Log Loss): $$J(w) = -\frac{1}{m}\sum\left[y\ln(y_p) + (1-y)\ln(1-y_p)\right]$$

**Weight Update (Gradient Descent)**: $$b = b + \alpha \cdot (y - \hat{y}) \cdot \hat{y} \cdot (1 - \hat{y}) \cdot x$$

**Relation to Linear Regression**:

- Both are _generalized linear models_
- Logistic adds sigmoid + threshold → classification
- Linear outputs continuous values → regression

---

## 📋 [YOUR NOTES] — Classification Performance Metrics

**Confusion Matrix:**

||Predicted Yes|Predicted No|
|---|---|---|
|**Actual Yes**|TP|FN|
|**Actual No**|FP|TN|

**Key Metrics:**

- **Accuracy** = (TP + TN) / (P + N)
- **Precision** = TP / (TP + FP)
- **Recall** = TP / P
- **F1 Score** = 2 × (Precision × Recall) / (Precision + Recall)

**Example** (cat classifier, 200 images, 170 actual cats, model says 160 are cats, 140 correct):

- Precision = 140/160 = 87.5%
- Recall = 140/170 = 82.4%
- Accuracy = (140+10)/200 = 75%

---

## 📋 [YOUR NOTES] — Softmax (Multi-Class Extension)

For K classes: $$p(y=k|x;w) = \frac{e^{w_k^T x}}{\sum_{l=1}^{K} e^{w_l^T x}}, \quad k=1,2,...,K$$

All probabilities sum to 1. The class with highest probability is selected.

---

## 🔵 [ENHANCED] — Logistic Regression: Full Python Implementation

```python
import numpy as np
from typing import Tuple, List, Dict

# ============================================================
# LOGISTIC REGRESSION FROM SCRATCH
# ============================================================

class LogisticRegression:
    """
    Binary Logistic Regression trained with gradient descent.
    
    Model: P(y=1|x) = sigmoid(wx + b)
    Loss: Binary Cross-Entropy
    """
    
    def __init__(self, learning_rate: float = 0.01, epochs: int = 1000):
        self.lr = learning_rate
        self.epochs = epochs
        self.weights = None  # shape: (n_features,)
        self.bias = 0.0
        self.loss_history = []
        self.accuracy_history = []
    
    @staticmethod
    def sigmoid(z: np.ndarray) -> np.ndarray:
        """Numerically stable sigmoid."""
        # Clip z to avoid overflow in exp
        z = np.clip(z, -500, 500)
        return 1 / (1 + np.exp(-z))
    
    def forward(self, X: np.ndarray) -> np.ndarray:
        """Compute probabilities: P(y=1|X)."""
        z = X @ self.weights + self.bias
        return self.sigmoid(z)
    
    def binary_cross_entropy(self, y_true: np.ndarray, 
                             y_pred: np.ndarray) -> float:
        """Cross-entropy loss. Adds small epsilon to avoid log(0)."""
        eps = 1e-15
        y_pred = np.clip(y_pred, eps, 1 - eps)
        return -np.mean(y_true * np.log(y_pred) + (1 - y_true) * np.log(1 - y_pred))
    
    def fit(self, X: np.ndarray, y: np.ndarray) -> 'LogisticRegression':
        """Train using batch gradient descent."""
        n_samples, n_features = X.shape
        self.weights = np.zeros(n_features)
        self.bias = 0.0
        
        for epoch in range(self.epochs):
            # Forward pass
            y_pred = self.forward(X)
            
            # Compute gradients
            error = y_pred - y
            grad_w = (X.T @ error) / n_samples
            grad_b = np.mean(error)
            
            # Update parameters
            self.weights -= self.lr * grad_w
            self.bias -= self.lr * grad_b
            
            # Track metrics
            loss = self.binary_cross_entropy(y, y_pred)
            self.loss_history.append(loss)
            
            acc = np.mean((y_pred >= 0.5) == y)
            self.accuracy_history.append(acc)
        
        return self
    
    def predict_proba(self, X: np.ndarray) -> np.ndarray:
        """Return probability of class 1."""
        return self.forward(X)
    
    def predict(self, X: np.ndarray, threshold: float = 0.5) -> np.ndarray:
        """Return binary predictions."""
        return (self.predict_proba(X) >= threshold).astype(int)


# ============================================================
# PERFORMANCE METRICS
# ============================================================

class ClassificationMetrics:
    """All classification performance metrics from your notes."""
    
    def __init__(self, y_true: np.ndarray, y_pred: np.ndarray):
        self.TP = np.sum((y_true == 1) & (y_pred == 1))
        self.TN = np.sum((y_true == 0) & (y_pred == 0))
        self.FP = np.sum((y_true == 0) & (y_pred == 1))
        self.FN = np.sum((y_true == 1) & (y_pred == 0))
        self.P = self.TP + self.FN   # Total actual positives
        self.N = self.TN + self.FP   # Total actual negatives
    
    @property
    def accuracy(self) -> float:
        return (self.TP + self.TN) / (self.P + self.N)
    
    @property
    def precision(self) -> float:
        return self.TP / (self.TP + self.FP) if (self.TP + self.FP) > 0 else 0
    
    @property
    def recall(self) -> float:
        return self.TP / self.P if self.P > 0 else 0
    
    @property
    def specificity(self) -> float:
        return self.TN / self.N if self.N > 0 else 0
    
    @property
    def f1_score(self) -> float:
        p, r = self.precision, self.recall
        return 2 * p * r / (p + r) if (p + r) > 0 else 0
    
    def f_beta(self, beta: float) -> float:
        """Fβ score: β > 1 favors recall, β < 1 favors precision."""
        p, r = self.precision, self.recall
        return (1 + beta**2) * p * r / (beta**2 * p + r) if (beta**2 * p + r) > 0 else 0
    
    def confusion_matrix(self) -> np.ndarray:
        return np.array([[self.TP, self.FN], [self.FP, self.TN]])
    
    def report(self):
        print("=== Classification Report ===")
        print(f"Confusion Matrix:")
        print(f"  TP={self.TP}  FN={self.FN}")
        print(f"  FP={self.FP}  TN={self.TN}")
        print(f"\nAccuracy:    {self.accuracy:.2%}")
        print(f"Precision:   {self.precision:.2%}")
        print(f"Recall:      {self.recall:.2%}")
        print(f"Specificity: {self.specificity:.2%}")
        print(f"F1 Score:    {self.f1_score:.2%}")


# ============================================================
# DEMO: Lecture's example dataset
# ============================================================
from sklearn.datasets import make_classification

# Generate a binary classification dataset
np.random.seed(42)
X_demo, y_demo = make_classification(
    n_samples=100, n_features=2, n_redundant=0,
    n_clusters_per_class=1, random_state=42
)

# Split into train/test (80/20)
split = int(0.8 * len(X_demo))
X_train, X_test = X_demo[:split], X_demo[split:]
y_train, y_test = y_demo[:split], y_demo[split:]

# Train
model = LogisticRegression(learning_rate=0.1, epochs=1000)
model.fit(X_train, y_train)

# Evaluate
y_pred = model.predict(X_test)
metrics = ClassificationMetrics(y_test, y_pred)
metrics.report()

print(f"\nFinal training loss: {model.loss_history[-1]:.4f}")
print(f"Final training accuracy: {model.accuracy_history[-1]:.2%}")
```

### Softmax Regression (Multi-Class)

```python
class SoftmaxRegression:
    """
    Multi-class Logistic Regression using Softmax.
    Generalization of LogisticRegression to K classes.
    """
    
    def __init__(self, learning_rate: float = 0.01, epochs: int = 1000):
        self.lr = learning_rate
        self.epochs = epochs
        self.weights = None   # shape: (n_features, n_classes)
        self.bias = None      # shape: (n_classes,)
    
    @staticmethod
    def softmax(z: np.ndarray) -> np.ndarray:
        """Numerically stable softmax."""
        z_shifted = z - np.max(z, axis=1, keepdims=True)  # stability
        exp_z = np.exp(z_shifted)
        return exp_z / np.sum(exp_z, axis=1, keepdims=True)
    
    def fit(self, X: np.ndarray, y: np.ndarray) -> 'SoftmaxRegression':
        n_samples, n_features = X.shape
        n_classes = len(np.unique(y))
        
        self.weights = np.zeros((n_features, n_classes))
        self.bias = np.zeros(n_classes)
        
        # One-hot encode y
        y_onehot = np.eye(n_classes)[y]
        
        for epoch in range(self.epochs):
            # Forward: compute probabilities for each class
            z = X @ self.weights + self.bias       # (n, K)
            probs = self.softmax(z)                 # (n, K)
            
            # Gradient (same form as binary, but for K classes)
            error = probs - y_onehot               # (n, K)
            grad_w = (X.T @ error) / n_samples     # (features, K)
            grad_b = np.mean(error, axis=0)        # (K,)
            
            self.weights -= self.lr * grad_w
            self.bias -= self.lr * grad_b
        
        return self
    
    def predict_proba(self, X: np.ndarray) -> np.ndarray:
        z = X @ self.weights + self.bias
        return self.softmax(z)
    
    def predict(self, X: np.ndarray) -> np.ndarray:
        return np.argmax(self.predict_proba(X), axis=1)


# Demo: 3-class problem
from sklearn.datasets import make_blobs
X_multi, y_multi = make_blobs(n_samples=150, centers=3, random_state=42)

softmax_model = SoftmaxRegression(learning_rate=0.01, epochs=1000)
softmax_model.fit(X_multi, y_multi)
y_pred_multi = softmax_model.predict(X_multi)
print(f"\nSoftmax accuracy: {np.mean(y_pred_multi == y_multi):.2%}")

# Show probabilities for first sample
probs = softmax_model.predict_proba(X_multi[:1])[0]
for i, p in enumerate(probs):
    print(f"  Class {i}: {p:.2%}")
```

---

## 🔵 [ENHANCED] — Key Insights: Logistic vs Linear

```
Linear Regression:
  Input X → weighted sum z = wx + b → output z (continuous)
  Loss: MSE = mean((y - z)²)
  
Logistic Regression:
  Input X → weighted sum z = wx + b → sigmoid(z) = probability
  Loss: Binary Cross-Entropy = -mean(y*log(p) + (1-y)*log(1-p))

Why not use MSE for classification?
  - MSE penalizes wrong predictions less aggressively
  - MSE is not convex for sigmoid → gradient descent may not converge
  - Cross-entropy has nice gradient: ∂J/∂w = mean((ŷ - y) * x)
```

---

## 🔵 [ENHANCED] — References to Review

- **Prerequisites**: derivatives (chain rule for backprop), probability basics (Bayes' theorem), linear algebra (matrix multiplication)
- **Related Topics**:
    - Neural Networks (logistic regression = single-layer neural net)
    - SVM (another classifier with different loss function)
    - Decision Trees (non-linear classifier)
    - ROC Curves (visualization of classifier performance)
- **Python Libraries**: `scikit-learn`, `numpy`, `matplotlib`

---

## 🔵 [ENHANCED] — Practice Tasks

### Easy

> For a confusion matrix with TP=50, TN=30, FP=10, FN=10: Calculate Accuracy, Precision, Recall, and F1 Score manually.

### Medium

```python
# Implement a threshold tuner for logistic regression
def find_best_threshold(model, X_val, y_val, 
                       metric: str = 'f1') -> float:
    """
    Try different thresholds (0.1 to 0.9, step 0.1).
    Return the threshold that maximizes the given metric.
    
    YOUR TASK: Implement this.
    Hint: Use ClassificationMetrics for each threshold.
    """
    # TODO: implement
    pass
```

### Hard

> Implement **ROC Curve** computation from scratch:
> 
> 1. For thresholds from 0 to 1, compute TPR (recall) and FPR (1-specificity)
> 2. Compute AUC (Area Under Curve) using the trapezoidal rule
> 3. Plot the curve and compare your implementation with sklearn's `roc_auc_score`

---

# 🔵 [ENHANCED] — Cross-Topic Summary: The Big Picture

```
AI
│
├── Rule-Based Systems (Lecture 2)
│   └── If rules are known and explicit → Expert Systems
│
├── Search Algorithms (Lecture 3)
│   ├── Uninformed: BFS (queue), DFS (stack)
│   ├── Informed: A* (priority queue + heuristic)
│   └── Adversarial: Minimax + Alpha-Beta
│
└── Machine Learning (Lectures 4-6)
    ├── Supervised
    │   ├── Regression → Linear Regression (continuous output)
    │   └── Classification → Logistic Regression (binary/multi-class)
    ├── Key Concepts
    │   ├── Loss functions (MSE, Cross-Entropy)
    │   ├── Gradient Descent (BGD, SGD, MBGD)
    │   ├── Overfitting → Regularization (L1/L2), Dropout, Early Stop
    │   └── Evaluation (Accuracy, Precision, Recall, F1, R²)
    └── Unsupervised → Clustering (next topic)
```

---

## 🔵 [ENHANCED] — Common Mistakes & Misconceptions

|Mistake|Reality|
|---|---|
|"BFS always finds shortest path"|Only true for unweighted graphs. Use Dijkstra/A* for weighted.|
|"More data = always better model"|Bad data (noise, bias) can hurt. Data quality > quantity.|
|"High accuracy = good model"|With 95% negative class, predicting always-negative gives 95% accuracy but is useless.|
|"L1 and L2 do the same thing"|L1 can zero out features (sparse), L2 just shrinks them.|
|"Logistic regression is for regression"|Despite the name, it's a **classifier**.|
|"A* always finds optimal path"|Only if heuristic is **admissible**. Bad heuristic = suboptimal path.|
|"Backward chaining is always better"|Forward: many possible conclusions. Backward: specific goal to verify.|

---

## 🔵 [ENHANCED] — Python Setup & Libraries

```bash
# Install all libraries needed for this course
pip install numpy scikit-learn matplotlib seaborn
pip install jupyter  # for interactive notebooks

# Optional but useful
pip install networkx  # for graph visualization
pip install plotly    # for interactive plots
```

```python
# Quick sanity check
import numpy as np
import sklearn
import matplotlib
print("All libraries installed successfully!")
print(f"numpy: {np.__version__}")
print(f"scikit-learn: {sklearn.__version__}")
```

---

_This document was generated as an enhanced learning resource based on university AI course notes._  
_Original lecture content preserved; enhancements clearly marked._