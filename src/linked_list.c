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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

static Node* node_alloc(Node* next, void *data)
{
    Node* new_node = (Node*) malloc(sizeof (Node));
    if (new_node == NULL)
    {
        perror("node_alloc: resource unavaible for malloc.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = next;
    return new_node;
}

/*!
 * This function initializes a list, allocating memory and setting the 
 * values to default.
 */
int ll_init(ll_list *l)
{
    l->root = NULL;
    l->size = 0;
    return 0;
}

/*!
 * This function destroys the list, deallocating memory.
 */
int ll_destroy(ll_list *l)
{
    if (l == NULL || l->size == 0)
        return 0;

    while (l->size > 0)
        ll_pop_front(l);
    free(l);
    return 0;
}

/*!
 * Adds a node in the front of the list (farthest from the root).
 */
int ll_push_front(ll_list *l, void *d)
{
    if (l->root == NULL) /* empty list */
    {
        l->root = node_alloc(NULL, d);
        l->size++;
        return 0;
    }

    /* list scroll */
    Node *n = l->root;
    while (n->next != NULL)
        n = n->next;

    n->next = node_alloc(NULL, d);
    l->size++;
    return 0;
}

/*!
 * Adds a node at the end of the list (in the root position).
 */
int ll_push_back(ll_list *l, void *d)
{
    Node* n = node_alloc(l->root, d);
    l->root = n;
    l->size++;
    return 0;
}

void* ll_pop_front(ll_list *l)
{
    if (l->root == NULL) /* empty list */
    {
        perror("ll_pop_front: trying to pop from empty list.\n");
        exit(EXIT_FAILURE);
    }
    
    if (l->root->next == NULL) /* one element list */
    {
        void *d = l->root->data;
        Node *n = l->root;
        l->root = NULL;
        l->size--;
        free(n);
        return d;
    }

    /* two or more element, scroll the list */
    Node *penultimate = l->root;
    Node *last = penultimate->next;
    while (last->next != NULL)
    {
        penultimate = last;
        last = last->next;
    }

    penultimate->next = NULL;
    void *d = last->data;
    free(last);
    l->size--;
    return d;
}

void* ll_pop_back(ll_list *l)
{
    if (l->root == NULL) /* empty list */
    {
        perror("ll_pop_front: trying to pop from empty list.\n");
        exit(EXIT_FAILURE);
    }

    Node *n = l->root;
    void *d = n->data;
    
    l->root = l->root->next; /* shift list head */
    free(n);
    l->size--;
    return d;
}

void* ll_get_pos(ll_list l, int pos)
{
    if (pos < 1 || (unsigned int) pos > l.size)
    {
        perror("ll_get_pos: index out of bound.\n");
        exit(EXIT_FAILURE);
    }
    
    int i;
    Node *n = l.root;
    for (i = 1; i < pos; ++i)
    {
        n = n->next;
    }
    return n->data;
}

void* ll_pop_pos(ll_list l, int pos)
{
    if (pos < 1 || (unsigned int) pos > l.size)
    {
        perror("ll_get_pos: index out of bound.\n");
        exit(EXIT_FAILURE);
    }
    
    int i;
    Node *n = l.root, *penultimate;
    void *res;
    for (i = 1; i < pos; ++i)
    {
        penultimate = n;
        n = n->next;
    }
    penultimate->next = n->next;
    res = n->data;
    free(n);
    return res;
}

ll_iterator ll_get_iterator(ll_list l)
{
    ll_iterator it = {l, l.root};
    return it;
}

void* ll_next(ll_iterator *it)
{
    void *res = it->next->data;
    if (it->next->next != NULL)
        it->next = it->next->next;
    return res;
}

int ll_copy(ll_list *to, ll_list from)
{
    ll_destroy(to);
    ll_init(to);

    ll_iterator it = ll_get_iterator(from);

    int i = 0;
    while ((unsigned int) i++ < from.size)
        ll_push_front(to, ll_next(&it));
    
    return 0;
}

int ll_append(ll_list *to, ll_list from)
{
    ll_iterator it = ll_get_iterator(from);

    int i = 0;
    while ((unsigned int) i++ < from.size)
        ll_push_front(to, ll_next(&it));
    
    return 0;
}
