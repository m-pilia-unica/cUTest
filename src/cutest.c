/*
 * This file is part of cUTest.
 *
 * cUTest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cUTest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cUTest. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Martino Pilia, 2015
 */

/*!
 * \file cutest.c
 *
 * @author Martino Pilia
 * @date 2015-01-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cutest.h"

/*!
 * Initialize a new suite, allocating memory for it and setting all entries
 * to default values. A newly initialized contains no test cases.
 */
int suite_new(
        Suite **s,
        const char *name,
        void (*bef)(void),
        void (*aft)(void))
{
    *s = (Suite*) malloc(sizeof (Suite));

    if (*s == NULL)
    {
        perror("suite_new: malloc error.\n");
        exit(EXIT_FAILURE);
    }

    ll_init(&(*s)->asserts);
    strncpy((*s)->name, name, NAME_LEN);
    (*s)->before = bef;
    (*s)->after = aft;

    return 0;
}

/*!
 * Add the test case to the suite, labelling the test with the provided
 * name.
 */
int suite_add(Suite *s, void (*t)(Status*), const char *name)
{
    Test_case *tc = (Test_case*) malloc(sizeof (Test_case));

    if (tc == NULL)
    {
        perror("suite_add: malloc error.\n");
        exit(EXIT_FAILURE);
    }

    tc->fun = t;
    strncpy(tc->name, name, NAME_LEN);

    ll_push_front(&s->asserts, (void*) tc);

    return 0;
}

/*!
 * Run a suite of test cases. Test cases are executed sequentially, following
 * the order used to add them to the suite. Each test case runs in a separate
 * process. Before each test case, the statements defined with
 * the BEFORE_TEST(name) are executed, then the test runs, and after 
 * test termination the statements defined inside the AFTER_TEST(name) 
 * are executed.
 */
int suite_run(Suite *s)
{
    Test_case *tc;
    Status st = {};
    ll_iterator it = ll_get_iterator(s->asserts);
    pid_t pid;
    int i;
    int res;
    int successes = 0;
    int fails = 0;
    int errors = 0;
    int tot = s->asserts.size;
    int char_num;
    int fd[2];
    int status;
    int term_sig;
    int exit_status;

    res = pipe(fd);
    if (res)
    {
        perror("suite_run: pipe error.\n");
        exit(EXIT_FAILURE);
    }

    printf("** Starting suite \"%s\" **\n", s->name);

    if (s->asserts.size < 1) /* empty suite */
    {
        printf("  Suite \"%s\" does not contain any test case.\n", s->name);
        return 0;
    }

    for (i = 0; i < tot; i++)
    {
        st.failed = 0;
        tc = (Test_case*) ll_next(&it);

        /* run BEFORE_TEST procedure if present */
        if (s->before != NULL)
        {
            pid = fork();
            switch (pid)
            {
                case -1:
                    perror("suite_run: fork error.\n");
                    exit(EXIT_FAILURE);

                case 0: /* child: exec BEFORE_TEST procedure and exit */
                    s->before();
                    exit(0);

                default: /* parent: wait for child termination */
                    waitpid(pid, &status, 0);
            }

            if(WIFSIGNALED(status)) /* child process signaled */
            {
                errors++;
                term_sig = WTERMSIG(status);
                printf( "Suite \"%s\", test case \"%s\", error:\n"
                        "  BEFORE_TEST procedure terminated by signal %d. "
                        "  Test case execution aborted.\n\n",
                        s->name,
                        tc->name,
                        term_sig);
                continue;
            }

            if (!WIFEXITED(status)) /* child process did not quit normally */
            {
                errors++;
                exit_status = WEXITSTATUS(status);
                printf( "Suite \"%s\", test case \"%s\", error:\n"
                        "  BEFORE_TEST procedure failed with status %d."
                        "  Test case execution aborted.\n\n",
                        s->name,
                        tc->name,
                        exit_status);
                continue;
            }
        }

        /* run current test case */
        pid = fork();
        switch (pid)
        {
            case -1:
                perror("suite_run: fork error.\n");
                exit(EXIT_FAILURE);

            case 0: /* child: exec test, write result and exit */
                tc->fun(&st); /* run test case function */
                write(fd[1], &st, sizeof (Status)); /* write result */
                exit(0);

            default: /* parent: wait for child termination */
                waitpid(pid, &status, 0);
        }

        if(WIFSIGNALED(status)) /* child process signaled */
        {
            errors++;
            term_sig = WTERMSIG(status);
            printf( "Suite \"%s\", test case \"%s\", error:\n"
                    "  test terminated by signal %d.\n\n",
                    s->name,
                    tc->name,
                    term_sig);
            continue;
        }

        if (!WIFEXITED(status)) /* child process did not terminate normally */
        {
            errors++;
            exit_status = WEXITSTATUS(status);
            printf( "Suite \"%s\", test case \"%s\", error:\n"
                    "  test failed with status %d.\n\n",
                    s->name,
                    tc->name,
                    exit_status);
            continue;
        }

        /* run AFTER_TEST procedure if present */
        if (s->after != NULL)
        {
            pid = fork();
            switch (pid)
            {
                case -1:
                    perror("suite_run: fork error.\n");
                    exit(EXIT_FAILURE);

                case 0: /* child: exec AFTER_TEST procedure and exit */
                    s->after();
                    exit(0);

                default: /* parent: wait for child termination */
                    waitpid(pid, &status, 0);
            }

            if(WIFSIGNALED(status)) /* child process signaled */
            {
                term_sig = WTERMSIG(status);
                printf( "Suite \"%s\", test case \"%s\", error on cleanup:\n"
                        "  AFTER_TEST procedure terminated by signal %d.\n\n",
                        s->name,
                        tc->name,
                        term_sig);
            }

            else if (!WIFEXITED(status)) /* child did not quit normally */
            {
                exit_status = WEXITSTATUS(status);
                printf( "Suite \"%s\", test case \"%s\", error on cleanup:\n"
                        "  AFTER_TEST procedure failed with status %d.\n\n",
                        s->name,
                        tc->name,
                        exit_status);
            }
        }

        /* test execution complete, read result */
        read(fd[0], &st, sizeof (Status));

        if (st.failed) /* write message if test failed */
        {
            fails++;
            printf( "Suite \"%s\", test case \"%s\", assertion failure:\n"
                    "  %s\n\n",
                    s->name,
                    tc->name,
                    st.assertion);
        }
        
        if (st.invalid) /* write message if test was invalid */
        {
            errors++;
            printf( "Suite \"%s\", test case \"%s\", invalid assertion:\n"
                    "  %s\n"
                    "  %s\n\n",
                    s->name,
                    tc->name,
                    st.assertion,
                    st.invalid);
        }
    }

    /* print suite summary */
    successes = tot - fails - errors;
    char_num = (successes == tot || fails == tot || errors == tot ? 6 : 5);
    printf("\nSuite \"%s\" execution complete:\n"
            " %d success%2s (%*.2f%%)\n"
            " %d failure%s  (%*.2f%%)\n"
            " %d error%s    (%*.2f%%)\n",
            s->name,
            successes,
            successes == 1 ? "  " : "es",
            char_num,
            (float) successes / tot * 100,
            fails,
            fails == 1 ? " " : "s",
            char_num,
            (float) fails / tot * 100,
            errors,
            errors == 1 ? " " : "s",
            char_num,
            (float) errors / tot * 100);

    return 0;
}

