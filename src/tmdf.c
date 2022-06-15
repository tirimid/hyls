#include "tmdf.h"

#include "util.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

struct tmdf_dict tmdf_dict_create(int max_cat_cnt)
{
    return (struct tmdf_dict)
    {
        .cat_cnt        = 0,
        .pv_max_cat_cnt = max_cat_cnt,
        .cats           = malloc(sizeof(struct tmdf_category) * max_cat_cnt),
    };
}

void tmdf_dict_destroy(struct tmdf_dict *dict)
{
    for (int i = 0; i < dict->cat_cnt; ++i)
        tmdf_dict_remove_category(dict, &dict->cats[i]);
    
    free(dict->cats);
}

static void print_list_item(const struct tmdf_item *item)
{
    printf("\t\t%s = ", item->key);
    switch (item->type)
    {
    case TMDF_ITEM_TYPE_STRING:
        printf("%s", item->val.str);
        break;
    case TMDF_ITEM_TYPE_NUMBER:
        printf("%f", item->val.num);
        break;
    }
    printf("\n");
}

/* nice tree-like way of displaying tmdf dictionary */
void tmdf_dict_print(const struct tmdf_dict *dict)
{
    for (int i = 0; i < dict->cat_cnt; ++i)
    {
        const struct tmdf_category *cat = &dict->cats[i];

        printf("%s:\n", cat->key);
        for (int j = 0; j < cat->list_cnt; ++j)
        {
            const struct tmdf_list *list = &cat->lists[j];

            printf("\t%s:\n", list->key);
            for (int k = 0; k < list->item_cnt; ++k)
                print_list_item(&list->items[k]);
        }
    }
}

size_t tmdf_dict_size(const struct tmdf_dict *dict)
{
    size_t total_size = sizeof(*dict) + sizeof(struct tmdf_category) * dict->cat_cnt;
    for (int i = 0; i < dict->cat_cnt; ++i)
    {
        const struct tmdf_category *cat = &dict->cats[i];
        total_size += sizeof(struct tmdf_list) * cat->list_cnt;
        for (int j = 0; j < cat->list_cnt; ++j)
            total_size += sizeof(struct tmdf_item) * cat->lists[j].item_cnt;
    }
    
    return total_size;
}

size_t tmdf_dict_max_size(const struct tmdf_dict *dict)
{
    size_t total_max_size = sizeof(*dict) + sizeof(struct tmdf_category) * dict->pv_max_cat_cnt;
    for (int i = 0; i < dict->cat_cnt; ++i)
    {
        const struct tmdf_category *cat = &dict->cats[i];
        total_max_size += sizeof(struct tmdf_list) * cat->pv_max_list_cnt;
        for (int j = 0; j < cat->list_cnt; ++j)
            total_max_size += sizeof(struct tmdf_item) * cat->lists[j].pv_max_item_cnt;
    }

    return total_max_size;
}

enum token_type
{
    TOKEN_TYPE_COLON,       /* : */
    TOKEN_TYPE_SEMICOLON,   /* ; */
    TOKEN_TYPE_EQUALS,      /* = */
    TOKEN_TYPE_BRACE_LEFT,  /* { */
    TOKEN_TYPE_BRACE_RIGHT, /* } */

    TOKEN_TYPE_KEYWORD_CATEGORY,
    TOKEN_TYPE_KEYWORD_LIST,
    TOKEN_TYPE_KEYWORD_ITEM,
    TOKEN_TYPE_KEYWORD_NUM,
    TOKEN_TYPE_KEYWORD_STR,

    TOKEN_TYPE_LITERAL_STRING,
    TOKEN_TYPE_LITERAL_NUMBER,
    TOKEN_TYPE_IDENTIFIER,
};

struct token
{
    enum token_type type;
    char *val;
};

void tmdf_parse_file(struct tmdf_dict *dst_dict, FILE *file)
{
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *src      = malloc(file_size + 1);
    src[file_size] = '\0';
    fread(src, file_size, 1, file);

    free(src);
}

const struct tmdf_category *tmdf_dict_category(const struct tmdf_dict *dict, const char *cat_key)
{
    for (int i = 0; i < dict->cat_cnt; ++i)
    {
        const struct tmdf_category *cat = &dict->cats[i];
        if (strcmp(cat->key, cat_key) == 0)
            return cat;
    }
    return NULL;
}

