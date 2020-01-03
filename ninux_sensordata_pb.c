#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ninux_sensordata_pb.h"



int sensordata_init(Ninux__Sensordata *Sensordata){
  //inizializza Sensordata con set vuoto
  Ninux__Myset **sets;
  sets=malloc(sizeof (Ninux__Myset *));
  Sensordata->n_sets = 0;
  Sensordata->sets=sets;
  return 0;
}

int sensordata_init2(unsigned char** buffer, int* buffer_len){
  Ninux__Sensordata sensordata = NINUX__SENSORDATA__INIT;
  sensordata_init(&sensordata);
  //char* buffer;
  sensordata_serialize2_size(&sensordata,buffer,buffer_len);
  return 0;
}

int sensordata_add_set(Ninux__Sensordata *Sensordata, Ninux__Myset *set, int timestamp){
  //aggiunge un set creato esternamente all'Sensordata nella prossima posizione libera
  (Sensordata->n_sets)+=1;
  //printf("num set:%d\n",Sensordata->n_sets);
  Sensordata->sets = realloc (Sensordata->sets,sizeof (Ninux__Myset *)* Sensordata->n_sets); //one insert at once
  Sensordata->sets[Sensordata->n_sets-1]=set;
  ninux__myset__init (Sensordata->sets[Sensordata->n_sets-1]);
  Sensordata->sets[Sensordata->n_sets-1]->timestamp=timestamp;
  Sensordata->sets[Sensordata->n_sets-1]->n_entries=0;
  Ninux__Entry **entries;
  entries=malloc(sizeof (Ninux__Entry *));
  Sensordata->sets[Sensordata->n_sets-1]->entries=entries;
  return 0;
}

int sensordata_add_entry(Ninux__Sensordata *Sensordata, Ninux__Myset *set, char* key, int value){
  //aggiunge un entry al set indicato aggiungendolo alle entries del set
  set->n_entries+=1;
  Sensordata->sets[Sensordata->n_sets-1]->entries=realloc(set->entries,sizeof (Ninux__Entry *)* set->n_entries);
  Sensordata->sets[Sensordata->n_sets-1]->n_entries=set->n_entries;
  Sensordata->sets[Sensordata->n_sets-1]->entries[set->n_entries-1]=malloc(sizeof(Ninux__Entry));
  ninux__entry__init (Sensordata->sets[Sensordata->n_sets-1]->entries[set->n_entries-1]);
  Sensordata->sets[Sensordata->n_sets-1]->entries[set->n_entries-1]->key=malloc(sizeof(char)*5);
  sprintf(Sensordata->sets[Sensordata->n_sets-1]->entries[set->n_entries-1]->key,"%s",key);
  Sensordata->sets[Sensordata->n_sets-1]->entries[set->n_entries-1]->value=value;
  return 0;
}

int sensordata_pack(Ninux__Sensordata *Sensordata){
  int len=0;
  uint8_t* buf;
  len = ninux__sensordata__get_packed_size (Sensordata); // This is the calculated packing length
  printf("Writing %d serialized bytes\n",len); // See the length of message
  buf = malloc (len);                     // Allocate memory
  ninux__sensordata__pack (Sensordata, buf);             // Pack msg, including submessages
  printf("buf:%s",(char*)buf);   
  //fwrite (buf, len, 1, stdout); 
  return 0;
}

int sensordata_serialize(Ninux__Sensordata *Sensordata,unsigned char **buf){
  int len=0;
  len = ninux__sensordata__get_packed_size (Sensordata); // This is the calculated packing length
  printf("Serialize: Writing %d serialized bytes\n",len); // See the length of message
  *buf = malloc (len+1);                     // Allocate memory
  bzero(*buf,len+1);
  ninux__sensordata__pack (Sensordata, *buf);             // Pack msg, including submessages
  return 0;
}