/*!
 * This function actually implements floating point number
 * equality assertion.
 */
int __assert_equals_flo(double x, double y, double tol, Status *__s)
{
    if (tol <= 0.0)
    { 
        sprintf((char *) __s->invalid,
                "Invalid \"tol\" value (%.6e). \"Tol\" must be positive",
                tol);
        return 0;
    }
    __s->failed = !(fabs((x) - (y)) < (tol));
    __s->invalid = NULL;
    return 0;
}

/*!
 * This function actually implements int array equality assertion.
 */
int __assert_equals_array_int(
        int *x,
        int *y,
        int len,
        Status *__s)
{
    int i;

    if (len < 1)
    {
        sprintf((char *) __s->invalid,
                "Invalid array length (%d). Length must be > 0.\n", len);
        return 0;
    }

    __s->failed = 0;
    for (i = 0; i < len; ++i)
        __s->failed |= (x[i] != y[i]);

    __s->invalid = NULL;

    return 0;
}
        
/*!
 * This function actually implements floating point array 
 * equality assertion.
 */
int __assert_equals_array_flo(
        double *x,
        double *y,
        int len, 
        double tol,
        Status *__s)
{
    int i;

    if (len < 1)
    {
        sprintf((char *) __s->invalid,
                "Invalid array length (%d). Length must be > 0.\n", len);
        return 0;
    }

    __s->failed = 0;
    for (i = 0; i < len; ++i)
        __s->failed |= fabs(x[i] - y[i]) > tol;

    __s->invalid = NULL;

    return 0;
}

/*!
 * This function actually implements floating point matrix
 * equality assertion.
 */
int __assert_equals_matrix_int(
        int m,
        int n,
        int x[m][n],
        int y[m][n],
        Status *__s)
{
    int i, j;

    if (n < 1 || m < 1)
    {
        sprintf((char *) __s->invalid,
                "Invalid matrix size. Dimension must be > 0.\n");
        return 0;
    }

    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            __s->failed |= (x[i][j] != y[i][j]);

    __s->invalid = NULL;

    return 0;
}
        
/*!
 * This function actually implements int matrix equality assertion.
 */
int __assert_equals_matrix_flo(
        int m, 
        int n,
        double x[m][n],
        double y[m][n],
        double tol,
        Status *__s)
{
    int i, j;

    if (n < 1 || m < 1)
    {
        sprintf((char *) __s->invalid,
                "Invalid matrix size. Dimension must be > 0.\n");
        return 0;
    }

    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            __s->failed |= fabs(x[i][j] - y[i][j]) > tol;

    __s->invalid = NULL;

    return 0;
}
