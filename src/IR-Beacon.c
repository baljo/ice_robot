// ------ Libraries and Definitions ------
#include "simpletools.h"
#include "abvolts.h"

// ------ Global Variables and Objects ------
int dimmer;

// ------ Main Program ------
int main() {
  da_init(26, 27);
  while(1) {
    for (dimmer = 0; dimmer < 160; dimmer += (10)) {
      da_out(0, (dimmer* 256 / 330));
      freqout(11, 3, 38000);
    }}
}
