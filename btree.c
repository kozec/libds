/*
 * The MIT License (MIT)
 * Copyright (c) 2015 Peter Vanusanik <admin@en-circle.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the 
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * btree.c
 *  Created on: Jan 3, 2016
 *      Author: Peter Vanusanik
 *  Contents: content copied and modified from
 *      http://web.mit.edu/~emin/Desktop/ref_to_emin/www.old/source_code/red_black_tree/index.html
 */

#include "btree.h"

/***********************************************************************/
/*  FUNCTION:  rb_create_tree */
/**/
/*  INPUTS:  All the inputs are names of functions.  CompFunc takes to */
/*  void pointers to keys and returns 1 if the first arguement is */
/*  "greater than" the second.   DestFunc takes a pointer to a key and */
/*  destroys it in the appropriate manner when the node containing that */
/*  key is deleted.  InfoDestFunc is similiar to DestFunc except it */
/*  recieves a pointer to the info of a node and destroys it. */
/*  PrintFunc recieves a pointer to the key of a node and prints it. */
/*  PrintInfo recieves a pointer to the info of a node and prints it. */
/*  If RBTreePrint is never called the print functions don't have to be */
/*  defined and NullFunction can be used.  */
/**/
/*  OUTPUT:  This function returns a pointer to the newly created */
/*  red-black tree. */
/**/
/*  Modifies Input: none */
/***********************************************************************/

rb_tree_t* rb_create_tree(cmp_func_t CompFunc,
                  key_deallocator_func_t DestFunc,
                  value_deallocator_func_t InfoDestFunc) {
  rb_tree_t* newTree;
  rb_node_t* temp;

  newTree=(rb_tree_t*) malloc(sizeof(rb_tree_t));
  newTree->cmp_func=  CompFunc;
  newTree->key_deallocator_func= DestFunc;
  newTree->value_deallocator_func= InfoDestFunc;

  /*  see the comment in the rb_red_blk_tree structure in red_black_tree.h */
  /*  for information on nil and root */
  temp=newTree->nil= (rb_node_t*) malloc(sizeof(rb_node_t));
  temp->parent=temp->left=temp->right=temp;
  temp->red=0;
  temp->key=0;
  temp=newTree->root= (rb_node_t*) malloc(sizeof(rb_node_t));
  temp->parent=temp->left=temp->right=newTree->nil;
  temp->key=0;
  temp->red=0;
  return(newTree);
}

/***********************************************************************/
/*  FUNCTION:  left_rotate */
/**/
/*  INPUTS:  This takes a tree so that it can access the appropriate */
/*           root and nil pointers, and the node to rotate on. */
/**/
/*  OUTPUT:  None */
/**/
/*  Modifies Input: tree, x */
/**/
/*  EFFECTS:  Rotates as described in _Introduction_To_Algorithms by */
/*            Cormen, Leiserson, Rivest (Chapter 14).  Basically this */
/*            makes the parent of x be to the left of x, x the parent of */
/*            its parent before the rotation and fixes other pointers */
/*            accordingly. */
/***********************************************************************/

static void left_rotate(rb_tree_t* tree, rb_node_t* x) {
  rb_node_t* y;
  rb_node_t* nil=tree->nil;

  /*  I originally wrote this function to use the sentinel for */
  /*  nil to avoid checking for nil.  However this introduces a */
  /*  very subtle bug because sometimes this function modifies */
  /*  the parent pointer of nil.  This can be a problem if a */
  /*  function which calls LeftRotate also uses the nil sentinel */
  /*  and expects the nil sentinel's parent pointer to be unchanged */
  /*  after calling this function.  For example, when RBDeleteFixUP */
  /*  calls LeftRotate it expects the parent pointer of nil to be */
  /*  unchanged. */

  y=x->right;
  x->right=y->left;

  if (y->left != nil) y->left->parent=x; /* used to use sentinel here */
  /* and do an unconditional assignment instead of testing for nil */

  y->parent=x->parent;

  /* instead of checking if x->parent is the root as in the book, we */
  /* count on the root sentinel to implicitly take care of this case */
  if( x == x->parent->left) {
    x->parent->left=y;
  } else {
    x->parent->right=y;
  }
  y->left=x;
  x->parent=y;

}


