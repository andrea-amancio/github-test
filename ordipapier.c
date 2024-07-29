// *******************************************************
// Ceci est un test de modification local
// Nom ......... : ordipapier.c
// Rôle ........ : Programme émulant le fonctionnement d'un ordinateur qui charge et exécute un programme
// Auteur ...... : Andréa Amancio
// Version ..... : V1 du 26/07/2024
// Licence ..... : Réalisé dans le cadre du cours de ADO en L1 à l'IED, chapitre n°10.
// Compilation . : gcc -Wall ordipapier.c -o mainprog
// Usage ....... : Pour exécuter : ./mainprog <nom_du_programme.txt>
//********************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum { false,true} bool;
#define MEM_SIZE 256

// Définition des registres et de la mémoire
signed short memory[MEM_SIZE]; // Mémoire de 256 cases
signed short RS;               // Registre de Sélection
signed short RM;               // Registre Mot
signed short PC;               // Compteur de Programme
signed short OP;               // Code opératoire
signed short AD;               // Adresse de l'opérande
signed short A;                // Accumulateur
signed short TZ;               // Case trapezoidale: pas vraiment utilisée ici
signed short in;               // Entrée
signed short out;              // Sortie
bool incrementer=true;

// Définition des microcodes
#define a RS=PC                 //microcode 1
#define b PC=RM                 //microcode 2
#define c A=RM                  //microcode 3
#define d RM=A                  //microcode 4
#define e OP=RM                 //microcode 5
#define f AD=RM                 //microcode 6
#define g RS=AD                 //microcode 7
#define h RM=in                 //microcode 8
#define i out=RM                //microcode 9
#define j TZ='+'                //microcode 10
#define k TZ='-'                //microcode 11
#define l TZ='&'                //microcode 17
#define n RM=memory[(int)RS]    //microcode 13
#define o memory[(int)RS]=RM    //microcode 14
#define p PC+=0x01              //microcode 15


//Fonction d'affichage des registres à un moment donné
void print_registers(const char *when) {
    printf("\n-- Registres %s l'instruction %02X--\n", when,OP);
    printf("PC  (Program Counter) : %02X\n", PC);
    printf("A   (Accumulateur)    : %02X\n", A);
    printf("RM  (Registre Mot)    : %02X\n", RM);
    printf("RS  (Registre Sélect) : %02X\n", RS);
    printf("OP  (Code opératoire) : %02X\n", OP);
    printf("AD  (Adresse opérande): %02X\n", AD);
    printf("\n");
}

// Fonction d'execution du programme
void execute_programme() {
    //sleep(1);
    a,n,e,p; //Recherche d'instruction : Phase 1

    switch (OP) {  //Exécution de l'instruction : Phase 2

        case 0xFF:// opération ajoutée pour les tests : met fin au programme
            exit(1);
        case 0x00:// LOAD
            a,n,c;
            break;
        case 0x01:// OUT #
            a,n,i;
            printf("Sortie: %02X\n", out);
            break;
        case 0x09:// IN #
            printf("Entrez valeur: "); //microcode 16
            scanf("%hx", &RM);        //microcode 8 
            c;
            break;
        case 0x10:// JUMP α
            a,n,b;
            incrementer=false;
            break;
        case 0x11:// BRN α
            if ((signed char)A<0){
                a,n,b;
                incrementer=false;
            }
            break;
        case 0x12:// BRZ α
            if (A==0){
                a,n,b;
                incrementer=false;
            }
            break;
        case 0x20:// ADD #
            j,a,n;
            A=A+RM; //microcode 12
            break;
        case 0x21:// SUB #
            k,a,n;
            A=A-RM; //microcode 12
            break;
        case 0x22:// NAND #
            l,a,n;
            A=!(A&&RM); //microcode 12
            break;
        case 0x40:// LOAD α
            a,n,f,g,n,c;
            break;  
        case 0x41:// OUT α
            a,n,f,g,n,i;
            printf("Sortie: %02X\n", out);
            break;   
        case 0x48:// STORE α
            a,n,f,g,d,o;
            break;
        case 0x49:// IN α
            a,n,f,g;
            printf("Entrez valeur: "); //microcode 16
            scanf("%hx", &RM);        //microcode 8
            o;
            break;
        case 0x60:// ADD α
            j,a,n,f,g,n;
            A=A+RM; //microcode 12
            break;
        case 0x61:// SUB α
            k,a,n,f,g,n;
            A=A-RM; //microcode 12
            break;
        case 0x62:// NAND α
            l,a,n,f,g,n;
            A=!(A&&RM); //microcode 12
            break;
        case 0xC0:// LOAD *α
            a,n,f,g,n,f,g,n,c;
            break;
        case 0xC1:// OUT *α
            a,n,f,g,n,f,g,n,i;
            printf("Sortie: %02X\n", out);
            break;
        case 0xC8:// STORE *α
            a,n,f,g,n,f,g,d,o;
            break;
        case 0xC9:// IN *α
            a,n,f,g,n,f,g;
            printf("Entrez valeur: ");  //microcode 16
            scanf("%hx", &RM);         //microcode 8
            o;
            break;
        case 0xE0:// ADD *α
            j,a,n,f,g,n,f,g,n;
            A=A+RM; //microcode 12
            break;
        case 0xE1:// SUB *α
            k,a,n,f,g,n,f,g,n;
            A=A-RM; //microcode 12
            break;
        case 0xE2:// NAND *α
            l,a,n,f,g,n,f,g,n;
            A=!(A&&RM); //microcode 12
            break;
        default:
            printf("Instruction inconnue: %02X\n", OP);
            exit(1);
    }

    if (incrementer){// Incrémenter PC : Phase 3
        p;
    }
    incrementer=true;
}

// Fonction principale
int main(int argc, char *argv[]) {
    FILE *file;
    signed short value;
    int index = 0;

    // Vérifier si le nom du fichier a été fourni
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Ouvrir le fichier en mode lecture
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    } 
    PC = 0x50;

    // Lire les valeurs hexadécimales du fichier et les stocker dans la mémoire
    while (fscanf(file, "%hx", &value) == 1) {
        
        if (index < MEM_SIZE) {
            memory[(int)PC+index] = (signed short)value;
            index++;
        } else {
            printf("Dépassement de mémoire, trop de valeurs dans le fichier.\n");
            exit(1);
            break;
        }
    }
    
    // Fermer le fichier
    fclose(file);

   

    // Exécution du programme
    while (PC<MEM_SIZE) {
        execute_programme();
        print_registers("après");
    }

    return 0;
}
