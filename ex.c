#include "ex.h"

void push(void* ctx, void* thing) {
}

void pop(void* ctx, void* thing) {
}

// TODO make ctx a queue
void type_annotate(void* ctx, Node* n) {
  n->ty = malloc(sizeof(Type));
  switch (n->tag) {
    case TAG_VAR:
      // TODO don't want to leak this, don't want this to be uninitialized.
      /* n->ty = ctx[n->idx]; */
      break;
    case TAG_APP:
      type_annotate(ctx, n->f);
      type_annotate(ctx, n->arg);
      // TODO n->ty = ...
      break;
    case TAG_IFZ:
      type_annotate(ctx, n->cond);
      type_annotate(ctx, n->then);
      type_annotate(ctx, n->els);
      // TODO probably need to implement an equality function.
      assert(n->then->ty == n->els->ty);
      n->ty = n->then->ty;
      break;
    case TAG_LAMBDA:
      type_annotate(ctx, n->body);
      push(ctx, n->arg_type);
      pop(ctx, n->arg_type);
      n->ty->tag = TYPEC_ARROW;
      n->ty->d = n->arg_type;
      n->ty->c = n->body->ty;
      break;
    case TAG_PACK:
      // TODO
      assert(false);
      break;
    case TAG_UNPACK:
      // TODO
      assert(false);
      break;
    case TAG_SUCC:
      type_annotate(ctx, n->n);
      assert(n->n->ty->tag == TYPEC_INT);
      n->ty = TYPEC_INT;
      break;
    case TAG_ZERO:
      // TODO syntax for pointers
      n->tag = TYPEC_INT;
      break;
    default:
      assert(false);
  }
  assert(n->ty);
}

int main() {
    return 0;
}