/***********************************************************************/
/*  FUNCTION:  right_rotate */
/**/
/*  INPUTS:  This takes a tree so that it can access the appropriate */
/*           root and nil pointers, and the node to rotate on. */
/**/
/*  OUTPUT:  None */
/**/
/*  Modifies Input?: tree, y */
/**/
/*  EFFECTS:  Rotates as described in _Introduction_To_Algorithms by */
/*            Cormen, Leiserson, Rivest (Chapter 14).  Basically this */
/*            makes the parent of x be to the left of x, x the parent of */
/*            its parent before the rotation and fixes other pointers */
/*            accordingly. */
/***********************************************************************/

static void right_rotate(rb_tree_t* tree, rb_node_t* y) {
  rb_node_t* x;
  rb_node_t* nil=tree->nil;

  /*  I originally wrote this function to use the sentinel for */
  /*  nil to avoid checking for nil.  However this introduces a */
  /*  very subtle bug because sometimes this function modifies */
  /*  the parent pointer of nil.  This can be a problem if a */
  /*  function which calls LeftRotate also uses the nil sentinel */
  /*  and expects the nil sentinel's parent pointer to be unchanged */
  /*  after calling this function.  For example, when RBDeleteFixUP */
  /*  calls LeftRotate it expects the parent pointer of nil to be */
  /*  unchanged. */

  x=y->left;
  y->left=x->right;

  if (nil != x->right)  x->right->parent=y; /*used to use sentinel here */
  /* and do an unconditional assignment instead of testing for nil */

  /* instead of checking if x->parent is the root as in the book, we */
  /* count on the root sentinel to implicitly take care of this case */
  x->parent=y->parent;
  if( y == y->parent->left) {
    y->parent->left=x;
  } else {
    y->parent->right=x;
  }
  x->right=y;
  y->parent=x;
}

/***********************************************************************/
/*  FUNCTION:  tree_insert_help  */
/**/
/*  INPUTS:  tree is the tree to insert into and z is the node to insert */
/**/
/*  OUTPUT:  none */
/**/
/*  Modifies Input:  tree, z */
/**/
/*  EFFECTS:  Inserts z into the tree as if it were a regular binary tree */
/*            using the algorithm described in _Introduction_To_Algorithms_ */
/*            by Cormen et al.  This function is only intended to be called */
/*            by the rb_insert function and not by the user */
/***********************************************************************/

static void tree_insert_help(rb_tree_t* tree, rb_node_t* z) {
  /*  This function should only be called by rb_insert (see above) */
  rb_node_t* x;
  rb_node_t* y;
  rb_node_t* nil=tree->nil;

  z->left=z->right=nil;
  y=tree->root;
  x=tree->root->left;
  while( x != nil) {
    y=x;
    if (1 == tree->cmp_func(x->key,z->key)) { /* x.key > z.key */
      x=x->left;
    } else { /* x,key <= z.key */
      x=x->right;
    }
  }
  z->parent=y;
  if ( (y == tree->root) ||
       (1 == tree->cmp_func(y->key,z->key))) { /* y.key > z.key */
    y->left=z;
  } else {
    y->right=z;
  }
}

/*  Before calling Insert RBTree the node x should have its key set */

/***********************************************************************/
/*  FUNCTION:  rb_insert */
/**/
/*  INPUTS:  tree is the red-black tree to insert a node which has a key */
/*           pointed to by key and info pointed to by info.  */
/**/
/*  OUTPUT:  This function returns a pointer to the newly inserted node */
/*           which is guarunteed to be valid until this node is deleted. */
/*           What this means is if another data structure stores this */
/*           pointer then the tree does not need to be searched when this */
/*           is to be deleted. */
/**/
/*  Modifies Input: tree */
/**/
/*  EFFECTS:  Creates a node node which contains the appropriate key and */
/*            info pointers and inserts it into the tree. */
/***********************************************************************/

