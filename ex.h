#ifndef EX_H
#define EX_H

enum {
  TAG_VAR,
  TAG_APP,
  TAG_IFZ,
  TAG_LAMBDA,
  TAG_PACK,
  TAG_UNPACK,
  TAG_SUCC,
  TAG_ZERO,
};

typedef struct Type {
  int todo;
} Type;

typedef struct Node {
  int tag;
  Type* ty;
  union {
    // Variable
    struct {
      char* varname;
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
  };
} Node;

#endif
