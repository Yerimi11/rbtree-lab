#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    x->parent->left = y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    x->parent->right = y;
  }
  else if (x == x->parent->right){
    x->parent->right = y;
  }
  else {
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = malloc(sizeof(node_t));
  node_t *x = t->root;
  node_t *y = t->nil;
  new_node->key = key;

  while (x != t->nil){
    y = x;
    if (new_node->key < y->key){
      x = x->left;
    }
    else {
      x = x->right;
    }
  }
  new_node->parent = y;
  if (y == t->nil){
    t->root = new_node;
  }
  else if (new_node->key < y->key){
    y->left = new_node;
  }
  else {
    y->right = new_node;
    }
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;

  // RB_INSERT_FIXUP()
  while (new_node->parent->color == RBTREE_RED){
    node_t *tmp
    if (new_node->parent == new_node->parent->parent->left){
      tmp = new_node->parent->parent->right; // uncle 노드

      // case 1
      // new_node, parent, uncle(tmp) 셋 다 RED이면
      if (tmp->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        tmp->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        // 조부모를 새로운 new_node로 두고 while루프 돌면서 색 바꿔줌
        new_node = new_node->parent->parent;
      }
      // case 2
      else { 
        if (new_node == new_node->parent->right){
          new_node = new_node->parent;
          left_rotate(t, new_node);
        }
      // case 3
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, new_node->parent->parent);
      }
    }
    else {
      tmp = new_node->parent->parent->left;

        // case 1
        if (tmp->color == RBTREE_RED){
          new_node->parent->color = RBTREE_BLACK;
          tmp->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          new_node = new_node->parent->parent;
        }
        // case 2
        else {
          if (new_node == new_node->parent->left){
            new_node = new_node->parent;
            right_rotate(t, new_node);
          }
        // case 3
          new_node->parent->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          left_rotate(t, new_node->parent->parent);
        }
      }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while (x->right != t->nil){
    x = x->right;
  }
  return x;
}

void RB_TRNASPLANT(rbtree *t, node_t *u, node_t *v){
  v->parent = u->parent;
  // root
  if (u->parent == t->nil){
    t->root = v;
  }
  // left
  else if (u == u->parent->left){
    u->parent->left = v;
  }
  // right (u->parent->right == u 생략)
  else {
    u->parent->right = v;
  }
  // v->parent = u->parent;
}

void RB_ERASE_FIXUP(rbtree *t, node_t, *target){
  node_t *u;
  // fix가 시작되는 조건
  while(target != t->root && target->color == RBTREE_BLACK){
    //기준이되는 노드가 왼쪽일 때
    if(target->parent->left == target){
      u = target->parent->right;
      //case1: uncle is red
      if(u->color == RBTREE_RED){
        //형제를 검은색으로 부모를 빨간색으로 칠한다. 부모노드를 기준으로 좌회전한다.
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        left_lotate(t, target->parent);
        u = target->parent->right;
      }
      //case2: uncle is black with two black child
      if(u->left->color == RBTREE_BLACK && u->right->color == RBTREE_BLACK){
        //형제노드만 빨간색으로 만들고 타겟을 부모로 변경한다.
        u->color = RBTREE_RED;
        target = target->parent;
      }      
      else { // 자식 중 최소 한개는 빨간색이다.
        //case3: uncle is black with red color left child and black color right child
        if(u->right->color == RBTREE_BLACK){
        // 형제 노드를 빨간색으로, 형제 노드의 왼쪽 자식을 검은색으로 칠하고 형제노드를 기준으로 우회전한다.
          u->color = RBTREE_RED;
          u->left->color = RBTREE_BLACK;
          right_lotate(t, u);
          u = target->parent->right;
        }
        //case4: uncle is black with red color right child
        // 부모 노드의 색을 형제에게 넘긴다. 부모노드와 형제 노드의 오른쪽 자식을 검은색으로 칠한다. 부모 노드 기준으로 좌회전한다.
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->right->color = RBTREE_BLACK;
        left_lotate(t, target->parent);
      }
    }
    //기준이 되는 노드가 오른쪽 일때
    else{
      if(target->parent->right == target){
      u = target->parent->left;
      //case1: uncle is red
      if(u->color == RBTREE_RED){
        //형제를 검은색으로 부모를 빨간색으로 칠한다. 부모노드를 기준으로 좌회전한다.
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        left_lotate(t, target->parent);
        u = target->parent->left;
      }
      //case2: uncle is black with two black child
      if(u->right->color == RBTREE_BLACK && u->left->color == RBTREE_BLACK){
        //형제노드만 빨간색으로 만들고 타겟을 부모로 변경한다.
        u->color = RBTREE_RED;
        target = target->parent;
      }      
      else { // 자식 중 최소 한개는 빨간색이다.
        //case3: uncle is black with red color right child and black color left child
        if(u->left->color == RBTREE_BLACK){
        // 형제 노드를 빨간색으로, 형제 노드의 왼쪽 자식을 검은색으로 칠하고 형제노드를 기준으로 우회전한다.
          u->color = RBTREE_RED;
          u->right->color = RBTREE_BLACK;
          right_lotate(t, u);
          u = target->parent->left;
        }
        //case4: uncle is black with red color left child
        // 부모 노드의 색을 형제에게 넘긴다. 부모노드와 형제 노드의 오른쪽 자식을 검은색으로 칠한다. 부모 노드 기준으로 좌회전한다.
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->left->color = RBTREE_BLACK;
        left_lotate(t, target->parent);
      }
    }
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *y= p;
  node_t *ptr; // fixed_up의 기준이 될 노드
  color_t y_original_color = p->color;

  // case1 : 왼쪽 노드가 nil일 때
  if (parent->left == t->nil){
    ptr = y->right;
    RB_TRNASPLANT(t, y, y->right);
  }
  // case2 : 오른쪽 노드가 nil일 때
  else if (parent->right == t->nil){
    ptr = y->left;
    RB_TRNASPLANT(t, y, y->left);
  }
  // case3 : 왼쪽, 오른쪽 둘 다 노드일 때 (not nil)
  else {
    // y는 p.r을 루트로 하는 서브 트리의 최소키를 가지는 노드
    y = rbtree_min(t, p->right);
    // ptr을 y->right로 바꿔주고 color를 위에서 찾은 y의 color로 변경시켜 준다.
    y_original_color = y->color;
    ptr = y->right;
    // case1 : p의 자식이 y일 때
    if (y->parent == p){
      ptr->parent = y; // fixup 후 y가 nil이 됐을 수도 있는데 nil은 p를 가리킬 수 없으니 ptr가 부모를 알아야 하는 상황에서 알 수 없어서 정해줌
    }
    else {
      // 그 외 케이스 : y의 부모노드를 y->right와 연결시킨다.
      RB_TRNASPLANT(t, t, y->right){
        // p의 오른쪽 노드와 y를 연결시킨다.
        p->right->right = y;
        y->right = p->right;
      }
      // p의 부모노드를 y와 연결시킨다.
      RB_TRNASPLANT(t, p, y);
      // p의 왼쪽노드를 y와 연결시킨다.
      y->left = p->left;
      p->left->parent = y;
      y->color = p->color;
    }
  if (y_original_color == RBTREE_BLACK){
    RB_ERASE_FIXUP(t, ptr);
  }
  free(p);
  return 0;


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
int main(){
  return 0;
}
