#ifndef VLIB_SKIPLIST_H
#define VLIB_SKIPLIST_H
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>

namespace VLIB {

uint8_t MAX_LEVEL = 6;
const float P = 0.5;
using namespace std;

// NODE Class DECLARATION //
template <class T>
class snode {
public:
    T value;
    snode<T> **forw;
    snode(int lvl, const T &val) {
        value = val;
        forw = new snode<T>*[lvl + 1];
        memset(forw, 0, sizeof(snode<T>*) * (lvl + 1));
    }
    ~snode() {
        delete[] forw;
    }
};

// SKIPLIST CLASS DECLARATION //
template <class T>
class skipList
{
    public:
    snode<T> *header;
    T value;
    int level;
    skipList(uint8_t max_lvl) 
    {
        MAX_LEVEL = max_lvl;
        header = new snode<T>(MAX_LEVEL, value);
        level = 0;
    }
    ~skipList() 
    {
        clear();
        delete header;
    }
    void display();
    void displayStructure();
    bool contains(const T &); 
    snode<T>* search(const T &);
    void insert_element(const T &);
    void delete_element(const T &);       
    snode<T>* getHead() const; 
    void clear();

    // comparison operators
    bool operator==(const skipList<T>& other) const{return this->value == other.value;}
    bool operator!=(const skipList<T>& other) const{return this->value != other.value;}
    bool operator<(const skipList<T>& other) const{return this->value < other.value;}
    bool operator>(const skipList<T>& other) const{return this->value > other.value;}
    bool operator<=(const skipList<T>& other) const{return this->value <= other.value;}
    bool operator>=(const skipList<T>& other) const{return this->value >= other.value;}
    
};

// SKIPLIST CLASS IMPLEMENTATION // 

// random value generator function
float frand() 
{
    return (float) rand() / RAND_MAX;
}
 
// Random Level Generator
int random_level() 
{
    static bool first = true;
    if (first) 
    {
        srand((unsigned)time(NULL));
        first = false;
    }
    int lvl = (int)(log(frand()) / log(1.-P));
    return lvl < MAX_LEVEL ? lvl : MAX_LEVEL;
}
 
/*
* Insert Element in Skip List
*/
template <class T>
void skipList<T>::insert_element(const T &value) 
{
    snode<T> *x = header;	
    snode<T> *update[MAX_LEVEL + 1];
    memset(update, 0, sizeof(snode<T>*) * (MAX_LEVEL + 1));
    for (int i = level;i >= 0;i--) 
    {
        while (x->forw[i] != NULL && x->forw[i]->value < value) 
        {
            x = x->forw[i];
        }
        update[i] = x; 
    }
    x = x->forw[0];
    if (x == NULL || x->value != value) 
    {        
        int lvl = random_level();
        if (lvl > level) 
        {
            for (int i = level + 1;i <= lvl;i++) 
            {
                update[i] = header;
            }
            level = lvl;
        }
        x = new snode<T>(lvl, value);
        for (int i = 0;i <= lvl;i++) 
        {
            x->forw[i] = update[i]->forw[i];
            update[i]->forw[i] = x;
        }
    }
}
 
/*
 * Delete Element from Skip List
 */
template <class T>
void skipList<T>::delete_element(const T &value) 
{
    snode<T> *x = header;	
    snode<T> *update[MAX_LEVEL + 1];
    memset (update, 0, sizeof(snode<T>*) * (MAX_LEVEL + 1));
    for (int i = level;i >= 0;i--) 
    {
        while (x->forw[i] != NULL && x->forw[i]->value < value)
        {
            x = x->forw[i];
        }
        update[i] = x; 
    }
    x = x->forw[0];
    if (x->value == value) 
    {
        for (int i = 0;i <= level;i++) 
        {
            if (update[i]->forw[i] != x)
                break;
            update[i]->forw[i] = x->forw[i];
        }
        delete x;
        while (level > 0 && header->forw[level] == NULL) 
        {
            level--;
        }
    }
}
 
/*
 * Display Elements of Skip List
 */
template <class T>
void skipList<T>::display() 
{
    const snode<T> *x = header->forw[0];
    while (x != NULL) 
    {
        cout << x->value;
        x = x->forw[0];
        if (x != NULL)
            cout << " - ";
    }
    cout <<endl;
}
 
/*
 * Search Elemets in Skip List
 */
template <class T>
bool skipList<T>::contains(const T &s_value) 
{
    snode<T> *x = header;
    for (int i = level;i >= 0;i--) 
    {
        while (x->forw[i] != NULL && x->forw[i]->value < s_value)
        {
            x = x->forw[i];
        }
    }
    x = x->forw[0];
    return x != NULL && x->value == s_value;
}

template <class T>
snode<T>* skipList<T>::search(const T &s_value) 
{
    snode<T> *x = header;
    for (int i = level;i >= 0;i--) 
    {
        while (x->forw[i] != NULL && x->forw[i]->value < s_value)
        {
            x = x->forw[i];
        }
    }
    x = x->forw[0];
    return x;
}

/*
 * Display Elements of Skip List with Level
 */

template <class T>
void skipList<T>::displayStructure() 
{
    for (int i = 0;i <= level;i++) 
    {
        snode<T> *x = header->forw[i];
        cout << "Level " << i << ": ";
        while (x != NULL) 
        {
            cout << x->value;
            x = x->forw[i];
            if (x != NULL)
                cout << " - ";

            // is there numbers between the last number and the next number in lower levels? then print them as a dash

        }
        cout << endl;
    }
}


template <class T>
snode<T>* skipList<T>::getHead() const
{
    return header;
}

/*
 * Clear Elements of Skip List
 */

template <class T>
void skipList<T>::clear() 
{
    snode<T> *x = header->forw[0];
    while (x != NULL) 
    {
        snode<T> *next = x->forw[0];
        delete x;
        x = next;
    }
    memset(header->forw, 0, sizeof(snode<T>*) * (MAX_LEVEL + 1));
    level = 0;
}
}
#endif
