#ifndef CCS811_H
#define CCS811_H

struct CCS811Data {
  float eco2;
  float tvoc;
  bool valid;
};

#endif