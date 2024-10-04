#include "SMMIterator.h"
#include "SortedMultiMap.h"

SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
    currentElement = map.head;
    if (currentElement != nullptr) {
         currentVelement = currentElement->vHead;  // Reset to first value node
     } else {
         currentVelement = nullptr;  // If no elements, value pointer is null
     }
}

void SMMIterator::first(){
    currentElement = map.head;
    if (currentElement != nullptr) {
         currentVelement = currentElement->vHead;  // Reset to first value node
     } else {
         currentVelement = nullptr;  // If no elements, value pointer is null
     }
}

// !!!
// !!!
// !!!
void SMMIterator::next() {
    if (!valid()) {
        throw exception();  // Throwing exception if the iterator is invalid
    }

    // Move to the next value in the current key's value list  !!!bookmark
    //_-------------____---___
    if (currentVelement != nullptr && currentVelement->next != nullptr) {
        currentVelement = currentVelement->next;
    } else {
        // If we've reached the end of the current key's value list, move to the next key
        currentElement = currentElement->next;
        if (currentElement != nullptr) {
            currentVelement = currentElement->vHead;  // Reset to the first value of the next key
        } else {
            currentVelement = nullptr;  // No more elements left
        }
    }
}
// !!!
// !!!
// !!!


bool SMMIterator::valid() const{
    return currentElement != nullptr && currentVelement != nullptr;
}

TElem SMMIterator::getCurrent() const{
    if (!valid()) {
        throw exception();
    }
    return std::make_pair(currentElement->key, currentVelement->val);
}

