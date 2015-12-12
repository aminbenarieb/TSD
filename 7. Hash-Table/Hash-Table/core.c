//
//  core.c
//  lab6
//
//  Created by Amin Benarieb on 21/11/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//
#include <stdlib.h>
#include "core.h"
#include "binarytree.h"
#include "hashmap.h"

#define kInetation 100
#define kAnalyzeSize 100
#define showTree true
#define autoSearch true
#define fullSearch false

#pragma mark Other

bool is_balanced(tnode *root){
    int lh; /* for height of left subtree */
    int rh; /* for height of right subtree */
    
    /* If tree is empty then return true */
    if(root == NULL)
        return 1;
    
    /* Get the height of left and right sub trees */
    lh = avl_height(root->left);
    rh = avl_height(root->right);
    
    if( abs(lh-rh) <= 1 && is_balanced(root->left) && is_balanced(root->right))
        return 1;
    
    /* If we reach here then tree is not height-balanced */
    return 0;
}
unsigned long long tick(void){
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}
int compare_name(tfile f1, tfile f2){
    return strncmp(f1.name, f2.name, MAX_NUMBER_SIZE+1);
}

#pragma mark Analyze

int randomRange(int t1, int t2){
    int t = rand() / RAND_MAX;
    return t1 + t * (t2 - t1);
}
void rword (char *word){
    int len = rand () % 6 + 1;
    word [len] = 0;
    while (len) word [--len] = 'a' + rand () % 26;
}
void generate_words(char words[kAnalyzeSize][BUFFER_MAX_SIZE+1], int size){
    int k = 0;
    
    while (k <= size) {
        
        char name[BUFFER_MAX_SIZE+1];
        rword(name);
        
        bool flag_continue = false;
        for (int i = 0; i < k; i++)
            if (strncmp(words[i], name, BUFFER_MAX_SIZE+1) == 0)
            {
                flag_continue = true;
                break;
            }
        
        if (!flag_continue)
            strncpy(words[k++], name, BUFFER_MAX_SIZE+1);
        
    }
}
void generate_tree(tnode **tree, int size, char words[kAnalyzeSize][BUFFER_MAX_SIZE+1], Error *error){
    
    for (int i = 0; i < size; i++)
    {
        tfile file;
        strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
        
        tnode *node = create_node(file, error);
        if (error->code == 1)
            return;
        *tree = insert(*tree, node, error);
        if (error->code != 0)
            printf("\nError%s\n", error->message);
    }
}
void generate_btree(tnode **btree, int size,  char words[kAnalyzeSize][BUFFER_MAX_SIZE+1], Error *error){
    
    for (int i = 0; i < size; i++)
    {
        tfile file;
        strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
        
        tnode *bnode = create_node(file, error);
        if (error->code == 1)
            return;
        *btree = avl_insert(*btree, bnode, error);
        if (error->code != 0)
            printf("\nError%s\n", error->message);
        
    }
}
void generate_table(hashmap *hashmap, int size,  char words[kAnalyzeSize][BUFFER_MAX_SIZE+1], Error *error){

    
    for (int i = 0; i < size; i++)
    {
        hashtab_add(hashmap, words[i], error);
        if (error->code == 1)
            return;
    }
}
void generate_table_closed(hashmap_closed *hashmap, int size,  char words[kAnalyzeSize][BUFFER_MAX_SIZE+1]){
    
    for (int i = 0; i < size; hashtab_closed_add(hashmap, words[i++]));
}

