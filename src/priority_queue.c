#include <stdio.h>
#include "priority_queue.h"

// Private heap functions
void _heapify_up(priority_queue *pq, int index);
void _heapify_down(priority_queue *pq, int index);

// Initialize the priority queue
void priority_queue_init(priority_queue *pq) {
    pq->size = 0;
}

// Check if the priority queue is empty
bool priority_queue_is_empty(priority_queue *pq) {
    return pq->size == 0;
}

// Check if the priority queue is full
bool priority_queue_is_full(priority_queue *pq) {
    return pq->size >= HEAP_SIZE;
}

// Add an element to the priority queue
enum priority_queue_err priority_queue_push(
    priority_queue *pq,
    int priority,
    PRIORITY_QUEUE_DATA_TYPE data
) {
    if (priority_queue_is_full(pq)) {
        return PQ_FULL;
    }

    // Add the new node to the end of the heap
    heap_node new_node = {priority, data};
    pq->heap[pq->size] = new_node;

    // Move the new node to its correct position
    _heapify_up(pq, pq->size);

    // Increment the size of the heap
    pq->size++;
    return PQ_SUCCESS;
}

// Remove and return the element with the highest priority (smallest priority value)
enum priority_queue_err priority_queue_pop(
    priority_queue *pq,
    heap_node *out_node
) {
    if (priority_queue_is_empty(pq)) {
        return PQ_EMPTY;
    }

    // Store the root of the heap in the output variable
    *out_node = pq->heap[0];

    // Replace the root with the last element in the heap
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;

    // Restore the heap property
    _heapify_down(pq, 0);

    return PQ_SUCCESS;
}

// Peek at the element with the highest priority without removing it
enum priority_queue_err priority_queue_peek(
    priority_queue *pq,
    heap_node *out_node
) {
    if (priority_queue_is_empty(pq)) {
        return PQ_EMPTY;
    }

    // Return the root of the heap
    *out_node = pq->heap[0];
    return PQ_SUCCESS;
}

// Restore heap property after adding a new element
void _heapify_up(priority_queue *pq, int index) {
    while (index > 0) {
        int parent_index = (index - 1) / 2;

        // If the current node's priority is less than its parent's, swap them
        if (pq->heap[index].priority < pq->heap[parent_index].priority) {
            heap_node temp = pq->heap[index];
            pq->heap[index] = pq->heap[parent_index];
            pq->heap[parent_index] = temp;

            // Move up the heap
            index = parent_index;
        } else {
            break;
        }
    }
}

// Restore heap property after removing the root
void _heapify_down(priority_queue *pq, int index) {
    while (true) {
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;
        int smallest = index;

        // Find the smallest among the current node and its children
        if (left_child < pq->size && pq->heap[left_child].priority < pq->heap[smallest].priority) {
            smallest = left_child;
        }
        if (right_child < pq->size && pq->heap[right_child].priority < pq->heap[smallest].priority) {
            smallest = right_child;
        }

        // If the smallest is not the current node, swap and continue
        if (smallest != index) {
            heap_node temp = pq->heap[index];
            pq->heap[index] = pq->heap[smallest];
            pq->heap[smallest] = temp;

            // Move down the heap
            index = smallest;
        } else {
            break;
        }
    }
}