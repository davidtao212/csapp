#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int val;
  struct Node* left;
  struct Node* right;
} Node;

// Binary search tree, root = 0x6030f0
int func7(Node* root, int a) {
  if (!root) {
    return -1;
  }

  int t = 0;
  if (a < root->val) {
    t = func7(root->left, a);
    return 2 * t;
  } else {
    if (root->val == a) {
      return 0;
    }
    t = func7(root->right, a);
    return 2 * t + 1;
  }
}

Node* newNode(int v) {
  Node *n = malloc(sizeof(Node));
  n->val = v;
  return n;
}

Node* newTree(int* a, int n) {
  if (!a || n <= 0) return NULL;
  Node* queue[n];
  Node* root = newNode(a[0]);
  int i = 0, j = 0;
  queue[j++] = root;
  while (i < j) {
    Node* curr = queue[i++];
    if (j < n) {
      Node* left = newNode(a[j]);
      queue[j++] = left;
      curr->left = left;
    }
    if (j < n) {
      Node* right = newNode(a[j]);
      queue[j++] = right;
      curr->right = right;
    }
  }
  return root;
}

void destroyTree(Node* root) {
  if (!root) return;
  destroyTree(root->left);
  destroyTree(root->right);
  free(root);
}

void printTree(Node* root) {
  if (!root) return;
  printTree(root->left);
  printf("%d, ", root->val);
  printTree(root->right);
}

#define N 15

int main() {
  int a[N] = {36, 8, 50, 6, 22, 45, 107, 1, 7, 20, 35, 40, 47, 99, 1001};
  Node* root = newTree(a, N);
  for (int i = 0; i < N; i++) {
    int ret = func7(root, a[i]);
    printf("%d -> %d\n", ret, a[i]);
  }
  destroyTree(root);
}