//
//g++ -g -O0 memerrors.cc -o memtest   //g++ for delete
//valgrind ./memtest
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void mem_out_of_bound() {
   int *p = (int*)malloc(10*sizeof(int));
   p[10] = 0;
   //free(p);
}

///mem leak start///
typedef struct _node{
   struct _node* l;
   struct _node* r;
   char v;
} node;

node *mk(node* l, node* r, char val) {
   node* f = (node*)malloc(sizeof(*f));
   f->l = l;
   f->r = r;
   f->v = val;
   return f;
}

void nodefree(node* n) {
   if(n) {
      nodefree(n->l);
      nodefree(n->r);
      free(n);
   }
}

void mem_leak() {
   //int *p = (int*)malloc(10*sizeof(int));
   node *tree1, *tree2, *tree3;
   tree1 = mk(mk(mk(0, 0, '3'), 0, '2'), 0, '1');
   tree2 = mk(0, mk(0, mk(0, 0, '6'), '5'), '4');
   tree3 = mk(mk(tree1, tree2, '8'), 0, '7');
   return;
}
///mem leak end///

void mem_uninitial() {
   int a[5];
   int i, s;
   a[0] = a[1] = a[3] = a[4] = 0;
   for (i = 0; i<5; ++i) {
      s += a[i];
   }
   if (s == 377) {
      printf("sum is %d\n", s);
   }
}

void mem_bad_access() {
   int len = 4;
   int *pt = (int*)malloc(len*sizeof(int));
   int *p = pt;
   for(int i = 0; i < len; ++i) {
      p++;
   }
   *p = 5;
   printf("the value of p is %d\n", *p);
   return;
}

void mem_overlaping() {
   char x[50];
   int i;
   for(i=0; i < 50; ++i) {
      x[i] = i+1;
   }
   strncpy(x+20, x, 20); //ok
   strncpy(x+20, x, 21); //overlap
   strncpy(x, x+20, 20); //ok
   strncpy(x, x+20, 21); //overlap

   x[39] = '\0';
   strcpy(x, x+20);      //ok

   x[39] = 39;
   x[40] = '\0';
   strcpy(x, x+20);     //overlap
}

void mem_bad_malloc() {
   int i;
   char* p = (char*)malloc(10);
   char* pt = p;
   for (i = 0; i < 10; ++i) {
      p[i] = 'z';
   }
   delete p;     //delete mismatch malloc

   pt[1] = 'x';  //invalid write
   free(pt);      //double free
   return;
}

int main(int argc, char* argv[]) {

   //mem_out_of_bound();
   //mem_leak();
   //mem_uninitial();
   //mem_bad_access();
   //mem_overlaping();
   mem_bad_malloc();
   return 0;
}

