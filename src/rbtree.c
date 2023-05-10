#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t*)calloc(1,sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p -> nil = nil;
  p -> root = nil;
  return p;
}


void left_rotate(rbtree *t, node_t* x){
  node_t *y=x->right;
  x->right = y->left;
  if(y->left != t->nil) y->left->parent=x;
  y->parent = x-> parent; 
  if(x->parent == t->nil) t->root=y;
  else if(x==x->parent->left) x->parent->left = y;
  else x->parent->right = y;
  y->left=x;
  x->parent=y;
}

void right_rotate(rbtree *t, node_t* x){
  node_t *y=x->left;
  x->left = y -> right;
  if(y->right != t->nil) y->right->parent=x;
  y->parent = x -> parent;
  if(x->parent == t->nil) t->root =y;
  else if(x==x->parent->right) x->parent->right =y;
  else  x->parent->left = y;
  y->right=x;
  x->parent=y;
}

void delete_node(rbtree *t, node_t *p){
  if(p != t->nil){
    delete_node(t, p->left);
    delete_node(t, p->right);
    free(p);
    p = NULL;
  }
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t,t->root);
  free(t->nil);
  t->nil = NULL;
  free(t);
  t = NULL;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  node_t *y;
  // 삽입된 노드의 부모가 RED
  while(z->parent->color==RBTREE_RED){
    // case1 : 삽입노드의 부모노드가 할아버지노드의 왼쪽에 있을 경우
    if(z->parent==z->parent->parent->left){
      y=z->parent->parent->right;
      // 삽입노드의 삼촌노드도 RED
      if(y->color==RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z=z->parent->parent;
      }
      // 삼촌노드는 BLACK
      else 
      {
        //삽입노드가 부모의 오른쪽에 있을 경우(꺾여서 펴줘야함)
        if(z==z->parent->right){
          z=z->parent;
          left_rotate(t,z);
        }
        //삽입노드가 부모의 왼쪽에 있을 경우(펴줄필요 없이 right_rotate)
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    // case2 : 삽입노드의 부모노드가 할아버지노드의 오른쪽에 있을 경우
    else {
      y=z->parent->parent->left;
      // 삽입노드의 삼촌노드도 RED
      if(y->color==RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z=z->parent->parent;
      }
      // 삼촌노드는 BLACK
      else 
      {
        //삽입노드가 부모의 왼쪽에 있을 경우(꺾여서 펴줘야함)
        if(z==z->parent->left){
          z=z->parent;
          right_rotate(t,z);
        }
        //삽입노드가 부모의 오른쪽에 있을 경우(펴줄필요 없이 left_rotate)
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color=RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    } 
  }
  t->root->color=RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // y는 부모노드, x는 루트노드
  node_t *y = t -> nil;
  node_t *x = t -> root;
  
  node_t *newnode=(node_t *)malloc(sizeof(node_t));
  newnode->color = RBTREE_RED;
  newnode->key = key;
  newnode->left = t->nil;
  newnode->right = t->nil;
  while (x!=(t->nil)){
    y=x;
    if (key<(x->key)) x=x->left;
    else x=x->right;
  }
  newnode->parent = y;
  if (y==t->nil){
    t->root = newnode;
  } else if(key<(y->key)){
    y->left=newnode;
  } else {
    y->right=newnode;
  }
  newnode ->left = t->nil;
  newnode -> right = t->nil;
  newnode -> color = RBTREE_RED;
  rbtree_insert_fixup(t, newnode);
  return newnode;
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x=t->root;
  while(x!=(t->nil)){
    if(key==(x->key)){
      return x;
    }else if(key<(x->key)){
      x=x->left;
    }else{
      x=x->right;
    }
  }
  return NULL;
}
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x=t->root;
  if(x==t->nil) return NULL;
  while (x->left != t->nil) x=x->left;
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x=t->root;
  if(x==t->nil) return NULL;
  while(x->right != t->nil) x=x->right;
  return x;
}

node_t *right_successor(rbtree *t, node_t *x){
  while(x->left!=t->nil) x=x->left; 
  return x;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t* v){
  if(u->parent==t->nil) t->root=v;
  else if(u==u->parent->left) u->parent->left=v;
  else u->parent->right=v;
  v->parent=u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *x){
  node_t *w; // x노드의 형제노드
  while (x!=t->root && x->color == RBTREE_BLACK){
    if (x==x->parent->left){
      w=x->parent->right;
      if(w->color==RBTREE_RED){
        w->color=RBTREE_BLACK;
        x->parent->color=RBTREE_RED;
        left_rotate(t,x->parent);
        w=x->parent->right;
      }
      if(w->left->color==RBTREE_BLACK && w->right->color==RBTREE_BLACK){
        w->color=RBTREE_RED;
        x=x->parent;
      }
      else {
        if(w->right->color == RBTREE_BLACK){
          w->left->color =RBTREE_BLACK;
          w->color=RBTREE_RED;
          right_rotate(t,w);
          w=x->parent->right;
        }
        w->color=x->parent->color;
        x->parent->color=RBTREE_BLACK;
        w->right->color=RBTREE_BLACK;
        left_rotate(t,x->parent);
        x=t->root;
      }
    }
    else {
      w=x->parent->left;
      if(w->color==RBTREE_RED){
        w->color=RBTREE_BLACK;
        x->parent->color=RBTREE_RED;
        right_rotate(t,x->parent);
        w=x->parent->left;
      }
      if(w->right->color==RBTREE_BLACK && w->left->color==RBTREE_BLACK){
        w->color=RBTREE_RED;
        x=x->parent;
      }
      else {
        if(w->left->color == RBTREE_BLACK){
          w->right->color =RBTREE_BLACK;
          w->color=RBTREE_RED;
          left_rotate(t,w);
          w=x->parent->left;
        }
        w->color=x->parent->color;
        x->parent->color=RBTREE_BLACK;
        w->left->color=RBTREE_BLACK;
        right_rotate(t,x->parent);
        x=t->root;
      }
    }
  }
  x->color=RBTREE_BLACK;
}
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y= p;
  node_t *x;
  color_t y_original_color=y->color;
  if(p->left==t->nil){
    x=p->right;
    rbtree_transplant(t,p,p->right);
    free(p);
  } else if(p->right==t->nil){
    x=p->left;
    rbtree_transplant(t,p,p->left);
    free(p);
  }
  else{
    y=right_successor(t,p->right);
    y_original_color=y->color;
    x=y->right;
    if(y->parent == p)  x->parent=y;
    else {
      rbtree_transplant(t,y,y->right);
      y->right=p->right;
      y->right->parent =y;
    }
    rbtree_transplant(t,p,y);
    y->left = p->left;
    y->left->parent=y;
    y->color=p->color;
    free(p);
  }
  if(y_original_color==RBTREE_BLACK)  rbtree_erase_fixup(t,x);
  return 0;
}

void inorder(const rbtree *t, node_t * node, key_t *arr, int* i){
  if(node!=t->nil){
    inorder(t,node->left, arr,i);
    arr[*i]=node->key;
    (*i)++;
    inorder(t, node->right,arr,i);
  }
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  // 중위순회
  int i=0;
  inorder(t,t->root, arr,&i);
  return 0;
}
// void print_rbtree(const rbtree *t, const node_t *x) {
//   if (x == t->nil) {
//     printf("nil\n");
//     return;
//   }
//   printf("key = %d\n", x->key);
//   printf("left: ");
//   print_rbtree(t, x->left);
//   printf("right: ");
//   print_rbtree(t, x->right);
// }