struct tmdf_category *tmdf_dict_add_category(struct tmdf_dict *dict, const char *key, int max_list_cnt)
{
    if (dict->cat_cnt >= dict->pv_max_cat_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d categories to tmdf dictionary", dict->pv_max_cat_cnt);

    struct tmdf_category *new_cat = &dict->cats[dict->cat_cnt];
    *new_cat = (struct tmdf_category)
    {
        .key             = malloc(strlen(key) + 1),
        .lists           = malloc(sizeof(struct tmdf_list) * max_list_cnt),
        .list_cnt        = 0,
        .pv_max_list_cnt = max_list_cnt,
    };
    strcpy(new_cat->key, key);
    
    ++dict->cat_cnt;

    return new_cat;
}

void tmdf_dict_remove_category(struct tmdf_dict *dict, struct tmdf_category *cat)
{
    uintptr_t dict_cats_start = (uintptr_t)dict->cats;
    uintptr_t dict_cats_end   = (uintptr_t)(dict->cats + dict->cat_cnt);
    if ((uintptr_t)cat < dict_cats_start || (uintptr_t)cat >= dict_cats_end)
        UTIL_PRINT_ERROR("tried removing nonexistant tmdf category");

    for (int i = 0; i < cat->list_cnt; ++i)
        tmdf_category_remove_list(cat, &cat->lists[i]);

    free(cat->key);
    free(cat->lists);

    memmove(cat, cat + 1, dict_cats_end - (uintptr_t)(cat + 1));
    --dict->cat_cnt;
}

const struct tmdf_list *tmdf_category_list(const struct tmdf_category *cat, const char *list_key)
{
    for (int i = 0; i < cat->list_cnt; ++i)
    {
        const struct tmdf_list *list = &cat->lists[i];
        if (strcmp(list->key, list_key))
            return list;
    }
    return NULL;
}

struct tmdf_list *tmdf_category_add_list(struct tmdf_category *cat, const char *key, int max_item_cnt)
{
    if (cat->list_cnt >= cat->pv_max_list_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d lists to tmdf category", cat->pv_max_list_cnt);

    struct tmdf_list *new_list = &cat->lists[cat->list_cnt];
    *new_list = (struct tmdf_list)
    {
        .key             = malloc(strlen(key) + 1),
        .items           = malloc(sizeof(struct tmdf_item) * max_item_cnt),
        .item_cnt        = 0,
        .pv_max_item_cnt = max_item_cnt,
    };
    strcpy(new_list->key, key);
    
    ++cat->list_cnt;

    return new_list;
}

void tmdf_category_remove_list(struct tmdf_category *cat, struct tmdf_list *list)
{
    uintptr_t cat_lists_start = (uintptr_t)cat->lists;
    uintptr_t cat_lists_end   = (uintptr_t)(cat->lists + cat->list_cnt);
    if ((uintptr_t)list < cat_lists_start || (uintptr_t)list >= cat_lists_end)
        UTIL_PRINT_ERROR("tried removing nonexistant tmdf list");

    for (int i = 0; i < list->item_cnt; ++i)
        tmdf_list_remove_item(list, &list->items[i]);

    free(list->key);
    free(list->items);

    memmove(list, list + 1, cat_lists_end - (uintptr_t)(list + 1));
    --cat->list_cnt;
}

const struct tmdf_item *tmdf_list_item(const struct tmdf_list *list, const char *item_key)
{
    for (int i = 0; i < list->item_cnt; ++i)
    {
        const struct tmdf_item *item = &list->items[i];
        if (strcmp(item->key, item_key))
            return item;
    }
    return NULL;
}

struct tmdf_item *tmdf_list_add_item_s(struct tmdf_list *list, const char *key, uint32_t flags, const char *str)
{
    if (list->item_cnt >= list->pv_max_item_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d items to tmdf list", list->pv_max_item_cnt);

    struct tmdf_item *new_item = &list->items[list->item_cnt];
    *new_item = (struct tmdf_item)
    {
        .key     = malloc(strlen(key) + 1),
        .flags   = flags,
        .type    = TMDF_ITEM_TYPE_STRING,
        .val.str = malloc(strlen(str) + 1),
    };
    strcpy(new_item->key, key);
    strcpy(new_item->val.str, str);
    
    ++list->item_cnt;

    return new_item;
}

struct tmdf_item *tmdf_list_add_item_n(struct tmdf_list *list, const char *key, uint32_t flags, float num)
{
    if (list->item_cnt >= list->pv_max_item_cnt)
        UTIL_PRINT_ERRORF("cannot add more than %d items to tmdf list", list->pv_max_item_cnt);

    struct tmdf_item *new_item = &list->items[list->item_cnt];
    *new_item = (struct tmdf_item)
    {
        .key     = malloc(strlen(key) + 1),
        .flags   = flags,
        .type    = TMDF_ITEM_TYPE_NUMBER,
        .val.num = num,
    };
    strcpy(new_item->key, key);
    ++list->item_cnt;

    return new_item;
}

void tmdf_list_remove_item(struct tmdf_list *list, struct tmdf_item *item)
{
    uintptr_t list_items_start = (uintptr_t)list->items;
    uintptr_t list_items_end   = (uintptr_t)(list->items + list->item_cnt);
    if ((uintptr_t)item < list_items_start || (uintptr_t)item >= list_items_end)
        UTIL_PRINT_ERROR("tried removing nonexistant tmdf item");

    free(list->key);
    if (item->type == TMDF_ITEM_TYPE_STRING)
        free(item->val.str);

    memmove(item, item + 1, list_items_end - (uintptr_t)(item + 1));
    --list->item_cnt;
}
