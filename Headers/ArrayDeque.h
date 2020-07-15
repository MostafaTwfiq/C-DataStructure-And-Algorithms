#ifndef C_DATASTRUCTURES_ARRAYDEQUE_H
#define C_DATASTRUCTURES_ARRAYDEQUE_H
#include "Utils.h"


typedef struct ArrayDequeItem {
    void *value;
    int sizeOfItem;
} ArrayDequeItem;



/** @struct ArrayDeque
 *  @brief This structure implements a basic generic Deque.
 *  @var ArrayDeque::arr
 *  Member 'memory' is a pointer the deque's allocated array.
 *  @var ArrayDeque::front
 *  Member 'front' index of the front from the end index of the deque array.
  *  @var ArrayDeque::rear
 *  Member 'rear' index of the rear from the start index of the deque array.
 *  @var ArrayDeque::size
 *  Member 'size' holds the current size of the allocated deque array,ie the number of allocated items.
 */
typedef struct ArrayDeque{
    void** memory;
    uint32_t  front;
    uint32_t  rear;
    uint32_t  size;
    uint32_t allocated;
    void(*freeFn)(ArrayDequeItem*);
}ArrayDeque;

ArrayDeque*         ArrayDequeInitialize(uint32_t initialSize);
void                ArrayDequeInsertFront(ArrayDeque* arrayDeque,void* key);
void                ArrayDequeInsertRear(ArrayDeque* arrayDeque,void* key);
void                ArrayDequeDeleteFront(ArrayDeque* arrayDeque);
void                ArrayDequeDeleteRear(ArrayDeque* arrayDeque);
uint16_t            ArrayDequeIsEmpty(ArrayDeque* arrayDeque);
void*               ArrayDequeGetFront(ArrayDeque* arrayDeque);
void*               ArrayDequeGetRear(ArrayDeque* arrayDeque);
void                ArrayDequePrint(ArrayDeque* arrayDeque, void (*printFn)(void *));
ArrayDequeItem*     ArrayDequeNewItem(int sizeOfType,void *key);

#endif //C_DATASTRUCTURES_ARRAYDEQUE_H