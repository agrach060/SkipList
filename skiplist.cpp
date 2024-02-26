#include "skiplist.h"
#include "random.h"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

using namespace std;

// checks if the value needs to be inserted at a higher level
bool SkipList::shouldInsertAtHigherLevel() const {
  return probability >= Random::random() % 100;
}

// you code goes here

// SNode constructor
SNode::SNode(int val) {
  this->val = val;
  this->next.resize(1, nullptr);
}

// SNode copy constructor
SNode::SNode(const SNode &other)
    : val(other.val), next(other.next.size(), nullptr) {}

// SkipList constructor
SkipList::SkipList(int levels, int probability)
    : levels(levels), probability(probability) {
  head = new SNode();
  head->next.resize(levels, nullptr);
}

// SkipList copy constructor
SkipList::SkipList(const SkipList &other)
    : levels(other.levels), probability(other.probability) {
  head = new SNode(*other.head); // Copy the head node

  // Copy the elements from the original SkipList
  for (int i = 0; i < levels; ++i) {
    SNode *current = other.head->next[i];
    SNode *prev = head;
    while (current != nullptr) {
      SNode *newNode = new SNode(*current); // Create a copy of the node
      prev->next[i] = newNode;
      prev = newNode;
      current = current->next[i];
    }
    prev->next[i] = nullptr; // Set the next pointer of the last node to nullptr

    // Deallocate unused nodes at this level
    while (prev->next.size() > levels) {
      SNode *nextNode = prev->next.back();
      prev->next.pop_back();
      delete nextNode;
    }
  }
}

// SkipList operator<<
ostream &operator<<(ostream &out, const SkipList &skip) {
  for (int i = skip.levels - 1; i >= 0; i--) {
    out << "[level: " << i + 1 << "] ";
    SNode *curr = skip.head->next[i];
    while (curr != nullptr) {
      out << curr->val << "-->";
      curr = curr->next[i];
    }
    out << "nullptr\n";
  }
  return out;
}

SkipList::~SkipList() {
  while (head != nullptr) {
    SNode *curr = head;
    head = head->next[0];
    delete curr;
  }
  delete head;
}

// collect all SNode* objects that come before this value at each level
// used as a helper function to add and remove
vector<SNode *> SkipList::getBeforeNodes(int val) const {
  vector<SNode *> beforeNodes(levels, nullptr);
  SNode *curr = head;
  for (int i = levels - 1; i >= 0; i--) {
    while (curr->next[i] != nullptr && curr->next[i]->val < val) {
      curr = curr->next[i];
    }
    beforeNodes[i] = curr;
  }

  return beforeNodes;
}

// Add to list, assume no duplicates
void SkipList::add(int val) {
  vector<SNode *> beforeNodes = getBeforeNodes(val);
  SNode *newNode = new SNode(val);
  newNode->next[0] = beforeNodes[0]->next[0];
  beforeNodes[0]->next[0] = newNode;
  for (int i = 1; i < levels; i++) {
    if (i < levels && shouldInsertAtHigherLevel()) {
      newNode->next.push_back(beforeNodes[i]->next[i]);
      beforeNodes[i]->next[i] = newNode;
    } else {
      break;
    }
  }
}

// Add a vector of values to list, assume no duplicates
void SkipList::add(const vector<int> &values) {
  for (int i = 0; i < values.size(); i++) {
    add(values[i]);
  }
}

bool SkipList::remove(int val) {
  vector<SNode *> beforeNodes = getBeforeNodes(val);
  SNode *nodeToRemove = beforeNodes[0]->next[0];

  // check if the node to remove exists
  if (nodeToRemove == nullptr || nodeToRemove->val != val) {
    return false; // value not found
  }

  // update links to remove node at all levels
  for (int i = 0; i < levels; i++) {
    if (beforeNodes[i]->next[i] != nodeToRemove) {
      break;
    }
    beforeNodes[i]->next[i] = nodeToRemove->next[i];
  }

  // remove any unused levels
  while (levels > 1 && head->next[levels - 1] == nullptr) {
    head->next.pop_back();
    levels--;
  }

  // delete the node
  delete nodeToRemove;

  return true; // value successfully removed
}

bool SkipList::contains(int val) const {
  SNode *node = head;
  for (int i = levels - 1; i >= 0; i--) {
    while (node->next[i] != nullptr && node->next[i]->val < val) {
      node = node->next[i];
    }
  }
  node = node->next[0];
  return (node != nullptr && node->val == val);
}