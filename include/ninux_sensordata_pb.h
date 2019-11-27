#include "sensordata.pb-c.h"


int sensordata_init(Ninux__Sensordata *Sensordata);
int sensordata_add_set(Ninux__Sensordata *Sensordata, Ninux__Myset *set, int timestamp);
int sensordata_add_entry(Ninux__Sensordata *Sensordata, Ninux__Myset *set, char* key, int value);

int sensordata_pack(Ninux__Sensordata *Sensordata);

int sensordata_serialize(Ninux__Sensordata *Sensordata,unsigned char **buf);
int sensordata_deserialize(Ninux__Sensordata **Sensordata, unsigned char *buf);
int sensordata_free(Ninux__Sensordata *Sensordata);

int sensordata_print_all(Ninux__Sensordata *Sensordata);