int sensordata_serialize2(Ninux__Sensordata *Sensordata,unsigned char **buf){
  int len=0;
  len = ninux__sensordata__get_packed_size (Sensordata); // This is the calculated packing length
  printf("Serialize: Writing %d serialized bytes\n",len); // See the length of message
  bzero(*buf,len+1);
  ninux__sensordata__pack (Sensordata, *buf);             // Pack msg, including submessages
  return 0;
}
int sensordata_serialize2_size(Ninux__Sensordata *Sensordata,unsigned char **buf,int* size){
  *size = ninux__sensordata__get_packed_size (Sensordata); // This is the calculated packing length
  printf("Serialize: Writing %d serialized bytes\n",*size); // See the length of message
  ninux__sensordata__pack (Sensordata, buf);             // Pack msg, including submessages
  sensordata_free(Sensordata);
  return 0;
}

//int sensordata_deserialize(Ninux__Sensordata **Sensordata, uint8_t *buf){
//  printf("buf len:%s\n",buf); // See the length of message
//  *Sensordata = ninux__sensordata__unpack(NULL,strlen((char *)buf),(unsigned char *) buf);
//  return 0;
//}
int sensordata_deserialize(Ninux__Sensordata **sensordata, uint8_t *buf){
  printf("buf len:%d\n",strlen((char*)buf)); // See the length of message
  *sensordata = ninux__sensordata__unpack(NULL,strlen((char*)buf),(uint8_t *)buf);
  return 0;
}

int sensordata_deserialize2_size(Ninux__Sensordata **sensordata, uint8_t *buf, int* size){
  *sensordata = ninux__sensordata__unpack(NULL,*size,(uint8_t *)buf);
  //ninux__sensordata__free_unpacked(*sensordata,NULL);
  return 0;
}

int sensordata_free(Ninux__Sensordata *Sensordata){
  int i,k;
  for(i=0;i<Sensordata->n_sets;i++){
	for(k=0;k<Sensordata->sets[i]->n_entries;k++){
 	 	free(Sensordata->sets[i]->entries[k]);
	}
	free(Sensordata->sets[i]->entries);
  }
  free(Sensordata->sets);
  return 0;
}


int sensordata_print_all(Ninux__Sensordata *Sensordata){
  int i,k;
  for(i=0;i<Sensordata->n_sets;i++){
	for(k=0;k<Sensordata->sets[i]->n_entries;k++){
 	 	printf("t:%d %s %d\n",Sensordata->sets[i]->timestamp,Sensordata->sets[i]->entries[k]->key,Sensordata->sets[i]->entries[k]->value);
	}
  }
  return 0;
}


int sensordata_print_all2(unsigned char** buffer,int* size){
  Ninux__Sensordata *sensordata2;
  sensordata2=malloc(sizeof(Ninux__Sensordata));
  Ninux__Sensordata **psensordata2 = &sensordata2;
  sensordata_deserialize2_size(psensordata2, buffer,size);
  sensordata_print_all(sensordata2);
  free(sensordata2);
  return 0;
}

int sensordata_insert_values2(unsigned char** buffer, int timestamp, char** keys,int* values, int len, int* size){

  /// inserimento singolo a rotazione
  Ninux__Sensordata *sensordata2;
  sensordata2=malloc(sizeof(Ninux__Sensordata));
  Ninux__Sensordata **psensordata2 = &sensordata2;
  printf("deserializzo %d bytes\n",*size);
  //sensordata2=ninux__sensordata__unpack(NULL,*size,buffer);/// FUNZIONA
  sensordata_deserialize2_size(psensordata2, buffer,size);
  //printf("size:%d\n",*size);
  Ninux__Myset *set3;
  set3= malloc (sizeof (Ninux__Myset)); //one insert at once
  sensordata_add_set(sensordata2,set3,timestamp);
  int i;
  for(i=0; i<len;i++){
  	sensordata_add_entry(sensordata2,set3,keys[i],values[i]);
  }
  //sensordata_print_all(sensordata2);
  //printf("size buf:%d\n",*size);
  sensordata_serialize2_size(sensordata2, buffer,size);
  free(sensordata2);
  //free(set3);
  /////////////
  return 0;
}
