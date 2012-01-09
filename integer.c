#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "stack.h"

void push(double i)
{
  p1++;
  if(p1 == (tos+SIZE)) {
    printf("Stack Overflow.\n");
    exit(1);
  }
  *p1 = i;
}

double pop(void)
{
  if(p1 == tos) {
    printf("Stack Underflow.\n");
    exit(1);
  }
  p1--;
  return *(p1+1);
}

unsigned int  lh_strhash(void *src)
{
 int i,l;
 unsigned long ret=0;
 unsigned short *s;
 char *str = (char *)src ;

 if (str == NULL) return(0);

 l = (strlen(str)+1)/2;
 s = (unsigned short *) str;

 for (i=0; i<l; i++)
  ret^=(s[i]<<(i&0x0f));

 return(ret);
}
int equal_str(void *k1, void *k2)
{
    return  (0 == strcmp((char *)k1,(char *)k2)) ;
}

inline  struct hashentry  *hash_new(void *key, void *data)
{
  struct hashentry  *new = malloc(sizeof(struct hashentry)) ;
  new->key  = key ;
  new->data = data ;
  new->next = NULL ;
  return new ;
}

void hlist_append(struct hashentry **root, void *key, void *data)
{
        struct hashentry  *l, *pos ;
 l = hash_new(key,data) ;
        if (*root == NULL) {
          *root =  l ; 
 } else {
         for(pos= *root; pos->next != NULL ; pos = pos->next ) ;
  pos->next = l ;
 }
}
 
int hlist_update(struct hashentry *root, void *key, void *data,
   int  (*compare)(void *,void *))
{
        struct hashentry  *pos ;
        for(pos= root; pos != NULL; pos = pos->next ) {
  if ( compare(key, pos->key) ) {
   free(pos->data) ;      
   pos->data = data ;
   free(key) ;
   return 0 ;
  }
 }
        return -1  ;
}
/*
 *Desc :free hash entry
 *
*/
inline  struct hashentry *hashentry_free(struct hashentry *h)
{
 struct hashentry *next = h->next ;
 free(h->key) ;
 free(h->data) ;
 free(h) ;
 h=NULL ;
 return (next) ;
}
 
int  hlist_remove(struct hashentry **root, void *key,
     int  (*compare)(void *,void *))
{
        struct hashentry  *pos ,*prev ;
 
 if  (NULL == *root) return -1 ;
 /* the root */
        if (compare((*root)->key, key)) {
         *root =  hashentry_free(*root) ;
  return 0 ;
 }
 /* the second */
 prev = *root ;
 for (pos = prev->next; NULL != pos;  pos = pos->next) {
         if (compare(pos->key, key)) {
          prev->next = hashentry_free(pos);
   return 0 ;
  }
  prev =pos ;
 }  ;
 return -1 ;
}
 

struct hashtable  *hash_create(unsigned int  (*keyfunc)(void *),
   int  (*comparefunc)(void *,void *),
   int size)
{
 int len = sizeof(struct hashentry *) * size  ;   
 int i ;
 struct hashtable *tab = malloc( sizeof(struct hashtable) );
 memset(tab,0, sizeof(struct hashtable)) ;
 tab->hashlist = malloc(len) ;
 if (tab->hashlist == NULL) {
  free(tab) ;
  return NULL;
 }
 memset(tab->hashlist, 0, len ) ;
 for (i=0; i<size ;i++) tab->hashlist[i] = NULL ;
 tab->compare = comparefunc ;
 tab->gethash = keyfunc ;
 tab->hashsize = size ;
 tab->count = 0 ;
 return tab ;
}

void   hash_free(struct hashtable *tab)
{
 int i ;
 struct hashentry *pos;
 
 for (i=0; i< tab->hashsize ; i++)
  for  (pos = tab->hashlist[ i ] ; NULL != pos; pos = hashentry_free(pos)) ;
 free(tab->hashlist) ;
 free(tab) ;
 tab =NULL ;
}

void hash_insert(void *key, void *data, struct hashtable *tab)
{ 
 unsigned int index = hashindex(key,tab) ;
 struct hashentry *root = tab->hashlist[ index ] ;
 if  ( hlist_update(root, key, data,  tab->compare )   != 0 ) {
          hlist_append(&(tab->hashlist[ index ]),  key, data )  ;
  tab->count ++ ;
 }
}

void hash_remove(void *key, struct hashtable *tab)
{
 unsigned int index = hashindex(key,tab) ;
 if (hlist_remove(&(tab->hashlist[ index ]), key,  tab->compare) ==0) {
         tab->count -- ;
 }
}
 

