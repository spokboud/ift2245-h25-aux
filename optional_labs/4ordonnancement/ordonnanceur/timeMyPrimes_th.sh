#!/bin/bash
# https://www.gantt.com/fr/
# nice -n 10 ./primes_th 10000000 10 &

# pgrep ./primes_th
# ps -l

{ /usr/bin/time --verbose nice -n 19 ./primes_th 9000000 10; } 2>&1 | grep -E 'Elapsed|User|System time|Voluntary|Involuntary'; echo "nice 19"; \
{ /usr/bin/time --verbose nice -n 0 ./primes_th 9000000 10; } 2>&1 | grep -E 'Elapsed|User|System time|Voluntary|Involuntary'; echo "nice 0"; \
{ /usr/bin/time --verbose chrt --idle 0 ./primes_th 9000000 10; } 2>&1 | grep -E 'Elapsed|User|System time|Voluntary|Involuntary'; echo "chrt idle 0"; \
{ /usr/bin/time --verbose sudo chrt --rr 50 ./prime_th 50000000 10; } 2>&1 | grep -E 'Elapsed|User|System time|Voluntary|Involuntary'; echo "chrt rr 50"; \
{ /usr/bin/time --verbose sudo nice -n -20 ./prime_th 50000000 10; } 2>&1 | grep -E 'Elapsed|User|System time|Voluntary|Involuntary'; echo "nice -20";

# man time
# man nice Niceness  values  range  from  -20  (most  favorable  to  the process) to 19 (least favorable to the process).
# man chrt sets  or  retrieves  the real-time scheduling attributes of an existing pid, or runs command with the given attributes.
#
#             -i, --idle
           #              Set scheduling policy to SCHED_IDLE (Linux-specific, supported since 2.6.23).   The
           #              priority argument has to be set to zero.
# ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           # SJF (shortest job first)
           # RR round-robin
           # préemption  est la capacité d'un système d'exploitation multitâche d'interrompre une tâche en cours en faveur d'une autre.
           # FIFO first in first out
           # SRT Shortest Remaining Time

#PROCESS FLAGS
#       The sum of these values is displayed in the "F" column, which is provided by the flags output specifier:
#               0    ?? same as 1 but forked did exec ??
#               1    forked but didn't exec
#               4    used super-user privileges
