#include "queue.h"

#include <stdlib.h>
#include <string.h>

#ifdef FSP_DEBUG
# include <stdio.h>
#endif

#define QUEUE_ALLOC_SIZE 50

/**
 * (INTERNAL FUNCTION)
 *
 * resets front-/back-pointer if too many spaces are unused and
 * shrinks the queue size if more than one chunk is unused.
 * 
 * @param    queue
 */
static inline void fsp_queue_rearrange(struct queue*);

/**
 * initializes the queue 
 * 
 * @param    queue
 */
void fsp_queue_init(struct queue *q)
{
  if(q == NULL) {
    return;
  }

  q->back = q->data = malloc(QUEUE_ALLOC_SIZE * sizeof(void*));

  if(q->data == NULL) {
#ifdef FSP_DEBUG
      printf("(-- ERROR: allocation failed: %s in %s at %d --)\n", __FILE__, __FUNCTION__, __LINE__);
#endif    
  }

  q->front = NULL;
  q->size = QUEUE_ALLOC_SIZE;

  pthread_mutex_init(&q->mtx, 0);
}

/**
 * free's and the queue.
 * 
 * be aware that the queue does NOT free the elements in 
 * itself since it doesn't know wether the element is allocated or not.
 * 
 * @param    queue
 */
void fsp_queue_free(struct queue *q)
{
  if(q == NULL) {
    return;
  }

  if(q->data != NULL) {
    free(q->data);
  }

  q->data = q->front = q->back = NULL;
  q->size = 0;

  pthread_mutex_destroy(&q->mtx);
}

/**
 * appends the element to the back of the queue. 
 *
 * queue may perform some optimizations.
 * 
 * @param    queue
 * @param    element
 */
void fsp_queue_push(struct queue *q, void *e)
{
  if(q == NULL || e == NULL) {
    return;
  }

  pthread_mutex_lock(&q->mtx);

  fsp_queue_rearrange(q);

#ifdef FSP_DEBUG
  printf("(push at pos %ld)\n", (q->back - q->data) + 1);
#endif

  if(q->back == &q->data[q->size - 1]) { // space for 1 element left, reallocating for more space
#ifdef FSP_DEBUG    
    printf("(reallocating for %d new elements)\n", QUEUE_ALLOC_SIZE);
#endif

    size_t pos_front = q->front - q->data;
    
    q->data = realloc(q->data, (q->size + QUEUE_ALLOC_SIZE) * sizeof(void*));

    if(q->data == NULL)
    {
#ifdef FSP_DEBUG
      printf("(-- ERROR: allocation failed: %s in %s at %d --)\n", __FILE__, __FUNCTION__, __LINE__);
#endif
    }
    
    if(q->front != NULL)
      q->front = &q->data[pos_front];

    q->back = &q->data[q->size - 1];
    q->size += QUEUE_ALLOC_SIZE;
  }

  if(q->front == NULL)
    q->front = q->back;

  *q->back++ = e;

  pthread_mutex_unlock(&q->mtx);
}

/**
 * returns the front element if there is one, otherwise NULL'll be returned.
 * 
 * @param    queue
 * @return   element
 */
void *fsp_queue_pop(struct queue *q)
{
  if(q == NULL) {
    return NULL;
  }

  pthread_mutex_lock(&q->mtx);

  if(q->front == NULL) {
    pthread_mutex_unlock(&q->mtx);
    return NULL;
  }

  void *e = *q->front;

  if(++q->front == q->back)
    q->front = NULL;

  pthread_mutex_unlock(&q->mtx);

  return e;
}

/**
 * returns confirmation if queue is empty
 * 
 * @param    queue
 * @return   true if empty or false if not
 */
bool fsp_queue_empty(struct queue *q)
{
  if(q == NULL || q->front == NULL) {
    return true;
  }

  return fsp_queue_size(q) == 0;
}

/**
 * returns the element count
 * 
 * @param    queue
 * @return   element count
 */
size_t fsp_queue_size(struct queue *q)
{
  if(q == NULL) {
    return 0;
  }

  return (q->front == NULL ? 0 : (q->back - q->data) - (q->front - q->data));
}

/**
 * (INTERNAL FUNCTION)
 *
 * resets front-/back-pointer if too many spaces are unused and
 * shrinks the queue size if more than one chunk is unused.
 * 
 * @param    queue
 */
static inline void fsp_queue_rearrange(struct queue *q)
{
  if(q == NULL) {
    return;
  }

  if(q->front != q->data) { // unused memory

    if(q->front == NULL) {
      
      q->back = q->data;

    } else if(q->front - q->data >= (QUEUE_ALLOC_SIZE - 1)) { // TODO: use more suited value 
#ifdef FSP_DEBUG      
      printf("(unused spaces: %ld, elements in queue: %ld)\n", q->front - q->data, (q->back - q->front));
#endif

      size_t elements = (q->back - q->front);
      memcpy(q->data, q->front, elements);

      q->back = &q->data[elements];
      q->front = q->data;
    }
  }

  if((q->size - (q->back - q->data)) > (QUEUE_ALLOC_SIZE + QUEUE_ALLOC_SIZE / 2)) {
    size_t elements = q->back - q->data;
    size_t chunks = (q->size - (elements + QUEUE_ALLOC_SIZE / 2)) / QUEUE_ALLOC_SIZE;

#ifdef FSP_DEBUG
    printf("(shrinking queue by %ld chunks, old size: %ld, new size: %ld)\n", chunks, q->size, (q->size - (chunks * QUEUE_ALLOC_SIZE)));
#endif

    q->data = realloc(q->data, (q->size - (chunks * QUEUE_ALLOC_SIZE)) * sizeof(void*));
    q->size = (q->size - (chunks * QUEUE_ALLOC_SIZE));

    if(q->data == NULL) {
#ifdef FSP_DEBUG
      printf("(-- ERROR: allocation failed: %s in %s at %d --)\n", __FILE__, __FUNCTION__, __LINE__);
#endif
    }

    if(elements > 0) {
      q->back = &q->data[elements - 1];
    } else {
      q->back = q->data;
    }

    if(q->front != NULL) {
      q->front = q->data;
    }
  }
}
