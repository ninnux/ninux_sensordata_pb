#include "array.pb-c.h"


int myarray_init(Tutorial__Array *array);
int myarray_add_set(Tutorial__Array *array, Tutorial__Myset *set, int timestamp);
int myarray_add_entry(Tutorial__Array *array, Tutorial__Myset *set, char* key, int value);

int impacchetto(Tutorial__Array *array);

int serialize(Tutorial__Array *array, uint8_t **buf);
//int deserialize(Tutorial__Array *array, uint8_t *buf);
int dealloco(Tutorial__Array *array);

int print_all(Tutorial__Array *array);