rb_node_t * rb_insert(rb_tree_t* tree, void* key, void* info) {
  rb_node_t * y;
  rb_node_t * x;
  rb_node_t * newNode;

  x=(rb_node_t*) malloc(sizeof(rb_node_t));
  x->key=key;
  x->info=info;

  tree_insert_help(tree,x);
  newNode=x;
  x->red=1;
  while(x->parent->red) { /* use sentinel instead of checking for root */
    if (x->parent == x->parent->parent->left) {
      y=x->parent->parent->right;
      if (y->red) {
    x->parent->red=0;
    y->red=0;
    x->parent->parent->red=1;
    x=x->parent->parent;
      } else {
    if (x == x->parent->right) {
      x=x->parent;
      left_rotate(tree,x);
    }
    x->parent->red=0;
    x->parent->parent->red=1;
    right_rotate(tree,x->parent->parent);
      }
    } else { /* case for x->parent == x->parent->parent->right */
      y=x->parent->parent->left;
      if (y->red) {
    x->parent->red=0;
    y->red=0;
    x->parent->parent->red=1;
    x=x->parent->parent;
      } else {
    if (x == x->parent->left) {
      x=x->parent;
      right_rotate(tree,x);
    }
    x->parent->red=0;
    x->parent->parent->red=1;
    left_rotate(tree,x->parent->parent);
      }
    }
  }
  tree->root->left->red=0;
  return(newNode);
}

/***********************************************************************/
/*  FUNCTION:  tree_successor  */
/**/
/*    INPUTS:  tree is the tree in question, and x is the node we want the */
/*             the successor of. */
/**/
/*    OUTPUT:  This function returns the successor of x or NULL if no */
/*             successor exists. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:  uses the algorithm in _Introduction_To_Algorithms_ */
/***********************************************************************/

rb_node_t* tree_successor(rb_tree_t* tree, rb_node_t* x) {
  rb_node_t* y;
  rb_node_t* nil=tree->nil;
  rb_node_t* root=tree->root;

  if (nil != (y = x->right)) { /* assignment to y is intentional */
    while(y->left != nil) { /* returns the minium of the right subtree of x */
      y=y->left;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->right) { /* sentinel used instead of checking for nil */
      x=y;
      y=y->parent;
    }
    if (y == root) return(nil);
    return(y);
  }
}

/***********************************************************************/
/*  FUNCTION:  tree_predecesor  */
/**/
/*    INPUTS:  tree is the tree in question, and x is the node we want the */
/*             the predecessor of. */
/**/
/*    OUTPUT:  This function returns the predecessor of x or NULL if no */
/*             predecessor exists. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:  uses the algorithm in _Introduction_To_Algorithms_ */
/***********************************************************************/

rb_node_t* tree_predecesor(rb_tree_t* tree, rb_node_t* x) {
  rb_node_t* y;
  rb_node_t* nil=tree->nil;
  rb_node_t* root=tree->root;

  if (nil != (y = x->left)) { /* assignment to y is intentional */
    while(y->right != nil) { /* returns the maximum of the left subtree of x */
      y=y->right;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->left) {
      if (y == root) return(nil);
      x=y;
      y=y->parent;
    }
    return(y);
  }
}


/***********************************************************************/
/*  FUNCTION:  tree_dest_helper */
/**/
/*    INPUTS:  tree is the tree to destroy and x is the current node */
/**/
/*    OUTPUT:  none  */
/**/
/*    EFFECTS:  This function recursively destroys the nodes of the tree */
/*              postorder using the destroy_key and destroy_value functions. */
/**/
/*    Modifies Input: tree, x */
/**/
/*    Note:    This function should only be called by rb_destroy */
/***********************************************************************/

void tree_dest_helper(rb_tree_t* tree, rb_node_t* x) {
  rb_node_t* nil=tree->nil;
  if (x != nil) {
    tree_dest_helper(tree,x->left);
    tree_dest_helper(tree,x->right);
    tree->key_deallocator_func(x->key);
    tree->value_deallocator_func(x->info);
    free(x);
  }
}


/***********************************************************************/
/*  FUNCTION:  rb_destroy */
/**/
/*    INPUTS:  tree is the tree to destroy */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Destroys the key and frees memory */
/**/
/*    Modifies Input: tree */
/**/
/***********************************************************************/

void rb_destroy(rb_tree_t* tree) {
  tree_dest_helper(tree,tree->root->left);
  free(tree->root);
  free(tree->nil);
  free(tree);
}


/***********************************************************************/
/*  FUNCTION:  tree_find */
/**/
/*    INPUTS:  tree is the tree to print and q is a pointer to the key */
/*             we are searching for */
/**/
/*    OUTPUT:  returns the a node with key equal to q.  If there are */
/*             multiple nodes with key equal to q this function returns */
/*             the one highest in the tree */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/

