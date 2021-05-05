// Ibrahim Amir 10037569
// Ignatyeva Marta 10034015

/*
    make bleep_censor
    ./bleep_censor
    make bleep_censor && ./bleep_censor

*/

#include "base.h"



typedef struct Token{
    char* start;
    char* end;
}Token;

typedef struct TreeNode{
    struct TreeNode* left;
    struct TreeNode* right;
    Token* token;
}TreeNode;

Token* new_token(char* start, char* end){
    Token* t = xmalloc(sizeof(Token));
    t->start = start;
    t->end = end;
    return t;
}

void print_token(Token* t, bool censored) {
    if(t == NULL) return;

    printf("%c", *(t->start));
    for(int i = 1; i < t->end - t->start; i++){
        if(censored){
            printf("*");
        }else {
            printf("%c", *(t->start+i));
        }
    }
    printf(" ");
}

void print_tokenln(Token* t, bool censored) {
    print_token(t, censored);
    printc('\n');
}

TreeNode* new_tree_node(TreeNode* left, Token* token, TreeNode* right){
    TreeNode* tree_node = xmalloc(sizeof(TreeNode));
    tree_node->token = token;
    tree_node->left = left;
    tree_node->right = right;
    return tree_node;
}

void free_tree(TreeNode* tree){
    //todo
    if(tree != NULL){
        free_tree(tree->left);
        free_tree(tree->right);
        free(tree->token);
        free(tree);
    }
}

TreeNode* new_leaf(Token* token){
    return new_tree_node(NULL, token, NULL);
}

void print_tree(TreeNode* tree){
    if(tree == NULL){
        return;
    }
    print_tree(tree->left);
    printc(' ');
    print_token(tree->token, false);
    printc(' ');
    print_tree(tree->right);
}

int minimum(int a, int b){
    if(a > b) return b;
    if(a < b) return a;
    return a;
}

int compare_token(Token* t1, Token* t2){
    require_not_null(t1);
    require_not_null(t2);
    
    int t1_length = t1->end - t1->start;
    int t2_length = t2->end - t2->start;
    
    //AABBB
    //AABB

    int min = minimum(t1_length, t2_length);

    for(int i = 0; i < min; i++){
        char t1_c = tolower(*(t1->start+i));
        char t2_c = tolower(*(t2->start+i));
       
        if(t1_c < t2_c){
            return -1;
        } else if(t1_c > t2_c) {
            return 1;
        }
    }

    if(t1_length < t2_length){
        return -1;
    }else if(t1_length > t2_length){
        return 1;
    }

    return 0;
}

void test_token_compare(){
    char* s = "hallo huhu hall HALLO";
    
    Token hallo = {s + 0, s + 5};
    print_tokenln(&hallo, true);
    Token huhu = {s + 6, s + 10};
    print_tokenln(&huhu, false);
    Token hall = {s + 11, s + 15};
    print_tokenln(&hall, true);
    Token hallo_2 = {s + 16, s + 21};
    print_tokenln(&hallo_2, false);
    
    
    test_equal_i(compare_token(&hallo, &hallo), 0);
    test_equal_i(compare_token(&huhu, &huhu), 0);
    test_equal_i(compare_token(&huhu, &hallo), 1);
    test_equal_i(compare_token(&hallo, &huhu), -1);
    test_equal_i(compare_token(&hall, &hallo), -1);
    test_equal_i(compare_token(&hallo, &hall), 1);
    test_equal_i(compare_token(&hallo, &hallo_2), 0);
    test_equal_i(compare_token(&hallo_2, &hallo), 0);
    
}

bool contains(TreeNode* tree, Token* token){
    if(tree == NULL){
        return false;
    }
    
    int cmp_result = compare_token(token, tree->token);
    if(cmp_result == 0){
        return true;
    }
    else if(cmp_result < 0){
        return contains(tree->left, token);
    }else{
        return contains(tree->right, token);
    }  
}


/*           Baum
        Affe       Giraffe
*/


void insert_in_tree(TreeNode** tree, Token* token){
    if(*tree == NULL){
        *tree = new_leaf(token);
    }else {
        if(compare_token(token, (*tree)->token) == -1){
            insert_in_tree(&((*tree)->left), token);
        }else if(compare_token(token, (*tree)->token) == 1){
            insert_in_tree(&((*tree)->right), token);
        }
    }
}

void test_insert_token(){
    char* s = "hallo huhu hall HALLO";
    
    Token hallo = {s + 0, s + 5};
    //print_tokenln(&hallo);
    Token huhu = {s + 6, s + 10};
    //print_tokenln(&huhu);
    Token hall = {s + 11, s + 15};
    //print_tokenln(&hall);
    
    TreeNode* tree = NULL;
    
    insert_in_tree(&tree, &hallo);
    
    test_equal_i(compare_token(tree->token, &hallo), 0);
    
    insert_in_tree(&tree, &hallo);
    test_equal_i(compare_token(tree->token, &hallo), 0);
    
    insert_in_tree(&tree, &hall);
    test_equal_i(compare_token(tree->left->token, &hall), 0);
    
    insert_in_tree(&tree, &huhu);
    test_equal_i(compare_token(tree->right->token, &huhu), 0);
    
    free(tree->left);
    free(tree->right);
    free(tree);
    
}


TreeNode* create_bleep_tree(char* bleep_words){
    require_not_null(bleep_words);

    TreeNode* tree = NULL;
    int start = 0;
    for(int i = 0; i < strlen(bleep_words); i++){
        if(bleep_words[i] == ' ' || bleep_words[i] == '\0'){
            Token* t = new_token(bleep_words + start, bleep_words + i);
            insert_in_tree(&tree, t);
            //define new start
            start = i + 1;
        }
    }
    return tree;
}

void test_create_bleep_tree(){
    char* s = "hallo huhu hall HALLO";
    
    Token hallo = {s + 0, s + 5};
    //print_tokenln(&hallo);
    Token huhu = {s + 6, s + 10};
    //print_tokenln(&huhu);
    Token hall = {s + 11, s + 15};
    //print_tokenln(&hall);
    
    TreeNode* tree = create_bleep_tree(s);
   
    test_equal_i(compare_token(tree->token, &hallo), 0);  
    test_equal_i(compare_token(tree->left->token, &hall), 0); 
    test_equal_i(compare_token(tree->right->token, &huhu), 0);
    
    free_tree(tree);
}

int main(void){    
    report_memory_leaks(true);
    test_token_compare();
    test_insert_token(); 
    test_create_bleep_tree();
    
    char* bleep_words = "schwein affe dumm bloed bloedmann idiot ";    
    TreeNode* tree = create_bleep_tree(bleep_words);
    
    print_tree(tree);
    printc('\n');
    
    char buffer[256];
    get_line(buffer, 256);
    printf("Original String: %s\n", buffer);
    //Der Affe sitzt im Baum

    printf("Bleeped String: ", buffer);
    //Der A*** sitzt im Baum

    int start = 0;
    for(int i = 0; i <= strlen(buffer); i++){
        if(buffer[i] == ' ' || buffer[i] == '\0'){
            //printf("#found token# ");
            Token t = {buffer + start, buffer + i};
            //define new start
            start = i + 1; 
            bool consored = contains(tree, &t);
            print_token(&t, consored);
        }
    }
    printf("\n");

    
    free_tree(tree);
    return 0;
}