#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Dominio
{
    int* Dom;
    int cantidad;

};

int leer_examenes(char file []){  //lee los archivos .exm
    int n = 0;
    char buffer [100];
    strcpy(buffer, file);
    strcat(buffer, ".exm");
    FILE * fp;
    char str1 [100] = "instancias/instances/" ; 
    strcat(str1, buffer);
    fp = fopen(str1, "r");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    int id_examen;
    int cant_alumnos;
    while (feof(fp) ==0){
        fscanf(fp, "%d %d", &id_examen, &cant_alumnos);
        n++;
        
    };
    fclose(fp);
    return n;

};


int **crear_matriz(int cant_examenes, char file [], int* a){
    int **Matriz = (int**)malloc(cant_examenes*sizeof(int*));
    for (int i =0; i < cant_examenes; i++){
        Matriz[i] = (int*)malloc(cant_examenes*sizeof(int*));
        for (int j=0; j<cant_examenes; j++){
            Matriz[i][j]=0;
        }
    }


    char buffer [100];
    strcpy(buffer, file);
    strcat(buffer, ".stu");
    FILE * fp;
    char str1 [100] = "instancias/instances/" ; 
    strcat(str1, buffer);
    fp = fopen(str1, "r");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    char id_alumno_prev [5] = "";
    char id_alumno_act [5];
    int id_examen_act;
    int id_examen_prev =-1;
    int conf_alumnos=0;
    int max_a=0;
    while (feof(fp) ==0){
        fscanf(fp, "%s %d", id_alumno_act, &id_examen_act);
        if (strcmp(id_alumno_act, id_alumno_prev) == 0){
            Matriz[id_examen_prev-1][id_examen_act-1] =1;
            Matriz[id_examen_act-1][id_examen_prev-1] =1;
            conf_alumnos++;

        }
        else{
            //printf("Alumno %s tiene %d conflictos \n ----------------------------- \n", id_alumno_act, conf_alumnos);
            strcpy(id_alumno_prev, id_alumno_act); 
            if (max_a<conf_alumnos){
                max_a= conf_alumnos;
                
            }
            conf_alumnos= 0;
        }
        id_examen_prev = id_examen_act;

        
    };
    fclose(fp);
    *a = max_a;
    printf("El alumno con más examenes tiene: %d \n", max_a);
    return Matriz;

}



void leer_dominios(struct Dominio* D, int c, int E){

    for (int exm=0; exm<E; exm++){
        printf("\n------------------------------- \n");
        printf("Para el examen %d \n", exm+1);
        for (int slot=0; slot<c; slot++){
            printf("%d \t", D[exm].Dom[slot]);
        }
    }
    printf("\n");
}


struct Dominio* Gen_Dom(int cant_timeslots, int cant_examenes){
    struct Dominio* Dom = (struct Dominio*)malloc(cant_examenes*sizeof(struct Dominio*));
    for (int i =0; i < cant_examenes; i++){
        Dom[i].Dom = (int*)malloc(cant_timeslots*sizeof(int*));
        for (int j=0; j<cant_timeslots; j++){
            Dom[i].Dom[j]=j+1;
        }
        printf("%d \t", Dom[0].Dom[0]);
        Dom[i].cantidad = cant_timeslots;
    }
    printf("\n %d", Dom[0].Dom[0]);
    return Dom;
}

int * inicializar_sol(int E){
    int *sol = (int *) malloc(E*sizeof(int));
    for (int i=0; i<E; i++){
        sol[i] = 0;
    }
    return sol;
}



void elimiar_elemento_dominio(struct Dominio* D, int pos, int ID_exm){
    for (int i=pos; i<D[ID_exm-1].cantidad-1; i++){
        D[ID_exm-1].Dom[i] = D[ID_exm-1].Dom[i+1];
    }
    D->cantidad--;
}


void free_memory(int **C, struct Dominio* D, int E){
    for (int n=0; n<E; n++){
        free(C[n]);
        
    }
    free(C);
    free(D);
}

int main (){
    char name_Archivo [20];
    printf("Ingrese el nombre del archivo de prueba (debe encontrarse en la carpeta instances): ");
    scanf("%s", name_Archivo);
    int a; 
    int E= leer_examenes(name_Archivo);
    int **C = crear_matriz(E, name_Archivo, &a);
    int b = E;
    int c;
    struct Dominio *Dominios = Gen_Dom(5,E);
    printf("%d \n",Dominios[0].Dom[0]);
    leer_dominios(Dominios, 5, E);
    //elimiar_elemento_dominio(Dominios, 0,0);
    //leer_dominios(Dominios, 10, 5);
    /*while (a != b)
    {
        c = floor((a+b)/2);
        int **Dominios = Gen_Dom(c,E);
        int* sol = FC(C, Dominios, orden);
        if sizeof(sol) != 0{
            b = c-1;
        }else{
            a = c+1;
        }
    } */
    free_memory(C, Dominios, E);
    return 0 ; 
}
