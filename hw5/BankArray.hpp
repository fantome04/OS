#include "bank_cell.h"

class BankArray{
public:
    unsigned int bankSize;
    BankCell cells[];

    BankCell& operator[](unsigned int ind){
        return cells[ind];
    }
};
