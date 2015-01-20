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
 * \file cutest.h
 *
 * @author Martino Pilia
 * @date 2015-01-13
 */

#include <math.h>
#include <string.h>
#include "linked_list.h"
#include "cutest_private.h"

/*!
 * \brief Declaration of a test case.
 * @param test_name Name for the test case
 *
 * A test case is declared with this macro, as:
 * \code
 * TEST_CASE(test_name)
 * {
 *     // test code
 * }
 * \endcode
 *
 * The test code may contain definition, statements and assertions.
 * Statements in the test case are executed sequentially until control
 * flow reaches the end of the code, an error rises or an assertion fails. 
 * If the code causes an error (e.g. segmentation fault) the test execution
 * is interrupted and the test is counted as failed with error.
 * If an assertion fails, the execution of the test case is interrupted 
 * and the test is considered as failed.
 */
#define TEST_CASE(name) _TEST_CASE((name))

/*!
 * \brief Define a procedure to be executed before each test case.
 *
 * \code
 * BEFORE_TEST(name)
 * {
 *     // procedure code
 * }
 * \endcode
 *
 * This macro permits to define a procedure which is executed before each 
 * test case, allowing to write once eventual initialization staments needed
 * by all the test cases. This must be done outside other subroutines.
 *
 * @param name Name for the procedure
 */
#define BEFORE_TEST(name) _BEFORE_TEST((name))

/*!
 * \brief Define a procedure to be executed after each test case.
 *
 * \code
 * AFTER_TEST(name)
 * {
 *     // procedure code
 * }
 * \endcode
 *
 * This macro permits to define a procedure which is executed after each 
 * test case, allowing to write once the cleanup statements needed after 
 * each test case execution. 
 *
 * The execution of the AFTER_TEST(name) is guaranted even if the test case
 * execution terminates with an error or after a signal.
 *
 * @param name Name for the procedure
 */
#define AFTER_TEST(name) _AFTER_TEST((name))

/*!
 * \brief Assert wether a condition is true.
 * @param expr Logical (integer) expression to be tested.
 * @param msg Human readable message shown when assertion fails.
 *
 * This macro works only inside a TEST_CASE(test_name) definition. It permits
 * to test the truth of a condition: if the condition,
 * represented by the logical (integer) expression <code>expr</code>, is true
 * then the execution of the containing test case go on to the next statement,
 * otherwise the test case execution is interrupted and the corresponding 
 * <code>msg</code> message is displayed.
 */
#define assert(expr, msg) _assert(expr, msg)

/*!
 * \brief Assert a logical (integer) expression is false.
 *
 * This assertion checks if a logical expression is false, and it fails
 * if the expression evaluation result is true (nonzero).
 *
 * @param expr Expression to be evaluated 
 * @param msg Human readable message, showed when assert fails
 */
#define assert_false(expr, msg) _assert_false(expr, msg)

/*!
 * \brief Assert two integers are equal.
 *
 * @param x First integer to be compared
 * @param y Second integer to be compared
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_int(x, y, msg) _assert_equals_int(x, y, msg)

/*!
 * \brief Assert two floating point numbers are equal.
 *
 * @param x First floating point number to be compared
 * @param y Second floating point number to be compared
 * @param tol Tolerance for numerical comparison
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_flo(x, y, tol, msg) _assert_equals_flo(x, y, tol, msg)

/*!
 * \brief Assert two strings are equal.
 *
 * @param x First string to be compared
 * @param y Second string to be compared
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_str(x, y, msg) _assert_equals_str(x, y, msg)

/*!
 * \brief Assert a pointer is NULL.
 *
 * @param x Pointer to be NULL checked
 * @param msg Human readable message, showed when assert fails
 */
#define assert_null(x, msg) _assert_null(x, msg)

/*!
 * \brief Assert a pointer is not NULL.
 *
 * @param x Pointer to be NULL checked
 * @param msg Human readable message, showed when assert fails
 */
