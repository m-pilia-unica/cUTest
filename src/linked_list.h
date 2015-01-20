/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Martino Pilia, 2014
 */

/*!
 * \file linked_list.h
 * \brief Linked list
 *
 * @author Martino Pilia
 * @date 2014-12-17
 */

#define LIST_INITIALIZER {NULL, 0} /*!< Value for list initialization */

/*!
 * \brief Type for the node of the list.
 *
 * This type defines a node of the list, containing a pointer to the next
 * node and the data for the current element.
 */
typedef struct node
{
    struct node *next; /*!< pointer to the next node */
    void *data; /*!< data contained in the current node */
} Node;

/*!
 * \brief Type for the list.
 *
 * This type defines a list, containing a pointer to its root node and 
 * its size.
 */
typedef struct
{
    Node* root; /*!< pointer to the root node of the list */
    unsigned int size; /*!< current list size */
} ll_list;

/*!
 * \brief Type for the iterator.
 *
 * This type defines an iterator. An iterator is an object used to read
 * the list from the root to the last node (front). To read the list,
 * create an iterator and then read the nodes with the ll_next() function.
 */
typedef struct
{
    ll_list l; /*!< list that the iterator is referring */
    Node *next; /*!< next node to be read from the list */
} ll_iterator;

/*!
 * \brief Initialize a list.
 *
 * @param l list to be initialized
 */
int ll_init(ll_list *l);

/*!
 * \brief Destroy a list
 *
 * @param l list to be destroyed
 */
int ll_destroy(ll_list *l);

/*!
 * \brief Add a node at the list beginning.
 *
 * @param l list to add the new node to
 * @param d data to be contained in the new node
 */ 
int ll_push_front(ll_list *l, void *d);

/*!
 * \brief Add a node at the list end.
 *
 * @param l list to add the new node to
 * @param d data to be contained in the new node
 */ 
int ll_push_back(ll_list *l, void *d);

/*!
 * \brief Remove a node from the list beginning.
 *
 * @param l list to remove the node from
 */
void* ll_pop_front(ll_list *l);

/*!
 * \brief Remove a node from the list end.
 *
 * @param l list to remove the node from
 */
void* ll_pop_back(ll_list *l);

/*!
 * \brief Get an element in a specific position in the list
 *
 * @param l list to get the element from
 * @param pos one-based position of the desired element (starting from root)
 */
void* ll_get_pos(ll_list l, int pos);

/*!
 * \brief Remove an element from a specific position in the list
 *
 * @param l list to get the element from
 * @param pos one-based position of the desired element (starting from root)
 */
void* ll_pop_pos(ll_list l, int pos);

/*!
 * \brief Get an iterator for the specified list
 *
 * @param l list to be iterated
 */
ll_iterator ll_get_iterator(ll_list l);

/*!
 * \brief Get the next element from an iterator
 *
 * @param it iterator for the desired list
 */
void* ll_next(ll_iterator *it);

/*!
 * \brief Copy one list into another
 *
 * @param to list to be filled
 * @param from list to be copied
 */
int ll_copy(ll_list *to, ll_list from);

/*!
 * \brief Add all elements contained in one list to another
 *
 * @param to list to be filled
 * @param from list to be copied
 */
int ll_append(ll_list *to, ll_list from);
