//Name : Laxmi Magadum
//USN : 4SU19CS045
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define println(...)                                                           \
  printf(__VA_ARGS__);                                                         \
  printf("\n");
// Define a struct for a file or directory
typedef struct {
  char *name;
  int is_dir;
} NodeData;

// Define a node in the binary search tree
typedef struct Node {
  NodeData *data;
  struct Node *left;
  struct Node *right;
} Node;

// Define a binary search tree
typedef struct {
  Node *root;
} BST;

BST bst = {.root = NULL};
// Create a new node with the given data
Node *create_node(NodeData *data) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->data = (NodeData*)malloc(sizeof(NodeData));
  node->data->name = (char*)malloc(strlen(data->name) + 1);
  for(int i=0;i<strlen(data->name);i++){
    node->data->name[i] = data->name[i];
  }
  node->left = NULL;
  node->right = NULL;
  return node;
}

// Insert a node into the binary search tree
void insert(BST *bst, NodeData *data) {
  if (bst->root == NULL) {
    bst->root = create_node(data);
  } else {
    Node *cur_node = bst->root;
    while (cur_node) {
      if (strcmp(data->name, cur_node->data->name) < 0) {
        if (cur_node->left == NULL) {
          cur_node->left = create_node(data);
          break;
        }
        cur_node = cur_node->left;
      } else {
        if (cur_node->right == NULL) {
          cur_node->right = create_node(data);
          break;
        }
        cur_node = cur_node->right;
      }
    }
  }
}

Node *search_bst(Node *node, char *name) {
  if (node == NULL) {
    return NULL;
  }
  if (strcmp(node->data->name, name) == 0) {
    return node;
  } else if (strcmp(name, node->data->name) < 0) {
    return search_bst(node->left, name);
  } else {
    return search_bst(node->right, name);
  }
  return NULL;
}
// Print the binary search tree in alphabetical order
void print_bst_inorder(Node *node, int level) {
  if (node) {
    print_bst_inorder(node->left, level + 4);
    for (int i = 0; i < level; i++) {
      printf(" ");
    }
    if (node->data->is_dir) {
      printf("%s/\n", node->data->name);
    } else {
      printf("%s\n", node->data->name);
    }
    print_bst_inorder(node->right, level + 4);
  }
}
void print_bst_postorder(Node *node, int level) {
  if (node) {
    print_bst_postorder(node->right, level + 4);
    print_bst_postorder(node->left, level + 4);
    for (int i = 0; i < level; i++) {
      printf(" ");
    }
    if (node->data->is_dir) {
      printf("%s/\n", node->data->name);
    } else {
      printf("%s\n", node->data->name);
    }
  }
}
void print_bst_preorder(Node *node, int level) {
  if (node) {
    for (int i = 0; i < level; i++) {
      printf(" ");
    }
    if (node->data->is_dir) {
      printf("%s/\n", node->data->name);
    } else {
      printf("%s\n", node->data->name);
    }
    print_bst_preorder(node->right, level + 4);
    print_bst_preorder(node->left, level + 4);
  }
}
// Delete a node from the binary search tree
void delete_node(BST *bst, char *name) {
  Node *parent = NULL;
  Node *node = bst->root;

  // Find the node to be deleted
  while (node != NULL && strcmp(node->data->name, name) != 0) {
    parent = node;
    if (strcmp(name, node->data->name) < 0) {
      node = node->left;
    } else {
      node = node->right;
    }
  }

  // If the node was not found, return
  if (node == NULL) {
    return;
  }

  // If the node has no children, simply delete it
  if (node->left == NULL && node->right == NULL) {
    if (parent == NULL) {
      bst->root = NULL;
    } else if (node == parent->left) {
      parent->left = NULL;
    } else {
      parent->right = NULL;
    }
    free(node->data);
    free(node);
  }

  // If the node has one child, replace it with its child
  else if (node->left == NULL || node->right == NULL) {
    Node *child = (node->left != NULL) ? node->left : node->right;
    if (parent == NULL) {
      bst->root = child;
    } else if (node == parent->left) {
      parent->left = child;
    } else {
      parent->right = child;
    }
    free(node->data);
    free(node);
  }

  // If the node has two children, find the in-order predecessor (or successor)
  // to replace it
  else {
    Node *predecessor = node->left;
    parent = node;
    while (predecessor->right != NULL) {
      parent = predecessor;
      predecessor = predecessor->right;
    }
    node->data = predecessor->data;
    if (parent == node) {
      parent->left = predecessor->left;
    } else {
      parent->right = predecessor->left;
    }
    free(predecessor->data);
    free(predecessor);
  }
}

void parse_input(char *input) {
  char *token = strtok(input, " ");
  char *name;
  bool is_directory;
  bool is_file;
  if (strcmp(token, "insert") == 0) {
    token = strtok(NULL, " ");
    is_directory = strcmp(token, "directory") == 0 ? true : false;
    is_file = strcmp(token, "file") == 0 ? true : false;
    token = strtok(NULL, " ");
    name = token;
    NodeData node_data = {.name=name, .is_dir = is_directory};
    node_data.is_dir = is_directory;
    if (is_directory) {
      node_data.is_dir = true;
    }
    if (!is_directory && !is_file) {
      println("Not a valid insert type");
      return;
    }
    insert(&bst, &node_data);
    println("[ INFO ] : Successfully insertted");
  } else if (strcmp(token, "delete") == 0) {
    token = strtok(NULL, " ");
    name = token;
    delete_node(&bst, name);
  } else if (strcmp(token, "search") == 0) {
    token = strtok(NULL, " ");
    name = token;
    Node *result = search_bst(bst.root, name);
    if (result != NULL) {
      printf("%s found\n", name);
    } else {
      printf("%s not found\n", name);
    }
  } else if (strcmp(token, "traverse") == 0) {
    token = strtok(NULL, " ");
    if (strcmp(token, "preorder") == 0) {
      print_bst_preorder(bst.root, 0);
    } else if (strcmp(token, "inorder") == 0) {
      print_bst_inorder(bst.root, 0);
    } else if (strcmp(token, "postorder") == 0) {
      print_bst_postorder(bst.root, 0);
    }
  } else if (strcmp(token, "print") == 0) {
    print_bst_preorder(bst.root, 0);
  } else {
    println("Unknown command %s", token);
  }
}
// Example usage
int main() {
  //   NodeData root_dir_data = { .name = "root", .is_dir = 1 };
  //   insert(&bst, &root_dir_data);
  //   NodeData file1_data = { .name = "file1.txt", .is_dir = 0 };
  //   insert(&bst, &file1_data);
  //   NodeData dir1_data = { .name = "dir1", .is_dir = 1 };
  //   insert(&bst, &dir1_data);
  //   NodeData file2_data = { .name = "file2.txt", .is_dir = 0 };
  //   insert(&bst, &file2_data);
  //   NodeData file3_data = { .name = "file3.txt", .is_dir = 0 };
  //   insert(&bst, &file3_data);
  //   NodeData dir2_data = { .name = "dir2", .is_dir = 1 };
  //   insert(&bst, &dir2_data);
  //   NodeData file4_data = { .name = "file4.txt", .is_dir = 0 };
  //   insert(&bst, &file4_data);
  //   print_bst_inorder(bst.root, 0);
  //   Node* result = search_bst(bst.root, "file4.");
  char input[100];
  while (true) {
    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "exit\n") == 0) {
      break;
    }
    input[strlen(input) - 1] = '\0';
    parse_input(input);
  }
  return 0;
}
