//
// Created by Daniel on 13.05.2019.
//

#include "Shop.h"

int Shop::add_customer(Customer *c) {
    WriteLock lck(lck_shop);
    return 0;
}