void analyze_search(){
    
    unsigned long long sum = 0, tb, te,
    time_balanced,
    kcmd_balanced = 0,
    time_binary,
    kcmd_binary = 0,
    time_table,
    kcmd_table = 0,
    time_table_closed,
    kcmd_table_closed = 0;
    
    char raw_input[BUFFER_MAX_SIZE+1];
    tfile file;
    Error error = {0, ""};
    bool search_flag = false;
    tnode *search_node = NULL;
    listnode *search_listnode = NULL;
    char words[kAnalyzeSize][BUFFER_MAX_SIZE+1];
    tnode *btree = NULL, *tree = NULL;
    hashmap hashmap;
    hashmap_closed hashmap_closed;
    
    
    hashtab_init(&hashmap, 10);
    hashtab_closed_init(&hashmap_closed, 10);
    
    printf("\nAnalyze of search.\n");
    
    /**************************/
    /***** Generaing all  *****/
    /**************************/
    generate_words(words, kAnalyzeSize);
    generate_tree(&tree, kAnalyzeSize, words, &error);
    generate_btree(&btree, kAnalyzeSize, words, &error);
    generate_table(&hashmap, kAnalyzeSize, words, &error);
    generate_table_closed(&hashmap_closed, kAnalyzeSize, words);
    
    if (tree == NULL)
    {
        printf("Error: Tree is empty!\n");
        show_continue();
        return;
    }
    if (btree == NULL)
    {
        printf("Error: Balanced tree is empty!\n");
        show_continue();
        return;
    }
    if (error.code == 1)
    {
        printf("\nMemory error: %s\n", error.message);
        show_continue();
        return;
    }
    /**************************/
    /**************************/
    
    
    /**************************/
    /****** SHOWING ALL *******/
    /**************************/
    
    /**************************/
    /***** Binary Tree ********/
    FILE *f = fopen("analyze_tree_binary.gv", "w");
    if (!f) {
        printf("Error: Unable to create file!\n");
        show_continue();
        return;
    }
    export_to_dot(f, "tree", tree);
    fclose(f);
    if (showTree)
        system("open -a Graphviz analyze_tree_binary.gv");
    /**************************/
    /**************************/
    
    /**************************/
    /***** Balanced Tree ******/
    /**************************/
    f = fopen("analyze_tree_balanced.gv", "w");
    if (!f)
    {
        printf("Error: Unable to create file!\n");
    }
    else{
        export_to_dot(f, "btree", btree);
        fclose(f);
       if (showTree)
            system("open -a Graphviz analyze_tree_balanced.gv");
    }
    /**************************/
    /**************************/
    
    /**************************/
    /****** Hash Table ********/
    /******     OPEN   ********/
    /**************************/
    printf("\n\nHashTable (open): \n");
    for (int i = 0; i < hashmap.size; i++)
        if (hashmap.table[i] != NULL)
        {
            printf("%d ", i);
            for (listnode *node = hashmap.table[i]; node != NULL; node = node->next)
                printf("-> %s", node->key);
            printf("\n");
        }
    printf("\nCount of collisions: %d",  hashmap.col_count);
    printf("\nCount of restructuralizations: %d\n",  hashmap.res_count);
    
    printf("\n");
    /**************************/
    /**************************/
    
    /**************************/
    /****** Hash Table ********/
    /******    CLOSED  ********/
    /**************************/
    printf("\n\nHashTable (closed): \n");
    for (int i = 0; i < hashmap_closed.size; i++)
        if (hashmap_closed.table[i][0] != '\0')
        {
            printf("%d %s", i, hashmap_closed.table[i]);
            printf("\n");
        }
    printf("\nCount of collisions: %d",  hashmap_closed.col_count);
    printf("\nCount of restructuralizations: %d\n",  hashmap_closed.res_count);
    
    printf("\n");
    /**************************/
    /**************************/
    
    /**************************/
    /* INPUT FOR ALL ANALYSES */
    /**************************/
    
    printf("\n----------------------------------------------------------\n");
    if (!autoSearch && !fullSearch)
    {
        printf("Input file name to find it's node in tree: ");
        scanf("%s", raw_input);
        
        strncpy(file.name, raw_input, BUFFER_MAX_SIZE+1);
    }
    else if(!fullSearch)
    {
        strncpy(file.name, words[randomRange(kAnalyzeSize/2, kAnalyzeSize)], BUFFER_MAX_SIZE+1);
        printf("Searh word is \"%s\"\n", file.name);
    }
    printf("----------------------------------------------------------\n");
    /**************************/
    /**************************/
    
    
    /******************************************************************************/
    /*** ANALYZING SEARCH   ***/
    /******************************************************************************/
    
    /**************************/
    /*****   Binary Tree ******/
    /**************************/
    sum = 0;
    for (int i = 0; i == 0 || (fullSearch && i < kAnalyzeSize); i++)
    {
        if (fullSearch)
            strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
            
        for (int j = 0; j < kInetation; j++)
        {
            tb = tick();
            search_node = lookup(tree, file, compare_name, &kcmd_binary);
            te = tick();
            sum += te-tb;
        }
    }
    if (search_node != NULL)
        printf("\nNode is found in binary tree!");
    else
        printf("\nNode does not found in binary tree!");
    time_binary =  (sum / ( fullSearch ? kAnalyzeSize : kInetation ));
    /**************************/
    /**************************/
    
    /**************************/
    /***** Balanced Tree ******/
    /**************************/
    sum = 0;
    for (int i = 0;  i == 0 || (fullSearch && i < kAnalyzeSize); i++)
    {
        if (fullSearch)
            strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
        for (int j = 0; j < kInetation; j++)
        {
            tb = tick();
            search_node = lookup(btree, file, compare_name, &kcmd_balanced);
            te = tick();
            sum += te-tb;
        }
    }
    if (search_node != NULL)
        printf("\nNode is found in balanced tree!");
    else
        printf("\nNode does not found in balanced tree!");
    time_balanced = (sum / ( fullSearch ? kAnalyzeSize : kInetation ));
    /**************************/
    /**************************/
    
    /**************************/
    /****** Hash Table ********/
    /******     OPEN   ********/
    /**************************/
    sum = 0;
    for (int i = 0;  i == 0 || (fullSearch && i < kAnalyzeSize); i++)
    {
        if (fullSearch)
            strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
        for (int j = 0; j < kInetation; j++)
        {
    //        tb = tick();
            search_listnode = hashtab_lookup(hashmap, file.name, &tb, &te, &kcmd_table);
    //        te = tick();
            sum += te-tb;
        }
    }
    if (search_listnode != NULL)
        printf("\nNode is found in open table!");
    else
        printf("\nNode does not found in open table!");
    time_table = (sum / ( fullSearch ? kAnalyzeSize : kInetation ));
    
    /**************************/
    /**************************/
    
    
    /**************************/
    /****** Hash Table ********/
    /******    CLOSED  ********/
    /**************************/
    sum = 0;
    for (int i = 0;  i == 0 || (fullSearch && i < kAnalyzeSize); i++)
    {
        if (fullSearch)
            strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
        for (int j = 0; j < kInetation; j++)
        {
            //        tb = tick();
            search_flag = hashtab_closed_lookup(hashmap_closed, file.name, &tb, &te, &kcmd_table_closed);
            //        te = tick();
            sum += te-tb;
        }
    }
    if (search_flag)
        printf("\nNode is found in closed table!");
    else
        printf("\nNode does not found in closed table!");
    time_table_closed = (sum / ( fullSearch ? kAnalyzeSize : kInetation ));
    
    /**************************/
    /**************************/
    
    /******************************************************************************/
    /******************************************************************************/
    /******************************************************************************/
    
    
    printf("\n\nBalanced tree");
    printf("\n\tTime: %llu\n",time_balanced);
    printf("\tCount of comparisons: %llu",  kcmd_balanced);
    
    printf("\n\nBinary tree");
    printf("\n\tTime: %llu\n",time_binary);
    printf("\tCount of comparisons: %llu",  kcmd_binary);
    
    printf("\n\nHash open table");
    printf("\n\tTime: %llu\n",time_table);
    printf("\tCount of comparisons: %llu\n",  kcmd_table);
    
    printf("\n\nHash closed table");
    printf("\n\tTime: %llu\n",time_table_closed);
    printf("\tCount of comparisons: %llu\n",  kcmd_table_closed);

    
    
    printf("\n\n");
    
    /**************************/
    //Очищение памяти
    /**************************/
    apply_post(tree, free_node, NULL);
    apply_post(btree, free_node, NULL);
    for (int i = 0; i < hashmap.size; i++)
        if (hashmap.table[i] != NULL)
            for (listnode *node = hashmap.table[i]; node != NULL; node = node->next)
                free(node);
    /**************************/
    /**************************/
    
    show_continue();
    
}


