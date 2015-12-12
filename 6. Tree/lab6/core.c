//
//  core.c
//  lab6
//
//  Created by Amin Benarieb on 21/11/15.
//  Copyright © 2015 Amin Benarieb. All rights reserved.
//

#include "core.h"

#pragma mark Private part (analyze & comparing)

#define kInetation 1
#define maxTree 640
#define minTree 5
#define showTree false
#define autoSearch true
#define fullSearch true
#define hideSearchWord true


unsigned long long tick(void){
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc": "=A" (d) );
    
    return d;
}
int compare_date(tfile f1, tfile f2){
    
    tdate d1 = f1.lastused;
    tdate d2 = f2.lastused;
    
    if (d1.year < d2.year)
        return -1;
    else if (d1.year > d2.year)
        return 1;
    else
    {
        if (d1.month < d2.month)
            return -1;
        else if (d1.month > d2.month)
            return 1;
        else if (d1.day < d2.day)
            return -1;
        else if(d1.day > d2.day)
            return 1;
        else
            return 0;
    }
    
}
int compare_date_lower(tfile f1, tfile f2){
    
    tdate d1 = f1.lastused;
    tdate d2 = f2.lastused;
    
    if (d1.year < d2.year)
        return 1;
    else if (d1.year > d2.year)
        return 0;
    else
    {
        if (d1.month < d2.month)
            return 0;
        else if (d1.month > d2.month)
            return 0;
        else if (d1.day < d2.day)
            return 1;
        else if(d1.day > d2.day)
            return 0;
        else
            return 1;
    }
    
}
int compare_name(tfile f1, tfile f2){
    return strcmp(f1.name, f2.name);
}
void rword (char *word){
    int len = rand () % 6 + 1;
    word [len] = 0;
    while (len) word [--len] = 'a' + rand () % 26;
}
int randomRange(int t1, int t2){
    int t = rand() / RAND_MAX;
    return t1 + t * (t2 - t1);
}

