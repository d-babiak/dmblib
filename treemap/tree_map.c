#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KV KV;

struct KV {
  char *key;
  void *val;
  KV   *left;
  KV   *right;
};

KV *KV_init(char *key, void *val) {
  KV *kv = malloc(sizeof(KV));
  kv->key   = strdup(key);
  kv->val   = strdup(val);
  kv->left  = NULL;
  kv->right = NULL;
  return kv;
}

KV *M_set(KV *kv, char *key, void *val) {
  if (kv == NULL)
    return KV_init(key, val);

  int ord = strcmp(key, kv->key);

  if      (ord <  0) kv->left  = M_set(kv->left, key, val);
  else if (ord == 0) kv->val   = val;
  else if (ord >  0) kv->right = M_set(kv->right, key, val);

  return kv;
}

char *M_get(KV *kv, char *key) {
  if (!kv)
    return NULL;

  int ord = strcmp(key, kv->key);

  if      (ord <  0) return M_get(kv->left, key);
  else if (ord == 0) return kv->val;
  else if (ord >  0) return M_get(kv->right, key);
  else               return NULL;  // lol - no

}

void M_print(KV *kv) {
  if (!kv)
    return;

  M_print(kv->left);
  //printf(
  //  "  %s: %d\n", 
  //  kv->key, 
  //  *((int*)(kv->val))
  //);
  printf(
    "  %s: %s\n", 
    kv->key, 
    kv->val
  );
  M_print(kv->right);
}

char *trim(char *s) {
  size_t len = strlen(s);

  if (len > 0 && s[len - 1] == '\n')
    s[len - 1] = '\0';
  return s;
}

int main(int argc, char *argv[]) {
  FILE *fp = fopen(argv[1], "r");
  assert(fp);

  KV *kv = NULL;
  char key[256], val[256];
  for (;;) {
    int n = fscanf(fp, "%s %s\n", key, val);

    if (n != 2)
      break;

    kv = M_set(kv, key, val);
  }

  M_print(kv);

  char buf[4096];
  while (fgets(buf, sizeof buf, stdin)) {
    char *k = trim(buf);
    char *v = M_get(kv, k);
    printf("  %s: %s (is null? %d)\n", k, v, v == NULL);
  }
}
