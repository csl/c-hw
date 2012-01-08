
#define TABLE_SIZE 128;
//hash table

#ifndef __USE_ISOC99
#define inline
#endif

#define FINDVAR 1
#define SETVAR 2
#define DELVAR 3
#define PRINTVAR 4
#define EXIT 5

struct hashentry
{
 void *key;
 void *data;
 struct hashentry *next;
};

struct hashtable
{
 unsigned int  (*gethash)(void *);
 int  (*compare)(void *,void *);
 int     hashsize ;
 int     count ;
 struct hashentry    **hashlist  ;  /* store data */
};

#define create_hashtable(hsize) hash_create(lh_strhash, equal_str, hsize)
#define  hashindex(key, tab)  ((tab->gethash) (key) % (tab->hashsize -1))

unsigned int  lh_strhash(void *src) ;

int equal_str(void *k1, void *k2) ;

struct hashtable* hash_create(unsigned int  (*keyfunc)(void *),
   int  (*comparefunc)(void *,void *),
   int size) ;

void hash_free(struct hashtable *tab) ;
void hash_insert(void *key, void *data, struct hashtable *tab) ;

void hash_remove(void *key, struct hashtable *tab) ;
void *hash_value(void *key, struct hashtable *tab) ;
void hash_for_each_do (struct hashtable *tab, int (cb)(void *, void *))  ;
int  hash_count(struct hashtable *tab)  ;
