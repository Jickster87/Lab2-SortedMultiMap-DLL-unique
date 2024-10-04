    #include "SMMIterator.h"
    #include "SortedMultiMap.h"
    #include <iostream>
    #include <vector>
    #include <exception>
    using namespace std;

    SortedMultiMap::SortedMultiMap(Relation r) {
        rel = r;
        head = nullptr;
        tail = nullptr;
        
        totalSize = 0;
    }

    void SortedMultiMap::add(TKey c, TValue v) {
        mapNode * nomad = head;
        //point mapNodes to correct location according to rel
        //and if key is the same, push value in DLL
        while (nomad != nullptr && rel(nomad->key, c)) {
            if (nomad->key == c) {
                valNode * newVnode = new valNode(v);
                newVnode->prev = nomad->vTail;
                nomad->vTail->next = newVnode;
                nomad->vTail = newVnode;

                totalSize++;
                return;
            }
            nomad = nomad->next;
        }
        //key doesn't exist
        valNode * newValNode = new valNode(v);
        mapNode * newMapNode = new mapNode(c);
        newMapNode->vHead = newMapNode->vTail = newValNode;
        
        //insert the new node
        //0 is the list empty
        if (head == nullptr) {
            head = tail = newMapNode;
            totalSize++;
            return;
        }
        //1 head
        if (nomad == head) {
            newMapNode->next = head;
            head->prev = newMapNode;
            head = newMapNode;
        }
        //2 tail
        else if (nomad == nullptr){
            newMapNode->prev = tail;
            tail->next = newMapNode;
            tail = newMapNode;
        }
        //3 in-between
        else {
            newMapNode->next = nomad;
            newMapNode->prev = nomad->prev;
            if (nomad->prev != nullptr) {
                nomad->prev->next = newMapNode;
            }
            nomad->prev = newMapNode;
            if (nomad == head) {
                head = newMapNode;
            }
        }
        totalSize++;
    }

    bool SortedMultiMap::remove(TKey c, TValue v) {
        mapNode * nomad = head;
        //move to correct node
        while (nomad != nullptr && nomad->key != c) {
            nomad = nomad->next;
        }
        //key not in map
        if (nomad == nullptr) {
            return false;
        }
        
        valNode * valNomad = nomad->vHead;
        //key found, remove value
        //check if value in key
        while (valNomad != nullptr && valNomad->val != v) {
            valNomad = valNomad->next;
        }
        //valNomad cases
        //valNomad not found
        if (valNomad == nullptr) {
            return false;
        }
        //remove val head
        if (valNomad == nomad->vHead) {
            nomad->vHead = valNomad->next;
            if (nomad->vHead != nullptr) {
                nomad->vHead->prev = nullptr;
            } else {
                nomad->vTail = nullptr;
            }
        }
        //remove val tail
        else if (valNomad == nomad->vTail){
            nomad->vTail = valNomad->prev;
            if (nomad->vTail != nullptr) {
                nomad->vTail->next = nullptr;
            }
        } else {
            valNomad->next->prev = valNomad->prev;
            valNomad->prev->next = valNomad->next;
        }
        
        //remove val in between
        delete valNomad;
        totalSize--;
        
        //check if it was the last value to remove key too
        if (nomad->vHead == nullptr) {
            //remove head
            if (nomad == head) {
                     head = nomad->next;
                     if (head != nullptr) {
                         head->prev = nullptr;
                     } else {
                         tail = nullptr;  // List is now empty
                     }
            }
            // Case 2: Removing tail of mapNode list
            else if (nomad == tail) {
                tail = nomad->prev;
                if (tail != nullptr) {
                    tail->next = nullptr;
                }
            }
            // Case 3: Removing mapNode in between
            else {
                nomad->prev->next = nomad->next;
                nomad->next->prev = nomad->prev;
            }
            delete nomad;
        }
        return true;
    }

    vector<TValue> SortedMultiMap::search(TKey c) const {
        mapNode * nomad = head;
        while (nomad != nullptr) {
            if (nomad->key == c) {
                vector<TValue> searchValues;
                valNode * valNomad = nomad->vHead;
                while (valNomad != nullptr) {
                    searchValues.push_back(valNomad->val);
                    valNomad = valNomad->next;
                }
                return searchValues;
            }
            nomad = nomad->next;
        }
        return vector<TValue>();
    }

    int SortedMultiMap::size() const {
        return totalSize;
    }

    bool SortedMultiMap::isEmpty() const {
        return totalSize == 0;
    }

    SMMIterator SortedMultiMap::iterator() const {
        return SMMIterator(*this);
    }

    SortedMultiMap::~SortedMultiMap() {
        while (head != nullptr) {
            valNode * vTemp = head->vHead;
            while (vTemp != nullptr) {
                valNode * vDelete = vTemp;
                vTemp = vTemp->next;
                delete vDelete;
            }
            mapNode * temp = head;
            head = head->next;
            delete temp;
        }
    }
