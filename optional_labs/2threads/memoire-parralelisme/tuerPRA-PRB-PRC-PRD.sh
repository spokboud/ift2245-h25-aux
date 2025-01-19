#!/bin/bash
kill -s 9 `pgrep pra` &>> /dev/null
kill -s 9 `pgrep prb` &>> /dev/null
kill -s 9 `pgrep prc` &>> /dev/null
kill -s 9 `pgrep prd` &>> /dev/null