#define assert_not_null(x, msg) _assert_not_null(x, msg)

/*!
 * \brief Assert two arrays have the same integer entries.
 *
 * @param x First array to be compared
 * @param y Second array to be comprared
 * @param len Array length
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_array_int(x, y, len, msg) \
    _assert_equals_array_int(x, y, len, msg) 

/*!
 * \brief Assert two arrays have the same floating point entries.
 * 
 * @param x First array to be compared
 * @param y Second array to be comprared
 * @param len Array length
 * @param tol Tolerance for numerical comparison
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_array_flo(x, y, len, tol, msg) \
    _assert_equals_array_flo(x, y, len, tol, msg)

/*!
 * \brief Assert two matrixes have the same integer entries.
 * 
 * @param x First matrix to be compared
 * @param y Second matrix to be comprared
 * @param m Number of rows in the matrix
 * @param n Number of columns in the matrix
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_matrix_int(x, y, m, n, msg) \
    _assert_equals_matrix_int(x, y, m, n, msg)

/*!
 * \brief Assert two matrixes have the same floating point entries.
 * 
 * @param x First matrix to be compared
 * @param y Second matrix to be comprared
 * @param m Number of rows in the matrix
 * @param n Number of columns in the matrix
 * @param tol Tolerance for numerical comparison
 * @param msg Human readable message, showed when assert fails
 */
#define assert_equals_matrix_flo(x, y, m, n, tol, msg) \
    _assert_equals_matrix_flo(x, y, m, n, tol, msg)

/*!
 * \brief Cause the immediate failure of the current test case.
 * 
 * @param msg Human readable message, showed when the statement is reached
 */
#define fail(msg) _fail(msg)

/*!
 * \brief Type for a suite of test cases.
 *
 * This type defines a container filled with test cases. A suite has a name and
 * a set of test cases, memorized as a linked list of pointers to function.
 *
 * A unit test is an executable program which runs one one or more suites, 
 * each containing one or more test cases.
 * To create a suite, declare a pointer to Suite and 
 * initialize it with the 
 * suite_new(Suite**, const char*, void (*)(void), void (*)(void)) 
 * function (before and after procedures must be defined before, with the
 * BEFORE_TEST(name) and AFTER_TEST(name) macros).
 *
 * \code
 * Suite *s;
 * suite_new(&s, "Suite name", before_proc, after_proc);
 * \endcode
 *
 * then fill the suite with previous definited test cases. Test cases 
 * must be defined before, with the TEST_CASE(name) macro.
 *
 * \code
 * suite_add(s, test_name1, "Test name 1");
 * suite_add(s, test_name2, "Test name 2");
 * \endcode
 *
 * When all desired tests have been added to the suite, its execution
 * may be started with the function suite_run(Suite *s)
 *
 * \code
 * suite_run(s);
 * \endcode
 */
typedef struct suite
{
    char name[NAME_LEN]; /*!< Human readable name for the suite */
    ll_list asserts; /*!< Linked list containing pointers to test cases */
    void (*before)(void); /*!< Name of eventual BEFORE_TEST(name) procedure */
    void (*after)(void);   /*!< Name of eventual AFTER_TEST(name) procedure */
} Suite;

/*!
 * \brief Initialize a new suite.
 *
 * @param s Suite to be initialized
 * @param name Human readable name for the suite
 * @param bef Name of the BEFORE_TEST(name) procedure (set to NULL if none)
 * @param aft Name of the AFTER_TEST(name) procedure (set to NULL if none)
 */
int suite_new(
        Suite **s,
        const char *name,
        void (*bef)(void),
        void (*aft)(void)
        );

/*!
 * \brief Add a test case to a suite
 *
 * @param s Suite
 * @param t Name of a previously definited test case
 * @param name String with a descriptive name for the test case
 */
int suite_add(Suite *s, void (*t)(Status*), const char *name);

/*!
 * \brief Run a suite of test cases.
 *
 * @param s Suite to be executed
 */
int suite_run(Suite *s);
