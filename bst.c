#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define println(...) printf(__VA_ARGS__);printf("\n");

// Struct to represent a file or directory node
struct file_node {
  char *name;
  bool is_directory;
  struct file_node *left_child;
  struct file_node *right_child;
};

// Root node of the directory structure BST
struct file_node *root = NULL;

// Function to create a new file or directory node
struct file_node *create_node(char *name, bool is_directory) {
  struct file_node *node = (struct file_node *)malloc(sizeof(struct file_node));
  node->name = name;
  node->is_directory = is_directory;
  node->left_child = NULL;
  node->right_child = NULL;
  return node;
}

// Function to insert a new file or directory node
void insert_node(char *name, bool is_directory) {
  struct file_node *node = create_node(name, is_directory);
  if (root == NULL) {
    println("Creating root node");
    root = node;
  } else {
    println("Creating non-root node")
    struct file_node *temp_node = root;
    while (true) {
      if (strcmp(name, temp_node->name) < 0) {
        if (temp_node->left_child == NULL) {
          temp_node->left_child = node;
          break;
        }
        temp_node = temp_node->left_child;
      } else {
        if (temp_node->right_child == NULL) {
          temp_node->right_child = node;
          break;
        }
        temp_node = temp_node->right_child;
      }
    }
  }
}

// Function to find the minimum node in the right subtree of a node to be
// deleted
struct file_node *find_min_node(struct file_node *node) {
  struct file_node *temp_node = node;
  while (temp_node->left_child != NULL) {
    temp_node = temp_node->left_child;
  }
  return temp_node;
}

// Function to delete an existing file or directory node
struct file_node *delete_node(struct file_node *node, char *name) {
  if (node == NULL) {
    return node;
  }
  if (strcmp(name, node->name) < 0) {
    node->left_child = delete_node(node->left_child, name);
  } else if (strcmp(name, node->name) > 0) {
    node->right_child = delete_node(node->right_child, name);
  } else {
    if (node->left_child == NULL) {
      struct file_node *temp_node = node->right_child;
      free(node);
      return temp_node;
    } else if (node->right_child == NULL) {
      struct file_node *temp_node = node->left_child;
      free(node);
      return temp_node;
    }
    struct file_node *temp_node = find_min_node(node->right_child);
    node->name = temp_node->name;
    node->is_directory = temp_node->is_directory;
    node->right_child = delete_node(node->right_child, temp_node->name);
  }
  return node;
}

// Function to search for a file or directory node by name
struct file_node *search_node(struct file_node *node, char *name) {
  if (node == NULL || strcmp(name, node->name) == 0) {
    return node;
  }
  if (strcmp(name, node->name) < 0) {
    return search_node(node->left_child, name);
  }
  return search_node(node->right_child, name);
}

// Function to traverse the directory structure in pre-order
void traverse_preorder(struct file_node *node) {
  if (node != NULL) {
    println("Traversing in Preorder");
    printf("%s\n", node->name);
    traverse_preorder(node->left_child);
    traverse_preorder(node->right_child);
  }
}

// Function to traverse the directory structure in in-order
void traverse_inorder(struct file_node *node) {
  if (node != NULL) {
    println("Traversing in Inorder");
    traverse_inorder(node->left_child);
    printf("%s\n", node->name);
    traverse_inorder(node->right_child);
  }
}

// Function to traverse the directory structure in post-order
void traverse_postorder(struct file_node *node) {
  if (node != NULL) {
    println("Traversing in Postorder");
    traverse_postorder(node->left_child);
    traverse_postorder(node->right_child);
    printf("%s\n", node->name);
  }
}

// Function to print the directory structure in a formatted way
void print_directory(struct file_node *node, int level) {
  if (node == NULL) {
    return;
  }
  print_directory(node->right_child, level + 1);
  for (int i = 0; i < level; i++) {
    printf(" ");
  }
  if (node->is_directory) {
    printf("Directory : %s/\n", node->name);
  } else {
    printf("File : %s\n", node->name);
  }
  print_directory(node->left_child, level + 1);
}

// Function to parse user input and execute appropriate commands
void parse_input(char *input) {
  char *token = strtok(input, " ");
  char *name;
  bool is_directory;
  if (strcmp(token, "insert") == 0) {
    token = strtok(NULL, " ");
    is_directory = strcmp(token, "directory") == 0 ? true : false;
    token = strtok(NULL, " ");
    name = token;
    insert_node(name, is_directory);
  } else if (strcmp(token, "delete") == 0) {
    token = strtok(NULL, " ");
    name = token;
    root = delete_node(root, name);
  } else if (strcmp(token, "search") == 0) {
    token = strtok(NULL, " ");
    name = token;
    struct file_node *result = search_node(root, name);
    if (result != NULL) {
      printf("%s found\n", name);
    } else {
      printf("%s not found\n", name);
    }
  } else if (strcmp(token, "traverse") == 0) {
    token = strtok(NULL, " ");
    println("token : %s", token);
    if (strcmp(token, "preorder") == 0) {
        println("Inside preorder");
        root == NULL? printf("NULL\n") : println("Not NULL");
      traverse_preorder(root);
    } else if (strcmp(token, "inorder") == 0) {
        println("Inside inorder");
        root == NULL? printf("NULL\n") : println("Not NULL");
      traverse_inorder(root);
    } else if (strcmp(token, "postorder") == 0) {
        println("Inside postorder");
        root == NULL? printf("NULL\n") : println("Not NULL");
        root == NULL? printf("NULL\n") : println("Not NULL");
      traverse_postorder(root);
    }
  } else if (strcmp(token, "print") == 0) {
    print_directory(root, 0);
  }else {
    println("Unknown command %s", token);
  }
}

int main() {
  char input[100];
  while (true) {
    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "exit\n") == 0) {
      break;
    }
    parse_input(input);
  }
  return 0;
}
