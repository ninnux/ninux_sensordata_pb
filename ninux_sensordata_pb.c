#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.pb-c.h"


int myarray_init(Tutorial__Array *array){
  //inizializza array con set vuoto
  Tutorial__Myset **sets;
  sets=malloc(sizeof (Tutorial__Myset *));
  array->n_sets = 0;
  array->sets=sets;
  return 0;
}

int myarray_add_set(Tutorial__Array *array, Tutorial__Myset *set, int timestamp){
  //aggiunge un set creato esternamente all'array nella prossima posizione libera
  (array->n_sets)+=1;
  //printf("num set:%d\n",array->n_sets);
  array->sets = realloc (array->sets,sizeof (Tutorial__Myset *)* array->n_sets); //one insert at once
  array->sets[array->n_sets-1]=set;
  tutorial__myset__init (array->sets[array->n_sets-1]);
  array->sets[array->n_sets-1]->timestamp=timestamp;
  array->sets[array->n_sets-1]->n_entries=0;
  Tutorial__Entry **entries;
  entries=malloc(sizeof (Tutorial__Entry *));
  array->sets[array->n_sets-1]->entries=entries;
  return 0;
}

int myarray_add_entry(Tutorial__Array *array, Tutorial__Myset *set, char* key, int value){
  //aggiunge un entry al set indicato aggiungendolo alle entries del set
  set->n_entries+=1;
  array->sets[array->n_sets-1]->entries=realloc(set->entries,sizeof (Tutorial__Entry *)* set->n_entries);
  array->sets[array->n_sets-1]->n_entries=set->n_entries;
  array->sets[array->n_sets-1]->entries[set->n_entries-1]=malloc(sizeof(Tutorial__Entry));
  tutorial__entry__init (array->sets[array->n_sets-1]->entries[set->n_entries-1]);
  array->sets[array->n_sets-1]->entries[set->n_entries-1]->key=malloc(sizeof(char)*5);
  sprintf(array->sets[array->n_sets-1]->entries[set->n_entries-1]->key,"%s",key);
  array->sets[array->n_sets-1]->entries[set->n_entries-1]->value=value;
  return 0;
}

int impacchetto(Tutorial__Array *array){
  int len=0;
  uint8_t* buf;
  len = tutorial__array__get_packed_size (array); // This is the calculated packing length
  printf("Writing %d serialized bytes\n",len); // See the length of message
  buf = malloc (len);                     // Allocate memory
  tutorial__array__pack (array, buf);             // Pack msg, including submessages
  printf("buf:%s",(char*)buf);   
  //fwrite (buf, len, 1, stdout); 
  return 0;
}

int serialize(Tutorial__Array *array, uint8_t **buf){
  int len=0;
  len = tutorial__array__get_packed_size (array); // This is the calculated packing length
  printf("Serialize: Writing %d serialized bytes\n",len); // See the length of message
  *buf = malloc (len);                     // Allocate memory
  tutorial__array__pack (array, *buf);             // Pack msg, including submessages
  return 0;
}

//int deserialize(Tutorial__Array *array, uint8_t *buf){
//  printf("buf len:%d\n",strlen((char*)buf)); // See the length of message
//  array = tutorial__array__unpack(NULL,strlen((char*)buf),(char*)buf);
//  return 0;
//}

int dealloco(Tutorial__Array *array){
  int i,k;
  for(i=0;i<array->n_sets;i++){
	for(k=0;k<array->sets[i]->n_entries;k++){
 	 	free(array->sets[i]->entries[k]);
	}
	free(array->sets[i]->entries);
  }
  free(array->sets);
  return 0;
}


int print_all(Tutorial__Array *array){
  int i,k;
  for(i=0;i<array->n_sets;i++){
	for(k=0;k<array->sets[i]->n_entries;k++){
 	 	printf("t:%d %s %d\n",array->sets[i]->timestamp,array->sets[i]->entries[k]->key,array->sets[i]->entries[k]->value);
	}
  }
  return 0;
}
