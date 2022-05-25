//
// Define labels structures
//
#define UIDSIZE 30

typedef char o_r[8][UIDSIZE];     // {Owner; Reader1 ; R2; ...}
                               // maximum of 7 readers per owner
typedef o_r label[4];         // Array of owner_reader (max 4 owners)

extern void labecpy(label src, label dst);
