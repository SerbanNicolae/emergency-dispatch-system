# 🚨 Emergency Dispatch System

![Language](https://img.shields.io/badge/language-C99-blue.svg)
![Build](https://img.shields.io/badge/build-Makefile-brightgreen.svg)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)

A command-driven **emergency dispatch simulator** written in C. It manages response units, prioritized incidents and active interventions, and supports undoing the last dispatch. All data structures are built from scratch with manual memory management.

---

## 📋 Table of Contents

- [Features](#-features)
- [Data Structures](#-data-structures)
- [Architecture](#-architecture)
- [Getting Started](#-getting-started)
- [Input Format](#-input-format)
- [Commands](#-commands)
- [Example](#-example)

---

## ✨ Features

- **Priority-based dispatching:** incidents are served in `high` → `medium` → `low` order.
- **FIFO unit allocation:** the unit that has been available the longest gets dispatched first.
- **Undo support:** reverts the most recent active dispatch and restores the system state.
- **Incident lifecycle tracking:** `queued` → `intervened` → `solved`.
- **Dynamic input parsing:** incident descriptions of any length are read with a growing buffer.
- **Manual memory management:** every structure has its own create/free routines.

---

## 🧱 Data Structures

| Structure | Implementation | Purpose |
|---|---|---|
| **Unit list** | Singly linked list | Stores all registered response units |
| **Available units queue** | Linked-list queue (FIFO) | Units ready to be dispatched |
| **Incident list** | Circular doubly linked list with sentinel | Stores every reported incident |
| **Priority queues** (×3) | Linked-list queues (FIFO) | One queue per priority: `high`, `medium`, `low` |
| **Intervention list** | Circular doubly linked list with sentinel + tail pointer | Active and completed interventions |
| **Intervention stack** | Linked-list stack (LIFO) | Dispatch history used by `UNDO_LAST_DISPATCH` |

### Design notes

- **Sentinel nodes** remove edge cases when inserting into or deleting from an empty list.
- **Queues and the stack store pointers** to nodes in the main lists, so data is never duplicated.
- **Tail pointer** on the intervention list gives O(1) appends.
- **Undo** pops the stack until it finds an `intervened` entry, then:
  - puts the incident back at the **front** of its priority queue,
  - returns the unit to the **back** of the available units queue,
  - removes the intervention from the list.

---

## 🏗 Architecture

```
.
├── main.c            # Entry point: reads input and dispatches commands
├── unit.h / unit.c   # Units: list, availability queue, display
├── incident.h / .c   # Incidents: list, priority queues, input parsing
├── intervention.h/.c # Interventions: list, undo stack, dispatch logic, cleanup
└── Makefile
```

```mermaid
flowchart LR
    IN[tema1.in] --> MAIN[main.c]
    MAIN --> U[unit]
    MAIN --> I[incident]
    MAIN --> V[intervention]
    V --> U
    V --> I
    MAIN --> OUT[tema1.out]
```

---

## 🚀 Getting Started

### Requirements

- GCC (C99)
- GNU Make

### Build & Run

```bash
git clone git@github.com:SerbanNicolae/emergency-dispatch-system.git
cd emergency-dispatch-system

make          # build
make run      # run (reads tema1.in, writes tema1.out)
make clean    # remove the binary
```

---

## 📥 Input Format

The program reads from `tema1.in` and writes results to `tema1.out`.

```
<number_of_units>
<unit_id> <unit_type>
...
<number_of_commands>
<command> [arguments]
...
```

---

## 🕹 Commands

| Command | Arguments | Description |
|---|---|---|
| `ADD_INCIDENT` | `<id> <priority> <description>` | Registers an incident and enqueues it by priority |
| `CHECK_UNITS_AVAILABILITY` | – | Prints the number of available units |
| `DISPATCH` | – | Assigns the highest-priority incident to the first available unit |
| `UNDO_LAST_DISPATCH` | – | Reverts the most recent active dispatch |
| `SOLVED_INCIDENT` | `<id>` | Marks an incident as solved and frees its unit |
| `SHOW_UNIT` | `<id>` | Prints a unit's type and availability |
| `SHOW_INCIDENT` | `<id>` | Prints an incident's priority, description and status |
| `SHOW_INTERVENTIONS` | – | Lists all interventions and their status |

Invalid operations print `INVALID OPERATION! ERROR 404`.

---

## 💡 Example

**`tema1.in`**
```
3
1 A
2 P
3 M
9
ADD_INCIDENT 101 low Small fire in a parking lot
ADD_INCIDENT 102 high Car accident on the highway
ADD_INCIDENT 103 medium Person trapped in elevator
DISPATCH
DISPATCH
CHECK_UNITS_AVAILABILITY
UNDO_LAST_DISPATCH
SOLVED_INCIDENT 102
SHOW_INTERVENTIONS
```

**`tema1.out`**
```
Number of available units: 1
Incident 102 was assigned to unit 1, and has the following status: "solved"
```

**What happens:**
1. The `high` incident (102) is dispatched first, then the `medium` one (103).
2. One unit is left available.
3. The undo reverts incident 103 and returns its unit to the queue.
4. Incident 102 is solved, so only its intervention remains.

---

## 👤 Author

**Șerban Nicolae** – [@SerbanNicolae](https://github.com/SerbanNicolae)
