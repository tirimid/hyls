// Copyright (c) 2022 Dmitrii Fedorin
// This code is licensed under an MIT license. Check LICENSE for details.

#ifndef TMDF_H__
#define TMDF_H__

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

enum tmdf_item_type
{
    TMDF_ITEM_TYPE_STRING,
    TMDF_ITEM_TYPE_NUMBER,
};

struct tmdf_item
{
    char    *key;
    uint32_t flags;

    enum tmdf_item_type type;
    union
    {
        char *str;
        float num;
    } val;
};

struct tmdf_list
{
    char *key;
    
    struct tmdf_item *items;
    int item_cnt;
    int pv_max_item_cnt;
};

struct tmdf_category
{
    char *key;
    
    struct tmdf_list *lists;
    int list_cnt;
    int pv_max_list_cnt;
};

struct tmdf_dict
{
    struct tmdf_category *cats;
    int cat_cnt;
    int pv_max_cat_cnt;
};

struct tmdf_dict tmdf_dict_create(int max_cat_cnt);
void tmdf_dict_destroy(struct tmdf_dict *dict);

void tmdf_dict_print(const struct tmdf_dict *dict); // nice tree-like way of displaying tmdf dictionary
size_t tmdf_dict_size(const struct tmdf_dict *dict);
size_t tmdf_dict_max_size(const struct tmdf_dict *dict);

void tmdf_parse_file(struct tmdf_dict *dst_dict, FILE *file);

const struct tmdf_category *tmdf_dict_category(const struct tmdf_dict *dict, const char *cat_key);
struct tmdf_category *tmdf_dict_add_category(struct tmdf_dict *dict, const char *key, int max_list_cnt);
void tmdf_dict_remove_category(struct tmdf_dict *dict, struct tmdf_category *cat);

const struct tmdf_list *tmdf_category_list(const struct tmdf_category *cat, const char *list_key);
struct tmdf_list *tmdf_category_add_list(struct tmdf_category *cat, const char *key, int max_item_cnt);
void tmdf_category_remove_list(struct tmdf_category *cat, struct tmdf_list *list);

const struct tmdf_item *tmdf_list_item(const struct tmdf_list *list, const char *item_key);
struct tmdf_item *tmdf_list_add_item_s(struct tmdf_list *list, const char *key, uint32_t flags, const char *str);
struct tmdf_item *tmdf_list_add_item_n(struct tmdf_list *list, const char *key, uint32_t flags, float num);
void tmdf_list_remove_item(struct tmdf_list *list, struct tmdf_item *item);

#endif