void generate_words(char words[maxTree][BUFFER_MAX_SIZE+1], int size){
    int k = 0;
    
    while (k < size) {
        
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
void generate_tree(tnode **tree, int size, char words[maxTree][BUFFER_MAX_SIZE+1], Error *error){
    
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

void analyze_search(){
    
    unsigned long long sum = 0, tb, te;
    
    char raw_input[BUFFER_MAX_SIZE+1];
    tfile file;
    Error error = {0, ""};
    tnode *search_node = NULL;
    char words[maxTree][BUFFER_MAX_SIZE+1];
    tnode *tree = NULL;

    printf("\nAnalyze of search.\n");
    
    for (int size = minTree; size <= maxTree ; size *= 2) {
        /**************************/
        /*****    Generaing  *****/
        /**************************/
        generate_words(words, size);
        generate_tree(&tree, size, words, &error);
        
        if (tree == NULL)
        {
            printf("Error: Tree is empty!\n");
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
        FILE *f = fopen("analyze_tree.gv", "w");
        if (!f) {
            printf("Error: Unable to create file!\n");
            show_continue();
            return;
        }
        export_to_dot(f, "analyze_tree", tree);
        fclose(f);
        if (showTree)
            system("open -a Graphviz analyze_tree.gv");
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
            strncpy(file.name, words[randomRange(size-1, size-1)], BUFFER_MAX_SIZE+1);
            if (!hideSearchWord)
                printf("Searh word is \"%s\"\n", file.name);
        }
        printf("----------------------------------------------------------\n");
        /**************************/
        /**************************/
        
        /******************************************************************************/
        /*** ANALYZING SEARCH   ***/
        /******************************************************************************/
        /**************************/
        /*****      Tree     ******/
        /**************************/
        sum = 0;
        for (int i = 0; i == 0 || (fullSearch && i < size); i++)
        {
            if (fullSearch)
                strncpy(file.name, words[i], BUFFER_MAX_SIZE+1);
            
            for (int j = 0; j < kInetation; j++)
            {
                tb = tick();
                search_node = lookup(tree, file, compare_name);
                te = tick();
                sum += te-tb;
            }
            sum /= kInetation;
        }
        if (!autoSearch) {
            if (search_node != NULL)
                printf("\nNode is found in binary tree!");
            else
                printf("\nNode does not found in binary tree!");
        }
        printf("\nTree of size %d", size);
        printf("\nTime: %llu\n", (sum / ( fullSearch ? size : 1 )));
        /**************************/
        /**************************/
        /******************************************************************************/
        /******************************************************************************/
        /******************************************************************************/
        
        /**************************/
        //Очищение памяти
        /**************************/
        apply_post(tree, free_node, NULL);
        tree = NULL;
        /**************************/
        /**************************/

    }
    
    show_continue();
    
}

#pragma mark Interface functions
void show_menu(){

printf("Input action: \n");
printf("0 - Exit\n");
printf("1 - Insert node into tree\n");
printf("2 - Remove node from tree\n");
printf("3 - Find node in tree \n");
printf("4 - Perform task\n");
printf("5 - Print data\n");
printf("6 - Export tree \n");
printf("7 - Import data \n");
printf("8 - Analyze search \n");
}
void show_continue(){
    
    system( "read -n 1 -s -p \"\n\nPress any key to continue... 🌝   \"" );
    
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
    char *attr;
    switch (file.attubute) {
        case archive:
            attr = "archive";
            break;
        case hidden:
            attr = "hidden";
            break;
        case systeme:
            attr = "system";
            break;
        case readonly:
            attr = "readonly";
            break;
            
        default:
            attr = "unknown";
            break;
    }
    printf("Attribute %s.\n", attr);
    printf("Last used (%d/%d/%d).\n", file.lastused.day, file.lastused.month, file.lastused.year);
}


#pragma mark Processing
void process_input_insert_node(tnode **root){
    char raw_input[BUFFER_MAX_SIZE+1];
    tfile file;
    
    printf("\nInput file info:\n");
    
    printf("\nInput filename: ");
    scanf("%s", raw_input);
    strncpy(file.name, raw_input, BUFFER_MAX_SIZE+1);

    
    printf("\nInput attribute (1 - archive, hidden - 2, system - 3, readonly - 4): ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int attr = atoi(raw_input);
        if (abs(attr) > 4)
            attr = 0;

        file.attubute = attr;
    }
    else
    {
        printf("Invalid attribute!\n");
        show_continue();
        return;
    }
    
    printf("\nInput date: \n");
    
    printf("\nInput day: ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int day = atoi(raw_input);
        if (day > 31)
            day = 31;
        
        file.lastused.day = day;
    }
    else
    {
        printf("Invalid day!\n");
        show_continue();
        return;
    }
    
    printf("\nInput month: ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int month = atoi(raw_input);
        if (month > 12)
            month = 12;
        
        file.lastused.month = month;
    }
    else
    {
        printf("Invalid month!\n");
        show_continue();
        return;
    }
    
    printf("\nInput year: ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int year = atoi(raw_input);
        if (year > 2015)
            year = 2015;
        
        file.lastused.year = year;
    }
    else
    {
        printf("Invalid year!\n");
        show_continue();
        return;
    }
    


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
void process_load_file(tnode **root){
    
    char filename[BUFFER_MAX_SIZE+1], name[BUFFER_MAX_SIZE+1];
    int attribute, day, month, year, count = 0;
    
    printf("Input filename: ");
    scanf("%s", filename);
    
    FILE *f = fopen(filename, "r");
    if(!f)
    {
        printf("\nUnable to read file!\n");
        show_continue();
        return;
    }
    
    while ( fscanf(f, "%s %d %d/%d/%d\n", name, &attribute, &day, &month, &year) == 5 )
    {
        if (abs(attribute) > 4)
            attribute = 0;
        if (abs(day) > 31)
            day = 31;
        if (abs(month) > 12)
            month = 12;
        if (abs(year) > 2015)
            year = 2015;
        
        tfile file;
        strncpy(file.name, name, BUFFER_MAX_SIZE+1);
        file.attubute = attribute;
        file.lastused.day = day;
        file.lastused.month = month;
        file.lastused.year = year;
        
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
        
        count++;
        
    }
    
    if (count == 0)
        printf("\nFile is empty or has corrupted data! ⚠️\n");
    else
        printf("\nReaded %d records!\n", count);
    
    fclose(f);
    show_continue();

    
    
}
void process_task(tnode **root){
    
    
    if (*root == NULL)
    {
        printf("\nTree is empty!\n");
        show_continue();
        return;
    }
    
    char raw_input[BUFFER_MAX_SIZE+1];
    tdate date;
    
    printf("\nInput date: \n");
    
    printf("\nInput day: ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int day = atoi(raw_input);
        if (day > 31)
            day = 31;
        
        date.day = day;
    }
    else
    {
        printf("Invalid day!\n");
        show_continue();
        return;
    }
    
    printf("\nInput month: ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int month = atoi(raw_input);
        if (month > 12)
            month = 12;
        
        date.month = month;
    }
    else
    {
        printf("Invalid month!\n");
        show_continue();
        return;
    }
    
    printf("\nInput year: ");
    scanf("%s", raw_input);
    if (is_number(raw_input))
    {
        int year = atoi(raw_input);
        if (year > 2015)
            year = 2015;
        
        date.year = year;
    }
    else
    {
        printf("Invalid year!\n");
        show_continue();
        return;
    }
    
    tfile file = {"", 0, date};
    Error error = {0, ""};
    tnode *new_tree = NULL;
    
    remove_nodes_predate(root, &new_tree, file, &error);
    if (error.code != 0)
        printf("\nError: %s\n", error.message);
    
    apply_post(*root, free_node, NULL);
    *root = copy_tree(new_tree, &error);
    apply_post(new_tree, free_node, NULL);
    
    
    show_continue();
    
}
void process_lookup(tnode *root){
    
    unsigned long long sum = 0, tb, te;
    
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
        search_node = lookup(root, file, compare_name);
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
void handle_action(int action, tnode **tree){
    
    switch (action) {
        case 0:
            break;
        case 1:
            process_input_insert_node(tree);
            break;
        case 2:
            process_input_eject_node(tree);
            break;
        case 3:
            process_lookup(*tree);
            break;
        case 4:
            process_task(tree);
            break;
        case 5:
            if (*tree == NULL)
            {
                printf("Error: Tree is empty!\n");
                show_continue();
                break;
            }
            apply_pre(*tree, show_node_info, "%s \n");
            printf("\n");
            apply_post(*tree, show_node_info, "%s \n");
            printf("\n");
            apply_in(*tree, show_node_info, "%s \n");
            show_continue();
            break;
        case 6:
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
                break;
            }
        case 7:
            process_load_file(tree);
            break;
        case 8:
            analyze_search();
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


#pragma Tree Node
tnode* create_node(tfile file, Error *error){
    tnode *node = malloc(sizeof(tnode));
    if (node == NULL)
    {
        error->code = 1;
        error->message = MESSAGE_MEMORY_ERROR;
        return NULL;
    }
    
    node->file = file;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}
void free_node(tnode *node, void *param){
    free(node);
}
tnode* insert(tnode *tree, tnode *node, Error *error){
    int cmp;
    
    if (tree == NULL)
        return node;
    
    cmp = strcmp( node->file.name, tree->file.name);
    
    if (cmp == 0)
    {
        error->code = 2;
        error->message = "Node is already exist in tree!";
        return tree;
    }
    else if (cmp < 0)
    {
        tree->left = insert(tree->left, node, error);
    }
    else
    {
        tree->right = insert(tree->right, node, error);
    }
    
    return tree;
}
tnode* lookup(tnode *tree, tfile file, int (*compare)(tfile, tfile)){
    int cmp;
    
    while (tree != NULL)
    {

        cmp = compare(file, tree->file);
        if (cmp == 0)
            return tree;
        else if (cmp < 0)
            tree = tree->left;
        else
            tree = tree->right;
    }
    
    return NULL;
}
tfile rightmost(tnode *root){
    while (root->right != NULL)
        root = root->right;
    return root->file;
}
tnode *eject(tnode *root, tfile file, int (*compare)(tfile, tfile), Error *error){
    
    if (root == NULL)
    {
        error->code = 4;
        error->message = "Node to eject does not exist in tree!";
        return NULL;
    }
    
    int cmp = compare(file, root->file);
    
    if (cmp == 0) {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (root->right == NULL && root->left != NULL) {
            tnode *temp = root->left;
            free(root);
            return temp;
        }
        if (root->right != NULL && root->left == NULL) {
            tnode *temp = root->right;
            free(root);
            return temp;
        }
        root->file = rightmost(root->left);
        root->left = eject(root->left, root->file, compare, error);
        return root;
    }
    if (cmp < 0) {
        root->left = eject(root->left, file, compare, error);
        return root;
    }
    if (cmp > 0) {
        root->right = eject(root->right, file, compare, error);
        return root;
    }
    
    
    return root;
}


#pragma mark Task functionality
void remove_nodes_predate(tnode **tree, tnode **newtree, tfile file, Error *error){
    if (*tree == NULL )
        return;
    
    if(!compare_date_lower((*tree)->file, file))
    {
        tnode *enode = create_node((*tree)->file, error);
        *newtree = insert(*newtree, enode, error);
    }
    remove_nodes_predate(&(*tree)->left, newtree, file, error);
    remove_nodes_predate(&(*tree)->right, newtree, file, error);
        
    


}
tnode *copy_tree(tnode *tree, Error *error){
    if (tree == NULL)
        return tree;
    
    tnode *temp = create_node(tree->file, error);
    temp->left = copy_tree(tree->left, error);
    temp->right = copy_tree(tree->right, error);
    return temp;
    
}

#pragma mark Traversal
void apply_pre(tnode *tree, void (*f)(tnode*, void*), void *arg){
    if (tree == NULL)
        return;
    
    f(tree, arg);
    apply_pre(tree->left, f, arg);
    apply_pre(tree->right, f, arg);
}
void apply_post(tnode *tree, void (*f)(tnode*, void*), void *arg){
    if (tree == NULL)
        return;
    
    apply_post(tree->left, f, arg);
    apply_post(tree->right, f, arg);
    f(tree, arg);
}
void apply_in(tnode *tree, void (*f)(tnode*, void*), void *arg){
    if (tree == NULL)
        return;
    
    apply_in(tree->left, f, arg);
    f(tree, arg);
    apply_post(tree->right, f, arg);
}


#pragma Exporting DOT
void to_dot(tnode *tree, void *param){
    FILE *f = param;
    
    if (tree->left)
        fprintf(f, "\"%s\n%d/%d/%d\" -> \"%s\n%d/%d/%d\";\n",
            tree->file.name, tree->file.lastused.day, tree->file.lastused.month, tree->file.lastused.year,
            tree->left->file.name, tree->left->file.lastused.day, tree->left->file.lastused.month, tree->left->file.lastused.year);
    
    if (tree->right)
        fprintf(f, "\"%s\n%d/%d/%d\" -> \"%s\n%d/%d/%d\";\n",
            tree->file.name, tree->file.lastused.day, tree->file.lastused.month, tree->file.lastused.year,
            tree->right->file.name, tree->right->file.lastused.day, tree->right->file.lastused.month, tree->right->file.lastused.year);
    
}
void export_to_dot(FILE *f, const char *tree_name, tnode *tree){
    fprintf(f, "digraph %s {\n", tree_name);
    
    apply_pre(tree, to_dot, f);
    
    fprintf(f, "}\n");
}
