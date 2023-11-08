#include "test.h"
#include "map.h"
#include <stdio.h>
#include <malloc.h>

void test_int_int();
void test_char_int();
void test_char_char();
void test_char_struct();

struct pair_int {
  int fi, se;
};
int pair_int_cmp(void *lhs, void *rhs);
void *pair_int_clone(void *val);
void pair_int_free(void *val);

int main()
{
  TEST_PRINT_FILE();

  TestFunction tf[] = {
    test_int_int,
    test_char_int,
    test_char_char,
    test_char_struct,
    NULL
  };

  for (int i = 0; tf[i] != NULL; i++) {
    tf[i]();
    printf("\n");
  }
}

void test_int_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_int, treap_utils.tu_int);

  map->insert(map, &(int){1}, &(int){1});
  map->insert(map, &(int){2}, &(int){2});
  printf("%d\n", *(int *)map->find(map, &(int){1}));
  printf("%d\n", *(int *)map->find(map, &(int){2}));
  if (map->find(map, &(int){2}) == NULL) { printf("can not find 2\n"); }
  if (map->find(map, &(int){3}) == NULL) { printf("can not find 3\n"); }

  map->insert(map, &(int){2}, &(int){22});
  printf("%d\n", *(int *)map->find(map, &(int){2}));
  printf("%d\n", map->size(map));

  map->erase(map, &(int){3});
  map->erase(map, &(int){1});
  if (map->find(map, &(int){1}) == NULL) { printf("can not find 1\n"); }

  free_map(map);
}

void test_char_int()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_char, treap_utils.tu_int);

  map->insert(map, "John", &(int){100});
  map->insert(map, "Mary", &(int){98});
  map->insert(map, "David", &(int){70});
  printf("%d\n", *(int *)map->find(map, "John"));
  printf("%d\n", *(int *)map->find(map, "Mary"));
  if (map->find(map, "David") == NULL) { printf("can not find David\n"); }
  if (map->find(map, "Tom") == NULL) { printf("can not find Tom\n"); }

  map->erase(map, "Tom");
  map->erase(map, "John");
  if (map->find(map, "John") == NULL) { printf("can not find John\n"); }
  printf("%d\n", *(int *)map->find(map, "David"));

  free_map(map);
}

void test_char_char()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_char, treap_utils.tu_char);

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[2];
    cc[0] = c, cc[1] = '\0';

    map->insert(map, cc, cc);
  }

  for (char c = 'a'; c <= 'z'; c++) {
    char cc[2];
    cc[0] = c, cc[1] = '\0';

    if (map->find(map, cc) != NULL) {
      printf("%s", (char *)map->find(map, cc));
    }
  }
  printf("\n");

  free_map(map);
}

int pair_int_cmp(void *lhs, void *rhs)
{
  struct pair_int *le = (struct pair_int *)lhs;
  struct pair_int *ri = (struct pair_int *)rhs;

  if (le->fi == ri->fi && le->se == ri->se) return 0;
  return le->fi > ri->fi ? 1 : -1;
}

void *pair_int_clone(void *val)
{
  struct pair_int *val_copy = (struct pair_int *)malloc(sizeof(struct pair_int));

  val_copy->fi = ((struct pair_int *)val)->fi;
  val_copy->se = ((struct pair_int *)val)->se;

  return val_copy;
}
void pair_int_free(void *val)
{
  free((struct pair_int *)val);
}

void test_char_struct()
{
  TEST_PRINT_FUNC();

  map_t *map = new_map(treap_utils.tu_char, (treap_utils_t) {
    pair_int_cmp, pair_int_clone, pair_int_free
  });

  map->insert(map, "{1, 2}", &(struct pair_int){ 1, 2 });
  map->insert(map, "{2, 3}", &(struct pair_int){ 2, 3 });

  printf("%d\n", ((struct pair_int *)map->find(map, "{2, 3}"))->fi);

  free_map(map);
}