rb_node_t* tree_find(rb_tree_t* tree, void* q) {
  rb_node_t* x=tree->root->left;
  rb_node_t* nil=tree->nil;
  int compVal;
  if (x == nil) return(0);
  compVal=tree->cmp_func(x->key,(int*) q);
  while(0 != compVal) {/*assignemnt*/
    if (1 == compVal) { /* x->key > q */
      x=x->left;
    } else {
      x=x->right;
    }
    if ( x == nil) return(0);
    compVal=tree->cmp_func(x->key,(int*) q);
  }
  return(x);
}


/***********************************************************************/
/*  FUNCTION:  rb_delete_fixup */
/**/
/*    INPUTS:  tree is the tree to fix and x is the child of the spliced */
/*             out node in rb_delete. */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Performs rotations and changes colors to restore red-black */
/*             properties after a node is deleted */
/**/
/*    Modifies Input: tree, x */
/**/
/*    The algorithm from this function is from _Introduction_To_Algorithms_ */
/***********************************************************************/

static void rb_delete_fixup(rb_tree_t* tree, rb_node_t* x) {
  rb_node_t* root=tree->root->left;
  rb_node_t* w;

  while( (!x->red) && (root != x)) {
    if (x == x->parent->left) {
      w=x->parent->right;
      if (w->red) {
    w->red=0;
    x->parent->red=1;
    left_rotate(tree,x->parent);
    w=x->parent->right;
      }
      if ( (!w->right->red) && (!w->left->red) ) {
    w->red=1;
    x=x->parent;
      } else {
    if (!w->right->red) {
      w->left->red=0;
      w->red=1;
      right_rotate(tree,w);
      w=x->parent->right;
    }
    w->red=x->parent->red;
    x->parent->red=0;
    w->right->red=0;
    left_rotate(tree,x->parent);
    x=root; /* this is to exit while loop */
      }
    } else { /* the code below is has left and right switched from above */
      w=x->parent->left;
      if (w->red) {
    w->red=0;
    x->parent->red=1;
    right_rotate(tree,x->parent);
    w=x->parent->left;
      }
      if ( (!w->right->red) && (!w->left->red) ) {
    w->red=1;
    x=x->parent;
      } else {
    if (!w->left->red) {
      w->right->red=0;
      w->red=1;
      left_rotate(tree,w);
      w=x->parent->left;
    }
    w->red=x->parent->red;
    x->parent->red=0;
    w->left->red=0;
    right_rotate(tree,x->parent);
    x=root; /* this is to exit while loop */
      }
    }
  }
  x->red=0;
}


/***********************************************************************/
/*  FUNCTION:  rb_delete */
/**/
/*    INPUTS:  tree is the tree to delete node z from */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Deletes z from tree and frees the key and info of z */
/*             using DestoryKey and DestoryInfo.  Then calls */
/*             rb_delete_fixup to restore red-black properties */
/**/
/*    Modifies Input: tree, z */
/**/
/*    The algorithm from this function is from _Introduction_To_Algorithms_ */
/***********************************************************************/

void rb_delete(rb_tree_t* tree, rb_node_t* z) {
  rb_node_t* y;
  rb_node_t* x;
  rb_node_t* nil=tree->nil;
  rb_node_t* root=tree->root;

  y= ((z->left == nil) || (z->right == nil)) ? z : tree_successor(tree,z);
  x= (y->left == nil) ? y->right : y->left;
  if (root == (x->parent = y->parent)) { /* assignment of y->p to x->p is intentional */
    root->left=x;
  } else {
    if (y == y->parent->left) {
      y->parent->left=x;
    } else {
      y->parent->right=x;
    }
  }
  if (y != z) { /* y should not be nil in this case */
    /* y is the node to splice out and x is its child */

    if (!(y->red)) rb_delete_fixup(tree,x);

    tree->key_deallocator_func(z->key);
    tree->value_deallocator_func(z->info);
    y->left=z->left;
    y->right=z->right;
    y->parent=z->parent;
    y->red=z->red;
    z->left->parent=z->right->parent=y;
    if (z == z->parent->left) {
      z->parent->left=y;
    } else {
      z->parent->right=y;
    }
    free(z);
  } else {
    tree->key_deallocator_func(y->key);
    tree->value_deallocator_func(y->info);
    if (!(y->red)) rb_delete_fixup(tree,x);
    free(y);
  }
}
