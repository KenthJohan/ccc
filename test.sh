#!/bin/bash

gcc test_csc_htable.c && ./a > test_csc_htable.log 2>&1
echo test_csc_htable.c $?
gcc test_csc_dlist.c && ./a > test_csc_dlist.log 2>&1
echo test_csc_dlist.c $?
gcc test_csc_str.c && ./a > test_csc_str.log 2>&1
echo test_csc_str.c $?