#pragma mark Interface functions
void show_menu(){
    
    printf("Input action: \n");
    printf("0 - Exit\n");
    printf("1 - Import data\n");
    printf("2 - Export and show trees\n");
    printf("3 - Show open hash-table\n");
    printf("4 - Show closed hash-table\n");
    printf("5 - Print tree \n");
    printf("6 - Print balanced tree \n");
    printf("7 - Analyze search\n");
    printf("8 - Process task\n");
}
void show_continue(){

    system( "read -n 1 -s -p \"Press any key to continue... 🌝   \"" );
    
}
void show_error_message(int error){
    
    switch (error) {
        case 0:
            break;
        case 1:
            printf("\nInvalid command code! 😱\n");
            break;
        case 2:
            printf("\nEmpty input! 😰!\n");
            break;
        case 3:
            printf("\nYou should input only numbers! 😥\n");
            break;
        case 4:
            printf("\nInvalid input! 😡\n");
            break;
        case 5:
            printf("\nToo big number. The max is %i! 😡\n", MAX_NUMBER_SIZE);
            break;
        default:
            printf("\nUnexpected error with code %d. 💀\n", error);
            break;
    }
    
}
void show_node_info(tnode * node, void *param){
    tfile file = node->file;
    
    printf("\nName: %s.\n", file.name);
   }

