#include "bank_cell.h"


BankCell::BankCell()
        :current_balance(0) {}

int BankCell::get_min_balance() {
    return min_amount;
}

int BankCell::get_max_balance() {
    return max_amount;
}

int BankCell::get_curr_balance() {
    return current_balance;
}

void BankCell::freeze() {
    is_frozen = true;
}

void BankCell::unfreeze() {
    is_frozen = false;
}

bool BankCell::is_frozen() {
    return is_frozen;
}

bool BankCell::receive_amount(int amount) {} {
    if(current_balance + amount <= max_amount) {
        current_balance += amount;
        return true;
    }

    return false;
}

bool BankCell::send_amount(int amount) {
    if(current_balance - amount >= min_amount) {
        current_balance -= amount;
        return true;
    }

    return false;
}

void BankCell::set_min_amount(int amount) {
    min_amount = amount;
}

void set_max_amount(int amount) {
    max_amount = amount;
}


