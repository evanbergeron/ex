#ifndef EX_H
#define EX_H
#include <stdlib.h>
#include <stdbool.h>

// Linked list elem
typedef struct Llelem {
  void* data;
  struct Llelem *next;
} Llelem;

enum {
  TAG_VAR,
  TAG_APP,
  TAG_IFZ,
  TAG_LAMBDA,
  TAG_PACK,
  TAG_OPEN,
  TAG_SUCC,
  TAG_ZERO,
  TAG_TYPE_LAMBDA,
  TAG_TYPE_APP
};

enum {
  TYPEC_INT,
  TYPEC_ARROW,
  TYPEC_EXISTS,
  TYPEC_VAR,
  TYPEC_ALL
};

typedef struct Type {
  int tag;
  union {
    // int
    // Arrow
    struct {
      struct Type* d;
      struct Type* c;
    };
    // Var
    struct {
      const char* type_name;
      int idx; // De Bruijn index
    };
    // Exists
    struct {
      int t;
      struct Type* r;
    };
    // All (binds!)
    struct {
      struct Type* domain_type;
      struct Type* codomain_type;
    };
  };
} Type;

typedef struct Node {
  int tag;
  Type* ty;
  union {
    // Variable
    struct {
      const char* var_name;
      int idx; // De Bruijn index
    };
    // Application
    struct {
      struct Node *f;
      struct Node *arg;
    };
    // Ifz
    struct {
      struct Node *cond;
      struct Node *then;
      struct Node *els;
    };
    // Lambda
    struct {
      const char* lambda_name;
      const char* arg_name;
      Type* arg_type;
      struct Node* body;
    };
    // Pack
    struct {
      Type* impl_type;
      struct Node* to_pack;
    };
    // Open
    struct {
      // User gives us a type and variable name
      // The variable name follows the usual scoping rules.
      // The type name should also follow the usual scoping rules.
      Type* fresh_type;
      struct Node* fresh_var;

      struct Node* package;

      // Kinda don't like this AST design, but anything else requires
      // thought.
      struct Node* rest_of_code;
    };
    // Succ
    struct {
      struct Node *n;
    };
    // Zero
    // Type lambda
    struct {
      Type* type_var;
      struct Node* poly_body;
    };
    // Type application
    struct {
      struct Node *polymorphic_f;
      Type *type_arg;
    };
  };
} Node;

typedef struct Ctx {
  void* type_vars;
  void* term_vars;
} Ctx;

#endif
