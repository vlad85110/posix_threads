//
// Created by Влад Кирилов on 25.11.2022.
//

#pragma once

struct detail_a {
    size_t id;
};
typedef struct detail_a detail_a_t;

struct detail_b {
    size_t id;
};
typedef struct detail_b detail_b_t;

struct module {
    size_t id;
    struct detail_a a;
    struct detail_b b;
};
typedef struct detail_c detail_c_t;

struct detail_c {
    size_t id;
};

struct widget {
    struct module module;
    struct detail_c detail_c;
};



detail_a_t create_detail_a();

detail_b_t create_detail_b();

detail_c_t create_detail_c();