void *hash_value(void *key, struct hashtable *tab)
{
 struct hashentry *pos ;
 unsigned int index = hashindex(key, tab) ;

 for (pos = tab->hashlist[ index ];  NULL != pos; pos = pos->next) {
  if  (tab->compare(key, pos->key)) {
   return (pos->data) ;
  }
 }
 return NULL ;
}

void hash_for_each_do (struct hashtable *tab, int (cb)(void *, void *))
{
 int i =0 ;
 struct hashentry *pos ;
 for (i=0; i < tab->hashsize; i++) {
  for  (pos = tab->hashlist[ i ] ; NULL != pos; pos=pos->next )   {
   cb(pos->key, pos->data) ;
  }
 }
}

int input_var(void *key, void *data)
{
	printf("The value of %s is %s\n", key, data);
}

inline int  hash_count(struct hashtable *tab)
{
 return tab->count ;
}

char checksign(char *str)
{
	if (!strcmp(str, "+"))
	{
		return '+';
	}
	else if (!strcmp(str, "-"))
	{
		return '-';
	}
	else if (!strcmp(str, "*"))
	{
		return '*';
	}
	else if (!strcmp(str, "/"))
	{
		return '/';
	}

	return '0';

}

int main(void)
{
    char cmdline[255];
    char op[255], cvalue[255];
    char *pch;
	char symbol_table[255][255];
	int csymbol=0;
	int n_v = 0;
	int count=0;
	int code=1;

	tos = stack;
  	p1 = stack;

    struct hashtable *tab = create_hashtable(255);

    while (1)
    {
		printf(">");
		gets(cmdline);
		pch = strtok(cmdline, " ");
		count=0;
		code=1;
 		while (pch != NULL)
  		{
	  		if (count == 0)
	  		{
				if (!strcmp("INT",pch))
					code = PROGRAM;
				else if (!strcmp("f",pch))
					code = FINDVAR;
				else if (!strcmp("=",pch))
					code = SETVAR;
				else if (!strcmp("d",pch))
					code = DELVAR;
				else if (!strcmp("p",pch))
					code = PRINTVAR;
				else if (!strcmp("q",pch))
					code = EXIT;
			else
			{
				printf("no command\n");
				break;
			}

	  		}
	 		else if (count == 1)
		 	{
			switch (code)
			{
			case FINDVAR:
			case SETVAR:
				strcpy(op, pch);
				break;			
			case DELVAR:
				hash_remove(pch, tab);
				break;
			case PROGRAM:
			case PRINTVAR:
			case EXIT:
			default:
				printf("no command\n");
				break;

			}
	 		}
	 		else if (count == 2)
	 		{
			switch (code)
			{
			case SETVAR:

				if (checksign(pch) != '0')
				{
					printf("no command");
					break;
				}

	            if (hash_value(pch, tab) == NULL)
					push(atof(pch));
                else
					push(atof(hash_value(pch, tab)));

				break;

			case FINDVAR:
			case DELVAR:
				printf("no command\n");
				break;	
			}
	 	}
	 	else
		{
		switch (code)
		{
			case SETVAR:
                if (checksign(pch) != '0')
                {
                   double v2 = pop();
				   double v1 = pop();
				   double v3 = 0;

				   switch (checksign(pch))				   
				   {
						case '+':
							v3 = (double) v1 + v2;
							push(v3);
							break;
						case '-':
							v3 = (double) v1 - v2;
							push(v3);
							break;
						case '*':
							v3 = (double) v1 * v2;
							push(v3);
							break;
						case '/':
							v3 = (double) v1 / v2;
							push(v3);
							break;

				   }
                }
                else if (hash_value(pch, tab) == NULL)
                    push(atof(pch));
                else
                    push(atof(hash_value(pch, tab)));
                break;			
			default:
				printf("no command\n");
				break;
		}
		}
 	 pch = strtok (NULL, " ");
	 count++;
  	}

    switch (code)
    {
		case PROGRAM:
			  if (count != 1) break;
			  printf("Welcome to INT!\n");
			  break;
		case FINDVAR:
			  if (count != 2) break;

              if (hash_value(op, tab) == NULL)
                {
                    printf("%s not found!\n", op);
                }
                else
                {
                    printf("The value of %s is %s\n", op, hash_value(op, tab));
                }
                break;
    	case SETVAR:
				sprintf(cvalue, "%f", pop());
				hash_insert((void *)strdup(op), (void *)strdup(cvalue), tab);
				break;
		case PRINTVAR:
				if (count != 1) break;
				hash_for_each_do(tab, input_var);
				break;

		case EXIT:
			  	if (count != 1) break;
				exit(1);	
				break;
	}
  }

  return 0;
}	

