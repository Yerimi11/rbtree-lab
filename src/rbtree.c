#include "rbtree.h"

#include <stdio.h>
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
    t->root = y;
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
    t->root = y;
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

void del_node(rbtree *t, node_t *n){
  if(n != t->nil){
    del_node(t, n->left);
    del_node(t, n->right);
    free(n);
  }
}

void delete_rbtree(rbtree *t){
  // TODO: reclaim the tree nodes's memory
  // t만 지우지 말고 노드들도 지우세요
  // arr처럼 하나하나 접근해서 삭제
  del_node(t, t->root);
  free(t->nil);
  free(t);
}
void RB_INSERT_FIXUP(rbtree *t, node_t *new_node){
  node_t *tmp;
  // 부모가 black / 반복문 거치지 않고 root를 black으로 변경 후 함수 종료
  while (new_node->parent->color == RBTREE_RED){
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
  RB_INSERT_FIXUP(t, new_node);
return new_node;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *tmp = t->root; // 돌아다니면서 찾음
    // tmp(이동하는 거)가 맨 끝이 아니고 && 찾을 목표 key값도 아직 아니면 while
    // 맨끝에 nil까지 가거나, 찾고자하는 key를 찾으면 종료
    while(tmp != t->nil && tmp->key != key) { 
      if (tmp->key > key) { // 찾을 키값보다 지금 위치의 키값이 더 크면
        tmp = tmp->left; // 더 작은 값을 찾아야하니까 왼쪽으로
      }
      else{
        tmp = tmp->right;
      }
    }
    
    if (tmp->key == key){ // 키값이 같을 때
        return tmp;
        }

    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while (x->left != t->nil){
    x = x->left;
  }
  return x;
}

node_t *node_min(const rbtree *t, node_t *n) {
  node_t *ptr;
  ptr = n;
  while(ptr->left != t->nil){
    ptr = ptr->left;
  }
  return ptr;
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

void RB_ERASE_FIXUP(rbtree *t, node_t *target){
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
        left_rotate(t, target->parent);
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
          right_rotate(t, u);
          u = target->parent->right;
        }
        //case4: uncle is black with red color right child
        // 부모 노드의 색을 형제에게 넘긴다. 부모노드와 형제 노드의 오른쪽 자식을 검은색으로 칠한다. 부모 노드 기준으로 좌회전한다.
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->right->color = RBTREE_BLACK;
        left_rotate(t, target->parent);
        target = t->root;
      }
    }
    //기준이 되는 노드가 오른쪽 일때
    else{
      u = target->parent->left;
      //case1: uncle is red
      if(u->color == RBTREE_RED){
        //형제를 검은색으로 부모를 빨간색으로 칠한다. 부모노드를 기준으로 좌회전한다.
        u->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        right_rotate(t, target->parent);
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
          left_rotate(t, u);
          u = target->parent->left;
        }
        //case4: uncle is black with red color left child
        // 부모 노드의 색을 형제에게 넘긴다. 부모노드와 형제 노드의 오른쪽 자식을 검은색으로 칠한다. 부모 노드 기준으로 좌회전한다.
        u->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        u->left->color = RBTREE_BLACK;
        right_rotate(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if(p == NULL){
    return 0;
  }
  node_t *y= p;
  node_t *ptr; // fixed_up의 기준이 될 노드
  color_t y_original_color = p->color;

  // case1 : 왼쪽 노드가 nil일 때
  if (p->left == t->nil){
    ptr = p->right;
    RB_TRNASPLANT(t, p, p->right);
  }
  // case2 : 오른쪽 노드가 nil일 때
  else if (p->right == t->nil){
    ptr = p->left;
    RB_TRNASPLANT(t, p, p->left);
  }
  // case3 : 왼쪽, 오른쪽 둘 다 노드일 때 (not nil)
  else {
    // y는 p.r을 루트로 하는 서브 트리의 최소키를 가지는 노드
    y = node_min(t, p->right);
    // ptr을 y->right로 바꿔주고 color를 위에서 찾은 y의 color로 변경시켜 준다.
    y_original_color = y->color;
    ptr = y->right;
    // case1 : p의 자식이 y일 때
    if (y->parent == p){
      ptr->parent = y; // fixup 후 y가 nil이 됐을 수도 있는데 nil은 p를 가리킬 수 없으니 ptr가 부모를 알아야 하는 상황에서 알 수 없어서 정해줌
    }
    else {
      // 그 외 케이스 : y의 부모노드를 y->right와 연결시킨다.
        RB_TRNASPLANT(t, y, y->right);
        // p의 오른쪽 노드와 y를 연결시킨다.
        p->right->parent = y;
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
  // p = NULL;
  return 0;
}

int inorder_array(node_t *p, key_t *arr, const rbtree *t, int i){
  if (p == t->nil) {
    return i;
  }
  i = inorder_array(p->left, arr, t, i);
  arr[i++] = p->key;
  i = inorder_array(p->right, arr, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  // int i = 0;
  node_t *tmp = t->root;
  inorder_array(tmp, arr, t, 0);
  return 0;
}
// int main(){
//   return 0;
// }

// int main(){
//   rbtree *t = new_rbtree();
//   rbtree_insert(t, 10);
//   rbtree_insert(t, 5);
//   rbtree_insert(t, 8);
//   rbtree_insert(t, 34);
//   rbtree_insert(t, 67);
//   rbtree_insert(t, 23);
//   rbtree_insert(t, 156);
//   rbtree_insert(t, 24);
//   rbtree_insert(t, 2);
//   rbtree_insert(t, 12);
//   int n = 10;
//   key_t *res = calloc(n, sizeof(key_t));
//   rbtree_to_array(t, res, n);
//   printf("[");
//   for (int i = 0; i < n; i++) {
//   printf(" %d:", res[i]);
//   node_t *node = rbtree_find(t, res[i]);
//   if(node->color == RBTREE_RED){
//     printf("RED, ");
//   }else{
//     printf("BLACK,");
//   }
//   }
//   printf("]\n");
//   node_t *min = rbtree_min(t);
//   printf("최소:%d ", min->key);
//   node_t *max = rbtree_max(t);
//   printf("최대:%d ", max->key);
//   return 0;
// }
