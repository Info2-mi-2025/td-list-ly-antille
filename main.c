#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ne pas modifier
void init_file()
{
    // data.txt
    FILE* f = fopen("data.txt", "w");
    if (f)
    {
        fprintf(f, "5\n12\n8\n1\n19\n");
        fclose(f);
    }

    // data2.txt
    f = fopen("data2.txt", "w");
    if (f)
    {
        fprintf(f, "10\n20\n30\n");
        fclose(f);
    }

    // data3.txt
    f = fopen("data3.txt", "w");
    if (f)
    {
        fprintf(f, "1\n2\n3\n4\n5\n");
        fclose(f);
    }

    // data_vide.txt
    f = fopen("data_vide.txt", "w");
    if (f)
    {
        fclose(f);  // fichier vide
    }
}



typedef struct Node {
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} List;



// Fonctions de base
int count(List* list) {
    int count = 0;
    Node* current = list->head;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

void append(List* list, int value) {
    if (list == NULL) {
        exit(1);
    }

    // Création du nouveau noeud 
    Node* new_node = malloc(sizeof(Node));
    new_node->next = NULL;
    new_node->value = value;

    // La liste est vide
    if (list->head == NULL) {
        list->head = new_node;
    }
    else {
        list->tail->next = new_node;
    }
    list->tail = new_node;
}

void free_list(List* list) {
    Node* current = list->head;

    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    free(list);    
}

void print_list(const List* list) {
    Node* current = list->head;
    printf("Liste : ");

    while (current != NULL) {
        printf(" %d", current->value);

        if (current->next != NULL) {
            printf(" ->");
        }
        current = current->next;
    }
}

void reverse_list(List* list) {
    int count = 0;
    Node* current = list->head;
    
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    int* temp = malloc (count * sizeof(int));
    current = list->head;

    for (int i=0; i<count; i++) {
        temp[i] = current->value;
        current = current->next;
    }

    current = list->head;

    for (int i= count - 1; i >= 0; i--) {
        current->value = temp[i];
        current = current->next;
    }

    free(temp);
}

int sum_list(const List* list) {
    Node* current = list->head;
    int sum = 0;

    while (current != NULL) {
        sum += current->value;
        current = current->next;
    }
    return sum;
}

int min_list(const List* list) {
    Node* current = list->head;
    int min = current->value;

    while (current != NULL) {
        if (current->value < min) {
            min = current->value;
        }
        current = current->next;
    }
    return min;
}

int max_list(const List* list) {
    Node* current = list->head;
    int max = current->value;

    while (current != NULL) {
        if (current->value > max) {
            max = current->value;
        }
        current = current->next;
    }
    return max;
}

void filter_list(List* list, int threshold) {
    Node* current = list->head;
    Node* prev = NULL;

    while (current != NULL) {
        if (current->value < threshold) {
            Node *to_delete = current;

            if (prev == NULL) {
                list->head = current->next;
            }
            else {
                prev->next = current->next;
            }

            current = current->next;
            free(to_delete);
        }
        else {
            prev = current;
            current = current->next;
        }
    }

    list->tail = NULL;
    current = list->head;

    while (current != NULL) {
        if (current->next == NULL) {
            list->tail = current;
        }

        current = current->next;
    }
}

void help()
{
    printf("Utilisation : ./app <fichier> [options]\n\n");
    printf("Options disponibles :\n");
    printf("  --reverse         Inverse l'ordre des éléments\n");
    printf("  --sum             Affiche la somme des éléments\n");
    printf("  --filter <val>    Filtre les éléments >= val\n");
    printf("  --add <val>       Ajoute une valeur à la fin du fichier\n");
    printf("  --help            Affiche ce message d'aide\n");
    printf("  --version, -v     Affiche la version du programme\n");
    printf("  --min             Affiche la valeur minimale de la liste\n");
    printf("  --max             Affiche la valeur maximale de la liste\n");
}

void version() {
    printf("version 1.0\n");
}

// Lecture fichier
bool read_file(const char* filename, List* list)
{
    FILE* f = fopen(filename, "r");
    if (!f) return false;
    int value;
    
    while (fscanf(f, "%d", &value) == 1) append(list, value);
    
    fclose(f);
    return true;
}

bool add_to_file(const char* filename, int value)
{
    FILE* f = fopen(filename, "a");
    if (!f) return false;
    fprintf(f, "%d\n", value);
    fclose(f);
    return true;
}

int main(int argc, char* argv[])
{
    // Ne pas modifier
    init_file();
    // ---------------

    bool option_add = false;
    bool option_filter = false;

    if (argc < 2 || strncmp(argv[1], "--", 2) == 0) {
        return 1; 
    }

    const char* filename = argv[1];

    List* list = (List*)malloc(sizeof(List));
    list->head = list->tail = NULL;

    if(!read_file(filename, list)) {
        free(list);
        return 2;
    }

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            help();
            free_list(list);
            return 0;
        } 
        else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            version();
            free_list(list);
            return 0;
        } 
        else if (strcmp(argv[i], "--reverse") == 0) {
            reverse_list(list);
        } 
        else if (strcmp(argv[i], "--sum") == 0) {
            printf("Somme : %d\n", sum_list(list));
        } 
        else if (strcmp(argv[i], "--min") == 0) {
            printf("Minimum : %d\n", min_list(list));
        } 
        else if (strcmp(argv[i], "--max") == 0) {
            printf("Maximum : %d\n", max_list(list));
        } 
        else if (strcmp(argv[i], "--filter") == 0) {
            if (i + 1 < argc) {
                int val = atoi(argv[++i]);
                filter_list(list, val);
            } else {
                fprintf(stderr, "Erreur! Valeur manquante pour --filter!\n");
                free_list(list);
                return 1;
            }
        } 
        else if (strcmp(argv[i], "--add") == 0) {
            if (i + 1 < argc) {
                int val = atoi(argv[++i]);
                if (add_to_file(filename, val)) {
                    printf("Valeur %d ajoutée au fichier.\n", val);
                    free_list(list);
                    return 0;
                } else {
                    fprintf(stderr, "Erreur lors de l'ajout au fichier.\n");
                    free_list(list);
                    return 2;
                }
            } else {
                fprintf(stderr, "Erreur! Valeur manquante pour --add!\n");
                free_list(list);
                return 1;
            }
        } else {
            fprintf(stderr, "Option inconnue : %s\n", argv[i]);
            free_list(list);
            return 1;
        }
    }    
    
    print_list(list);
    printf("\n");

    free_list(list);

    return 0;       
}