#pragma mark Task
void process_task(tnode **tree, tnode **btree, hashmap *hashtab, hashmap_closed * hashtab_closed){
    
    if (*tree == NULL)
    {
        printf("\nBinary tree is empty!\n");
        show_continue();
        return;
    }
    if (*tree == NULL)
    {
        printf("\n Balanced tree is empty!\n");
        show_continue();
        return;
    }
    
    char raw_input[BUFFER_MAX_SIZE+1];
    Error error = {0, ""};
    tnode *new_tree = NULL;
    tnode *new_tree_balanced = NULL;
    
    printf("\nInput letter: ");
    scanf("%s", raw_input);
    
    remove_nodes_predate(tree, &new_tree, raw_input[0], &error, insert);
    if (error.code != 0)
        printf("\nError: %s\n", error.message);
    
    apply_post(*tree, free_node, NULL);
    *tree = copy_tree(new_tree, &error);
    apply_post(new_tree, free_node, NULL);

    remove_nodes_predate(btree, &new_tree_balanced, raw_input[0], &error, avl_insert);
    if (error.code != 0)
        printf("\nError: %s\n", error.message);
    
    apply_post(*btree, free_node, NULL);
    *btree = copy_tree(new_tree_balanced, &error);
    apply_post(new_tree_balanced, free_node, NULL);
    
    hashtab_delete_all(hashtab, raw_input[0]);
    hashtab_closed_delete_all(hashtab_closed, raw_input[0]);
    
    
    show_continue();
}


