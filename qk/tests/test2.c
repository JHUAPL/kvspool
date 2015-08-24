#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "qk.h"

int dump(struct qk *qk) {
  size_t l = utvector_len(&qk->vals);

  utstring_printf(&qk->tmp, "qk_end: %lu kv pairs\n", l);

  UT_string *k=NULL, *v=NULL;
  while(l--) {
    k = (UT_string*)utvector_next(&qk->keys,k);
    v = (UT_string*)utvector_next(&qk->vals,v);
    assert(k); assert(v);
    utstring_printf(&qk->tmp, "%s -> %s\n", 
       utstring_body(k), utstring_body(v));
  }

  char *out = utstring_body(&qk->tmp);
  size_t len = utstring_len(&qk->tmp);
  write(STDOUT_FILENO, out, len);

  return 0;
}

int main() {
  struct qk *qk = qk_new();

  qk->cb = dump;

  qk_start(qk);
  qk_add(qk, "a",  "%d", 1);
  qk_add(qk, "b",  "%d%c", 2, 'a');
  qk_add(qk, "c",  "%d%c", 3, 'b');
  qk_add(qk, "d",  "%s", "IV");
  qk_add(qk, "e",  "%s", "IV");
  qk_end(qk);

  qk_start(qk);
  qk_add(qk, "A",  "%d", 1);
  qk_add(qk, "B",  "%d%c", 2, 'a');
  qk_add(qk, "C",  "%d%c", 3, 'b');
  qk_add(qk, "D",  "%s", "IV");
  qk_end(qk);

  qk_free(qk);

  return 0;
}
