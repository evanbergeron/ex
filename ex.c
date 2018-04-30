#include "ex.h"

void push(Ctx* ctx, void* thing) {
}

void push_type(Ctx* ctx, void* thing) {
}

void pop(Ctx* ctx) {
}

void pop_type(Ctx* ctx) {
}

Type* subst(Type* replacing, Type* to_replace, Type* t) {
  return NULL;
}

// TODO make ctx a queue
void type_annotate(Ctx* ctx, Node* n) {
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
      pop(ctx);
      n->ty->tag = TYPEC_ARROW;
      n->ty->d = n->arg_type;
      n->ty->c = n->body->ty;
      break;
    case TAG_PACK:
      push_type(ctx, n->impl_type);
      type_annotate(ctx, n->to_pack);
      // TODO
      /* n->ty = (TYPEC_EXISTS, n->impl, n->to_pack->ty) */
      pop_type(ctx);
      break;
    case TAG_OPEN:
      // TODO I'm pretty confidant there's a scoping issue here. I
      // think n->rest_of_code should be typechecked without knowledge
      // of n->fresh_type, since n->rest_of_code->ty is the type of
      // the final result and n->fresh_type is only in scope within
      // this expression.

      // Annotate without knowledge of the client's type and var.
      type_annotate(ctx, n->package);
      assert(n->package->ty->tag == TYPEC_EXISTS);
      Type* tau = n->package->ty->r;

      // TODO Maybe these should be asserts?
      n->fresh_var->tag = TYPEC_VAR;
      n->fresh_var->ty = tau;

      push(ctx, n->fresh_var->ty);
      push_type(ctx, n->fresh_type);

      // Note that this gives us type-correctness of the rest of the
      // code without any knowledge of the implmentation type.
      type_annotate(ctx, n->rest_of_code);

      // TODO assert n->rest_of_code->ty does not involve n->fresh_type.
      // That's the result of the whole thing and we can't export that.

      n->ty = n->rest_of_code->ty;

      pop(ctx);
      pop_type(ctx);

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
    case TAG_TYPE_LAMBDA:
      push_type(ctx, n->type_var);
      type_annotate(ctx, n->poly_body);
      pop_type(ctx);
    case TAG_TYPE_APP:
      type_annotate(ctx, n->polymorphic_f);
      assert(n->polymorphic_f->ty->tag == TYPEC_ALL);
      // TODO impl. Subst type_arg for domain_type in codomain_type
      n->ty = subst(n->type_arg, n->polymorphic_f->ty->domain_type,
                    n->polymorphic_f->ty->codomain_type);
    default:
      assert(false);
  }
  assert(n->ty);
}

int main() {
    return 0;
}
