# Operating-System-Projects
Projects for os class
assignment #1
producer-consumer issue solved with semaphore 

# Producer–Consumer Problem (Bounded Buffer, Linux)

This project implements the **Producer–Consumer problem** using:
- **Two processes** (`producer` and `consumer`)
- **Shared memory** (`shm_open`, `mmap`)
- **Counting semaphores** (`empty`, `full`)
- **A process-shared `pthread_mutex_t`** for mutual exclusion
- **Table of size 2** (the bounded buffer)

---
| File | Description |
|------|--------------|
| `producer.c` | Producer process — generates random items and places them on the shared table. |
| `consumer.c` | Consumer process — removes items from the shared table. |
| `Makefile` | Builds both executables (`producer` and `consumer`). |
| `report.txt` | Technical explanation of design, synchronization, and results. |

---

- **Linux / Unix environment**
- **gcc** with `pthread` and `rt` support  
  (Install with: `sudo apt install build-essential`)
- POSIX shared memory and semaphore support  
  (link with `-pthread -lrt`)

---

## Build Instructions
```bash
make
