#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct entry {
	struct list_head entries;
	int val;
};

int main() {
    struct entry *dummy = (struct entry *) malloc(sizeof(struct entry));
    struct entry *entry1 = (struct entry *) malloc(sizeof(struct entry));
    struct entry *entry2 = (struct entry *) malloc(sizeof(struct entry));
    struct entry *entry3 = (struct entry *) malloc(sizeof(struct entry));
    INIT_LIST_HEAD(&dummy->entries);
    INIT_LIST_HEAD(&entry1->entries);
    INIT_LIST_HEAD(&entry2->entries);
    INIT_LIST_HEAD(&entry3->entries);
    entry1->val = 1;
    entry2->val = 2;
    entry3->val = 3;
    
    list_add(&entry1->entries, &dummy->entries);
    list_add(&entry2->entries, &entry1->entries);
    list_add(&entry3->entries, &entry2->entries);

    struct entry *entry, *next_entry;

    list_for_each_entry_safe(entry, next_entry, &dummy->entries, entries) {
        printf("%d\n", entry->val);
        if (entry->val == 1) {
            list_del(&entry->entries);
        }
    }

    list_for_each_entry_safe(entry, next_entry, &dummy->entries, entries) {
        printf("%d\n", entry->val);
    }

    return 0;
}