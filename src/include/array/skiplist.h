//source:
//https://gist.github.com/unsuthee/4116663

///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "globalFunctions.h"
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

template<class keyType, class t, int MAXLEVEL>
class skiplist_node
{
public:
    skiplist_node()
    {
        for (int i = 1; i <= MAXLEVEL; i++) {
            forwards[i] = nullptr;
        }
    }

    skiplist_node(const keyType& searchKey) :key(searchKey)
    {
        for (int i = 1; i <= MAXLEVEL; i++) {
            forwards[i] = nullptr;
        }
    }

    skiplist_node(const keyType& searchKey, const t& value) :key(searchKey), value(value)
    {
        for (int i = 1; i <= MAXLEVEL; i++) {
            forwards[i] = nullptr;
        }
    }

    virtual ~skiplist_node()
    {
    }

    keyType key;
    t value;
    skiplist_node<keyType, t, MAXLEVEL>* forwards[MAXLEVEL + 1];
};

///////////////////////////////////////////////////////////////////////////////

template<class keyType, class t, int MAXLEVEL = 16>
class skiplist
{
public:
    typedef keyType KeyType;
    typedef t ValueType;
    typedef skiplist_node<keyType, t, MAXLEVEL> NodeType;

    skiplist(keyType minKey, keyType maxKey) :m_pHeader(nullptr), m_pTail(nullptr),
        max_curr_level(1), max_level(MAXLEVEL),
        m_minKey(minKey), m_maxKey(maxKey)
    {
        m_pHeader = new NodeType(m_minKey);
        m_pTail = new NodeType(m_maxKey);
        for (int i = 1; i <= MAXLEVEL; i++) {
            m_pHeader->forwards[i] = m_pTail;
        }
    }

    virtual ~skiplist()
    {
        NodeType* currNode = m_pHeader->forwards[1];
        while (currNode != m_pTail) {
            NodeType* tempNode = currNode;
            currNode = currNode->forwards[1];
            delete tempNode;
        }
        delete m_pHeader;
        delete m_pTail;
    }

    void insert(keyType searchKey, t newValue)
    {
        skiplist_node<keyType, t, MAXLEVEL>* update[MAXLEVEL];
        NodeType* currNode = m_pHeader;
        for (int level = max_curr_level; level >= 1; level--) {
            while (currNode->forwards[level]->key < searchKey) {
                currNode = currNode->forwards[level];
            }
            update[level] = currNode;
        }
        currNode = currNode->forwards[1];
        if (currNode->key == searchKey) {
            currNode->value = newValue;
        }
        else
{
            int newlevel = randomLevel();
            if (newlevel > max_curr_level) {
                for (int level = max_curr_level + 1; level <= newlevel; level++) {
                    update[level] = m_pHeader;
                }
                max_curr_level = newlevel;
            }
            currNode = new NodeType(searchKey, newValue);
            for (int lv = 1; lv <= max_curr_level; lv++) {
                currNode->forwards[lv] = update[lv]->forwards[lv];
                update[lv]->forwards[lv] = currNode;
            }
        }
    }

    void erase(keyType searchKey)
    {
        skiplist_node<keyType, t, MAXLEVEL>* update[MAXLEVEL];
        NodeType* currNode = m_pHeader;
        for (int level = max_curr_level; level >= 1; level--) {
            while (currNode->forwards[level]->key < searchKey) {
                currNode = currNode->forwards[level];
            }
            update[level] = currNode;
        }
        currNode = currNode->forwards[1];
        if (currNode->key == searchKey) {
            for (int lv = 1; lv <= max_curr_level; lv++) {
                if (update[lv]->forwards[lv] != currNode) {
                    break;
                }
                update[lv]->forwards[lv] = currNode->forwards[lv];
            }
            delete currNode;
            // update the max level
            while (max_curr_level > 1 && m_pHeader->forwards[max_curr_level] == nullptr) {
                max_curr_level--;
            }
        }
    }

    const NodeType* find(keyType searchKey)
    {
        NodeType* currNode = m_pHeader;
        for (int level = max_curr_level; level >= 1; level--) {
            while (currNode->forwards[level]->key < searchKey) {
                currNode = currNode->forwards[level];
            }
        }
        currNode = currNode->forwards[1];
        if (currNode->key == searchKey) {
            return currNode;
        }
        else
{
            return nullptr;
        }
    }

    bool empty() const
    {
        return (m_pHeader->forwards[1] == m_pTail);
    }

    std::string printList()
    {
        std::stringstream sstr;
        NodeType* currNode = m_pHeader->forwards[1];
        while (currNode != m_pTail) {
            sstr << "(" << currNode->key << "," << currNode->value << ")" << std::endl;
            currNode = currNode->forwards[1];
        }
        return sstr.str();
    }

    const int max_level;

protected:
    int randomLevel() {
        int level = 1;
        while (rand(2) && level < MAXLEVEL) {
            level++;
        }
        return level;
    }
    keyType m_minKey;
    keyType m_maxKey;
    int max_curr_level;
    skiplist_node<keyType, t, MAXLEVEL>* m_pHeader;
    skiplist_node<keyType, t, MAXLEVEL>* m_pTail;
};