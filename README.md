#esp-idf component for managing ninuxsensors data by protocolbuf

##example

#include "ninux_sensordata_pb.h"

int test4(){
  Ninux__Sensordata sensordata = NINUX__SENSORDATA__INIT;
  sensordata_init(&sensordata);


  Ninux__Myset *set0;
  set0= malloc (sizeof (Ninux__Myset)); //one insert at once

  sensordata_add_set(&sensordata,set0,1574244029);
  sensordata_add_entry(&sensordata,set0,"temp",19);
  sensordata_add_entry(&sensordata,set0,"pres",1013);
  sensordata_add_entry(&sensordata,set0,"batt",42);
  sensordata_add_entry(&sensordata,set0,"wind",223);

  printf("key: %s\n", set0->entries[set0->n_entries-1]->key);
  printf("key: %s\n", sensordata.sets[sensordata.n_sets-1]->entries[set0->n_entries-1]->key);

  Ninux__Myset *set1;
  set1= malloc (sizeof (Ninux__Myset)); //one insert at once
  sensordata_add_set(&sensordata,set1,1574244329);
  sensordata_add_entry(&sensordata,set1,"batt",42);
  sensordata_add_entry(&sensordata,set1,"wind",223);


  Ninux__Myset *set2;
  set2= malloc (sizeof (Ninux__Myset)); //one insert at once
  sensordata_add_set(&sensordata,set2,1574244629);
  sensordata_add_entry(&sensordata,set2,"temp",19);
  sensordata_add_entry(&sensordata,set2,"pres",1013);


  //impacchetto(&Sensordata);
  char* buffer;
  sensordata_serialize(&sensordata,&buffer);
  printf("mio buffer: %s\n",buffer);
  sensordata_print_all(&sensordata);
  sensordata_free(&sensordata);
  return 0;
}
