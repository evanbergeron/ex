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
  TAG_UNPACK,
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
      int idx; // De Bruijn index
    };
    // Exists
    struct {
      int t;
      int r;
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
      const char* varname;
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
      struct Type* arg_type;
      struct Node* body;
    };
    // Pack
    struct {
      struct Type* impl_type;
      struct Node* to_pack;
    };
    // Unpack 
    struct {
      struct Type* interface_type; // fresh in its scope
      struct Node* to_unpack;
    };
    // Succ 
    struct {
      struct Node *n;
    };
    // Zero
    // Type lambda
    struct {
      struct Type* type_var;
      struct Node* poly_body;
    };
    // Type application
    struct {
      struct Node *polymorphic_f;
      struct Type *type_arg;
    };
  };
} Node;

typedef struct Ctx {
  void* type_vars;
  void* term_vars;
} Ctx;

#endif
