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

/*
 * Maximum number of characters in suites and test cases name.
 */
#define NAME_LEN 100

/*
 * Mask the definition of a function with the name provided as
 * input and a parameter used to return the execution result to the
 * runner which calls the test.
 */
#define _TEST_CASE(name) void (name)(Status *__s)

/*
 * Mask the definition of a procedure to be executed before each test case.
 */
#define _BEFORE_TEST(name) void name(void)

/*
 * Mask the definition of a procedure to be executed after each test case.
 */
#define _AFTER_TEST(name) void name(void)

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert(expr, msg) \
    __s->assertion = ("assert("#expr", "#msg")"); \
    __s->failed = !(expr); \
    __s->invalid = NULL; \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_false(expr, msg) \
    __s->assertion = ("assert_false("#expr", "#msg")"); \
    __s->failed = (expr); \
    __s->invalid = NULL; \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_int(x, y, msg) \
    __s->assertion = ("assert_equals_int("#x", "#y", "#msg")"); \
    __s->failed = !((x) == (y)); \
    __s->invalid = NULL; \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_flo(x, y, tol, msg) \
    __s->assertion = ("assert_equals_flo("#x", "#y", "#tol", "#msg")"); \
    __assert_equals_flo((x), (y), (tol), __s); \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_str(x, y, msg) \
    __s->assertion = ("assert_equals_str("#x", "#y", "#msg")"); \
    __s->failed = strcmp((x), (y)); \
    __s->invalid = NULL; \
    if (__s->failed) return;

/*
 * Check if a pointer is NULL.
 */
#define _assert_null(x, msg) \
    __s->assertion = ("assert_null("#x", "#msg")"); \
    __s->failed = (x != NULL); \
    __s->invalid = NULL; \
    if (__s->failed) return;

/*
 * Check if a pointer is not NULL.
 */
#define _assert_not_null(x, msg) \
    __s->assertion = ("assert_not_null("#x", "#msg")"); \
    __s->failed = (x == NULL); \
    __s->invalid = NULL; \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_array_int(x, y, len, msg) \
    __s->assertion = ("assert_equals_array_int("#x", "#y", "#len", "#msg")"); \
    __assert_equals_array_int((x), (y), (len), __s); \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_array_flo(x, y, len, tol, msg) \
    __s->assertion = ("assert_equals_array_flo(" \
            #x", "#y", "#len", "#tol", "#msg")"); \
    __assert_equals_array_flo((x), (y), (len), (tol), __s); \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_matrix_int(x, y, m, n, msg) \
    __s->assertion = ("assert_equals_matrix_int(" \
            #x", "#y", "#m", "#n", "#msg")"); \
    __assert_equals_matrix_int((m), (n), (x), (y), __s); \
    if (__s->failed) return;

/*
 * Check the assertion. Write the assertion expression in a string, for the 
 * output, check the condition and return if assertion failed.
 */
#define _assert_equals_matrix_flo(x, y, m, n, tol, msg) \
    __s->assertion = ("assert_equals_matrix_flo(" \
            #x", "#y", "#m", "#n", "#tol", "#msg")"); \
    __assert_equals_matrix_flo((m), (n), (x), (y), (tol), __s); \
    if (__s->failed) return;

/*
 * Cause the test case to fail.
 */
#define _fail(msg) \
    __s->assertion = "Reached a fail() statement: " #msg; \
    __s->failed = 1; \
    return;

/*
 * A type representing the status of the test execution. The status is
 * hiddenly passed to each assertion.
 */
typedef struct status
{
    const char *assertion; /* string containing the current assertion */
    int failed;            /* 0 if assert was ok, nonzero otherwise */
    const char *invalid;   /* NULL if assert was ok, non-NULL otherwise */
} Status;

/*
 * A type defining a test case inside a suit. It consists in a pointer to the
 * test case function, defined with the macro TEST_CASE(test_name), and a
 * string providing a human readable name for the test.
 */
typedef struct test_case 
{
    void (*fun)(Status*); /* pointer to test case funtion */
    char name[NAME_LEN];  /* human readable name for the test */
} Test_case;

/*
 * \brief This function actually implements floating point asserts
 * @param x First number to be compared
 * @param y Second number to be compared
 * @param tol Tolerance for numerical comparison
 * @param __s Status of current test case
 */
int __assert_equals_flo(double x, double y, double tol, Status *__s);

/*
 * \brief This function actually implements int array asserts
 * @param x First array to be compared
 * @param y Second array to be compared
 * @param len Array length
 * @param __s Status of current test case
 */
int __assert_equals_array_int(
        int *x,
        int *y,
        int len,
        Status *__s);

/*
 * \brief This function actually implements floating point array asserts
 * @param x First array to be compared
 * @param y Second array to be compared
 * @param len Array length
 * @param tol Tolerance for numerical comparison
 * @param __s Status of current test case
 */
int __assert_equals_array_flo(
        double *x,
        double *y,
        int len,
        double tol,
        Status *__s);

/*
 * \brief This function actually implements int matrix asserts
 * @param m Number of matrix rows
 * @param n Number of matrix columns
 * @param x First array to be compared
 * @param y Second array to be compared
 * @param tol Tolerance for numerical comparison
 * @param __s Status of current test case
 */
int __assert_equals_matrix_int(
        int m,
        int n,
        int x[m][n],
        int y[m][n],
        Status *__s);

/*
 * \brief This function actually implements floating point matrix asserts
 * @param m Number of matrix rows
 * @param n Number of matrix columns
 * @param x First array to be compared
 * @param y Second array to be compared
 * @param __s Status of current test case
 */
int __assert_equals_matrix_flo(
        int m,
        int n,
        double x[m][n],
        double y[m][n],
        double tol,
        Status *__s);
