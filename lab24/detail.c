//
// Created by Влад Кирилов on 26.11.2022.
//

#include <unistd.h>
#include "detail.h"

detail_a_t create_detail_a() {
    static size_t id = 0;

    sleep(1);
    detail_a_t detailA;
    detailA.id = id++;
    return detailA;
}

detail_b_t create_detail_b() {
    static size_t id = 0;

    sleep(2);
    detail_b_t detailB;
    detailB.id = id++;
    return detailB;
}

struct detail_c create_detail_c() {
    static size_t id = 0;

    sleep(3);
    detail_c_t detailC;
    detailC.id = id++;
    return detailC;
}