#pragma mark Processing
void process_input_insert_node(tnode **root){
    char raw_input[BUFFER_MAX_SIZE+1];
    tfile file;
    
    printf("\nInput file info:\n");
    
    printf("\nInput filename: ");
    scanf("%s", raw_input);
    strncpy(file.name, raw_input, BUFFER_MAX_SIZE+1);

    Error error = {0, ""};
    tnode *node;
    
    node = create_node(file, &error);
    if (node == NULL) {
        printf(MESSAGE_MEMORY_ERROR);
        show_continue();
        return;
    }
    *root = insert(*root, node, &error);
    if (error.code != 0 )
        printf("\nError: %s ⚠️\n", error.message);
    
    
    show_continue();
}
void process_input_eject_node(tnode **root){
    if (*root == NULL)
    {
        printf("\nTree is empty!\n");
        show_continue();
        return;
    }
    
    char raw_input[BUFFER_MAX_SIZE+1];
    tfile file;
    Error error = {0, ""};
 
    printf("\nInput file name to eject from tree:\n");
    scanf("%s", raw_input);
    strncpy(file.name, raw_input, BUFFER_MAX_SIZE+1);
    
    *root = eject(*root, file, compare_name, &error);
    if (error.code != 0)
        printf("\nError: %s\n", error.message);

    show_continue();
    
    
    
}
void process_load_file(tnode **root, tnode **broot, hashmap *hashmap, hashmap_closed* hashmap_closed){
    
    char filename[BUFFER_MAX_SIZE+1], name[BUFFER_MAX_SIZE+1];
    int count = 0;
    
    printf("Input filename: ");
    scanf("%s", filename);
    
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        printf("\nUnable to read file!\n");
        show_continue();
        return;
    }
    
    while ( fscanf(f, "%s\n", name) == 1 )
    {
    
        tfile file;
        strncpy(file.name, name, BUFFER_MAX_SIZE+1);
        Error error = {0, ""};
        tnode *node = create_node(file, &error);
        if (node == NULL) {
            printf(MESSAGE_MEMORY_ERROR);
            show_continue();
            return;
        }
        *root = insert(*root, node, &error);
        if (error.code != 0 )
            printf("\nError: %s ⚠️\n", error.message);
        
        tnode *bnode = create_node(file, &error);
        if (node == NULL) {
            printf(MESSAGE_MEMORY_ERROR);
            show_continue();
            return;
        }
        *broot = avl_insert(*broot, bnode, &error);
        if (error.code != 0 )
            printf("\nError (b): %s ⚠️\n", error.message);
        
        hashtab_add(hashmap, file.name, &error);
        if (error.code != 0 )
            printf("\nError (b): %s ⚠️\n", error.message);
        
        hashtab_closed_add(hashmap_closed, file.name);
        
        count++;
        
    }
    
    if (count == 0)
        printf("\nFile is empty or has corrupted data! ⚠️\n");
    else
        printf("\nReaded %d records!\n", count);
    
    fclose(f);
    show_continue();

    
    
}
void process_lookup(tnode *root){
    
    unsigned long long sum = 0, tb, te, kcmp = 0;
    
    if (root == NULL)
    {
        printf("Tree is empty!");
        show_continue();
        return;
    }
    
    char raw_input[BUFFER_MAX_SIZE+1];
    tfile file;
    Error error = {0, ""};
    tnode *search_node = NULL;
    
    printf("\nInput file name to find it's node in tree:\n");
    scanf("%s", raw_input);
    strncpy(file.name, raw_input, BUFFER_MAX_SIZE+1);
    
    for (int j = 0; j < kInetation; j++)
    {
        tb = tick();
        search_node = lookup(root, file, compare_name, &kcmp);
        te = tick();
        sum += te-tb;
    }
    if (error.code != 0)
        printf("\nError: %s\n", error.message);
    
    if (search_node != NULL)
        show_node_info(search_node, NULL);
    else
        printf("\nNode does not found!\n");

    printf("\n\nTime : %llu\n\n", sum / kInetation);
    
    show_continue();
    
}
void handle_action(int action, tnode **tree, tnode **btree, hashmap *hashmap, hashmap_closed *hashmap_closed){
    
    switch (action) {
        case 0:
            break;
        case 1:
            process_load_file(tree, btree, hashmap, hashmap_closed);
            break;
        case 2:
        {
            if (*tree == NULL)
            {
                printf("Error: Tree is empty!\n");
                show_continue();
                break;
            }
            FILE *f = fopen("tree.gv", "w");
            if (!f) {
                printf("Error: Unable to create file!\n");
                show_continue();
                break;
            }
            
            export_to_dot(f, "tree", *tree);
            fclose(f);
            system("open -a Graphviz tree.gv");
            
            
            if (*btree == NULL)
            {
                printf("Error: Tree is empty!\n");
                show_continue();
                break;
            }
            f = fopen("btree.gv", "w");
            if (!f) {
                printf("Error: Unable to create file!\n");
                show_continue();
                break;
            }
            
            export_to_dot(f, "btree", *btree);
            fclose(f);
            system("open -a Graphviz btree.gv");
            
            
            break;
        }
        case 3:
        {
            bool empty = true;
            for (int i = 0; i < hashmap->size; i++)
                if (hashmap->table[i] != NULL)
                {
                    empty = false;
                    printf("%d ", i);
                    for (listnode *node = hashmap->table[i]; node != NULL; node = node->next)
                        printf("-> %s", node->key);
                    printf("\n");
                }
            if (empty)
                printf("\nHash-table is empty!\n");
            show_continue();
            break;
        }
        case 4:
        {
            bool empty = true;
            for (int i = 0; i < hashmap_closed->size; i++)
                if (hashmap_closed->table[i][0] != '\0')
                {
                    empty = false;
                    printf("%d %s\n", i, hashmap_closed->table[i]);
                }
            if (empty)
                printf("\nHash-table is empty!\n");
            show_continue();
            break;
        }
        case 5:
        {
            if (*tree == NULL)
            {
                printf("Error: Tree is empty!\n");
                show_continue();
                break;
            }
            printf("Traversal");
            printf("\n\nPre-order: \n");
            apply_pre(*tree, show_node_info, "%s \n");
            printf("\n\nInt-order: \n");
            apply_in(*tree, show_node_info, "%s \n");
            printf("\n\nPre-order: \n");
            apply_post(*tree, show_node_info, "%s \n");
            printf("\n");
            show_continue();
            break;
        }
        case 6:
        {
            if (*btree == NULL)
            {
                printf("Error: Tree is empty!\n");
                show_continue();
                break;
            }
            printf("Traversal\n");
            printf("Pre-order: \n");
            apply_pre(*btree, show_node_info, "%s \n");
            printf("Int-order: \n");
            apply_in(*btree, show_node_info, "%s \n");
            printf("Pre-order: \n");
            apply_post(*btree, show_node_info, "%s \n");
            show_continue();
            break;
        }
        case 7:
            analyze_search();
            break;
        case 8:
            process_task(tree, btree, hashmap, hashmap_closed);
            break;
        default:
            printf("Sorry, this functionality is unavialible at this moment 👷\n");
            show_continue();
            break;
    }
}
bool is_number(const char *str){

    while(*str)
        if (!isdigit(*str++))
            return false;
    
    return true;
    
}


#pragma mark Exporting DOT
void to_dot(tnode *tree, void *param){
    FILE *f = param;
    
    if (tree->left)
        fprintf(f, "\"%s\" -> \"%s\";\n",
            tree->file.name, tree->left->file.name);
    
    if (tree->right)
        fprintf(f, "\"%s\" -> \"%s\";\n",
            tree->file.name, tree->right->file.name);
    
}
void export_to_dot(FILE *f, const char *tree_name, tnode *tree){
    fprintf(f, "digraph %s {\n", tree_name);
    
    apply_post(tree, to_dot, f);
    
    fprintf(f, "}\n");
}

