# Synchro

## 1. Threads

### 1.1 Overview
- **Lightweight Processes**: Threads share the same address space but have their own stack pointer and registers.  
- **Concurrency vs. Parallelism**: On single-core systems, threads take turns. On multi-core systems, truly simultaneous execution is possible.  
- **Communication**: Shared memory allows easy data sharing but also introduces data races if not synchronized.  

### 1.2 Creation and Joining

<details>

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* myThreadFunc(void* arg) {
    printf("Thread %ld running\n", (long) arg);
    // Do thread work here
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];
    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, myThreadFunc, (void*)i);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
```
</details>

**Key Points**:
- `pthread_create()` spawns a new thread, which starts executing `myThreadFunc`.
- `pthread_join()` waits for the thread to finish, cleaning up resources.

### 1.3 Common Pitfalls
- **Race Conditions**: Two threads accessing and modifying shared data simultaneously.
- **Starvation**: One thread hogs CPU or resources, other threads never get enough CPU time.
- **Excessive Context Switching**: Too many threads can degrade performance.

### 1.4 Relevant man Pages
- `man pthread_create`
- `man pthread_join`
- `man pthread_exit`
- `man pthread_self`

---

## 2. Mutexes

### 2.1 Overview
- **Mutual Exclusion**: Ensures only one thread accesses a shared resource at a time.
- **Lock Granularity**: Fine-grained locks can boost concurrency but complicate design. Coarse-grained locks are easier but reduce parallelism.

### 2.2 Locking and Unlocking

<details>
<summary><strong>Example in C (POSIX Mutex)</strong></summary>

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int sharedCounter = 0;

void* incrementFunc(void* arg) {
    for(int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        sharedCounter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, incrementFunc, NULL);
    pthread_create(&t2, NULL, incrementFunc, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final counter: %d\n", sharedCounter);
    return 0;
}
```
</details>

**Key Points**:
- `pthread_mutex_lock()` and `pthread_mutex_unlock()` protect the shared resource.
- If you forget to unlock the mutex or take too long, other threads stall.

### 2.3 Common Pitfalls
- **Deadlock**: Cyclical waiting if multiple locks are acquired in inconsistent order.
- **Priority Inversion**: A low-priority thread holds a lock needed by a high-priority thread.
- **Long Lock-Hold Times**: Reduces overall system throughput.

### 2.4 Relevant man Pages
- `man pthread_mutex_init`
- `man pthread_mutex_lock`
- `man pthread_mutex_unlock`
- `man pthread_mutex_destroy`

---

## 3. Semaphores

### 3.1 Overview
- **Counting Semaphore**: Allows multiple threads to acquire up to a set limit.
- **Binary Semaphore**: Behaves like a single lock (similar to a mutex but without ownership tracking).

### 3.2 Usage Examples

<details>
<summary><strong>Binary Semaphore Example</strong></summary>

```c
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

sem_t binSem;
int sharedVal = 0;

void* routine(void* arg) {
    sem_wait(&binSem);         // Acquire
    sharedVal++;               // Critical section
    sem_post(&binSem);         // Release
    return NULL;
}

int main() {
    pthread_t threads[5];
    sem_init(&binSem, 0, 1);   // 0 for threads within same process, 1 is the initial value

    for(int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, routine, NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final sharedVal: %d\n", sharedVal);
    sem_destroy(&binSem);
    return 0;
}
```
</details>

<details>
<summary><strong>Counting Semaphore Example</strong></summary>

```c
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

sem_t countSem;

void* routine(void* arg) {
    // Acquire a "slot"
    sem_wait(&countSem);
    // Do something with the resource
    // ...
    sem_post(&countSem);  // Release the "slot"
    return NULL;
}

int main() {
    pthread_t threads[10];
    sem_init(&countSem, 0, 3); // Only 3 threads can enter at once

    for(int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, routine, NULL);
    }
    for(int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&countSem);
    return 0;
}
```
</details>

### 3.3 Common Pitfalls
- **Never Signaled**: If a thread exits without calling `sem_post`, other threads wait indefinitely.
- **Complexity**: Many semaphores can lead to intricate synchronization and subtle bugs.
- **No Ownership Tracking**: A semaphore doesn’t identify which thread did `sem_wait`, so debugging can be harder.

### 3.4 Relevant man Pages
- `man sem_init`
- `man sem_wait`
- `man sem_post`
- `man sem_destroy`
- For named semaphores: `man sem_open`, `man sem_close`, `man sem_unlink`

---

## 4. Stories from the Trenches

1. **Producer-Consumer Nightmare**  
   A team once used a counting semaphore incorrectly—every time a product was produced, they forgot to `sem_post()`. The consumer threads hung forever, halting the pipeline. Nobody knew why until they sprinkled debug prints and realized the semaphore was never signaled.

2. **The Bank Transfer Deadlock**  
   Two threads each locked a customer record (via mutex) to transfer funds. Lock order was reversed in the second transfer. Result? Each thread waited for the other to unlock first—deadlock, and the entire banking system froze.

3. **Spacecraft Concurrency**  
   A real anecdote from NASA’s Apollo program: concurrency bugs nearly scuttled a mission when the computer became overloaded, missing deadlines. The fix? They prioritized critical threads so the lander didn’t crash. *(We should be thankful that they tested well!)*

---

## 5. Diagrams

### 5.1 Thread and Mutex Relationship

```
   +-----------------------+
   |       THREAD 1       |
   |  lock(mutex)         |
   |  -- critical sect. --|
   |  unlock(mutex)       |
   +-----------------------+
           |
           v
      -------------
(mutex) | MUTEX | locked or free
      -------------
           ^
           |
   +-----------------------+
   |       THREAD 2       |
   |   waits for lock     |
   |  -- critical sect. --|
   |  unlock(mutex)       |
   +-----------------------+
```

### 5.2 Semaphore Flow (Counting)

```
   Thread 1    Thread 2    Thread 3    Semaphore Count = X
      |           |           |
      v           v           v
   sem_wait()  sem_wait()  sem_wait()  --> each reduces Count by 1
      |           |           |
   [Critical]  [Critical]  [Critical]
      |           |           |
   sem_post()  sem_post()  sem_post()  --> each increases Count by 1
      |           |           |
      -------------------------------- Count returns to X
```

---

## 6. Wrap-Up

### Key Takeaways
- **Plan Lock Order**: Always acquire mutexes in a consistent order to avoid deadlocks.
- **Keep Locks Short**: Minimize the time spent in critical sections.
- **Test Under Load**: Concurrency bugs often surface only under stress.
- **Balance**: Too many threads can be as harmful as too few.
- **Stay Vigilant**: Concurrency issues can be subtle—profilers, static analyzers, and race detectors help.

### Further Reading
- [POSIX Threads Programming](https://pubs.opengroup.org/onlinepubs/007908799/xsh/pthread.h.html)
- Advanced books like *“Programming with POSIX Threads”* by David R. Butenhof.
- [Intel Threading Building Blocks](https://www.intel.com/content/www/us/en/developer/tools/oneapi/threading-building-blocks.html) for higher-level concurrency models.

---

> **Man Pages to Explore**
> - `man pthread_create`
> - `man pthread_join`
> - `man pthread_mutex_lock`
> - `man pthread_mutex_unlock`
> - `man sem_wait`
> - `man sem_post`

Concurrency is powerful but unforgiving. A small oversight—one missing `unlock()` or `sem_post()`—can bring your entire system to a halt
