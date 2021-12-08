#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Dominios
{
    int* Dom;
    int cantidad;

}Dominio;


void leer_matriz(int** M, int E){
    for(int i=0; i<E; i++){
        for(int j=0; j<E; j++){
            printf("%d\t",M[i][j]);
        }
        printf("\n");
    }
}


void imprimir_matriz(int** M, int E){
    char buffer [100];
    strcpy(buffer, "Matriz");
    strcat(buffer, ".txt");
    FILE * fp;
    char str1 [100] = "instancias/resultados/" ; 
    strcat(str1, buffer);
    fp = fopen(str1, "wt");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    for(int i=0; i<E; i++){
        for(int j=0; j<E; j++){
            fprintf(fp,"%d\t",M[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}



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
    int conf_alumnos=1;
    int max_a=0;
    int array[cant_examenes];
    for (int n=0; n<cant_examenes;n++){
        array[n]=0;
    }
    while (feof(fp) ==0){
        fscanf(fp, "%s %d", id_alumno_act, &id_examen_act);
        
      
        if (strcmp(id_alumno_act, id_alumno_prev) == 0 || strcmp("", id_alumno_prev) == 0){
            if(strcmp("", id_alumno_prev) != 0){
                conf_alumnos++;
            }
            
            strcpy(id_alumno_prev, id_alumno_act); 
            array[id_examen_act-1]=1;
            
            

        }
        
        else{
            
            for(int n =0; n<cant_examenes; n++){
                if (array[n]==1){
                    for(int q=n+1; q<cant_examenes; q++){
                    if(array[q]==1){
                        Matriz[n][q]=1;
                        Matriz[q][n]=1;
                    }
                    
                }
                }
            }
            for (int n=0; n<cant_examenes;n++){
                array[n]=0;
             }
            array[id_examen_act-1]=1;    
            strcpy(id_alumno_prev, id_alumno_act); 
            if (max_a<conf_alumnos){
                max_a= conf_alumnos;
                
            }
            conf_alumnos= 1;
        }
        

        
    }
    for(int n =0; n<cant_examenes; n++){
                if (array[n]==1){
                    for(int q=n+1; q<cant_examenes; q++){
                    if(array[q]==1){
                        Matriz[n][q]=1;
                        Matriz[q][n]=1;
                    }
                    
                }
                }
            }

    fclose(fp);
    *a = max_a;
    printf("El alumno con más examenes tiene: %d \n", max_a);
    return Matriz;

}



void leer_dominios(struct Dominios* D, int c, int E){

    for (int exm=0; exm<E; exm++){
        printf("\n------------------------------- \n");
        printf("El examen %d tiene dominio de tmbo %d que es:  \n", exm+1, D[exm].cantidad);
        for (int slot=0; slot<D[exm].cantidad; slot++){
            printf("%d \t", D[exm].Dom[slot]);
        }
    }
    printf("\n");
}


struct Dominios* Gen_Dom(int cant_timeslots, int cant_examenes){
    struct Dominios* Dom = malloc(cant_examenes* sizeof(Dominio));
    for (int i =0; i < cant_examenes; i++){
        Dom[i].Dom = (int*)malloc(cant_timeslots*sizeof(int*));
        for (int j=0; j<cant_timeslots; j++){
            Dom[i].Dom[j]=j+1;
        }
        Dom[i].cantidad = cant_timeslots;
    }
    return Dom;
}

void reset_domains(int** Matriz_c,struct Dominios* D, int E, int i, int* orden, int del){
    for(int k = i+1; k <E; k++){
        if(Matriz_c[orden[i]][orden[k]]==1){
            D[orden[k]].Dom[D[orden[k]].cantidad]=del;
            D[orden[k]].cantidad++;
        }
        
    }
}

void insert_value(struct Dominios* D, int pos, int insert){
    D[pos].Dom[D[pos].cantidad]=insert;
    D[pos].cantidad++;


}

int* inicializar_sol(int E){
    int *sol = (int *) malloc(E*sizeof(int));
    for (int i=0; i<E; i++){
        sol[i] = 0;
    }
    return sol;
}



int pop(struct Dominios* D, int pos){
    int b = D->Dom[pos];
    for (int i=pos; i<D->cantidad-1; i++){
        D->Dom[i] = D->Dom[i+1];
    }
    D->cantidad --;
    return b;
}

void delete(struct Dominios* D, int pos){
    for (int i=pos; i<D->cantidad-1; i++){
        D->Dom[i] = D->Dom[i+1];
    }
    D->cantidad --;
}


void free_memory(int **C, struct Dominios* D, int E){
    for (int n=0; n<E; n++){
        free(C[n]);
        
    }
    free(C);
    free(D);
}

int SelectValueFC(int i, int E, struct Dominios* Dom, int** M_Conflic, int c, int* orden, int* check){
    int select=0;
    int empty_dom;
    while(Dom[orden[i]].cantidad != 0){
        select = pop(&Dom[orden[i]], 0);
        //printf("Para %d se elige %d \n", orden[i]+1,select);
        empty_dom =0;                   // boolean 0 == False
        for (int k = i+1; k < E; k++){
            for(int posi=0; posi<Dom[orden[k]].cantidad; posi++){
                if (M_Conflic[orden[i]][orden[k]]==1){
                    *check=*check+1;
                    if (select == Dom[orden[k]].Dom[posi]){
                    delete(&Dom[orden[k]], posi);
                };
                }
                
            }
            if (Dom[orden[k]].cantidad == 0){
                empty_dom=1;
                break;
            }
        }
        if (empty_dom == 1){
            
            reset_domains(M_Conflic,Dom,E, i, orden, select);
            
        }
        else{
            return select;
        }
        
    }
    insert_value(Dom, orden[i], select);
    return 0;
}



int* ForwardChecking (int *sol,int** M_Confl, struct Dominios* Dom, int* orden, int E, int c){
    int x_i;
    int i=0;
    int check=0;
    while(i<E){

        x_i = SelectValueFC(i, E, Dom, M_Confl, c, orden, &check);
        if (x_i==0){      //no encontró valor, debo retroceder
            
            if(i==0){
                break;
            }
            i--;
            //printf("pos %d val: %d\n",i, sol[orden[i]]);
            reset_domains(M_Confl, Dom, E, i, orden, sol[orden[i]]);
            
        }
        else{
            sol[orden[i]]= x_i;
            i++;
        }
    }
    printf("Con %d Timeslots se realizaron %d chequeos \n",c,check);
    if (i==0){
        sol[0]=0;
        return sol;
    }
    else{
        return sol;
    }

    
}

void intercambiar(int *a, int *b) {
  int temporal = *a;
  *a = *b;
  *b = temporal;
}


int particion(int arreglo[], int arreglo2[], int izquierda, int derecha) {
  int pivote = arreglo[izquierda];
  while (1) {
    while (arreglo[izquierda] < pivote) {
      izquierda++;
    }
    while (arreglo[derecha] > pivote) {
      derecha--;
    }
    if (izquierda >= derecha) {
      return derecha;
    } else {
      intercambiar(&arreglo[izquierda], &arreglo[derecha]);
      intercambiar(&arreglo2[izquierda], &arreglo2[derecha]);
      izquierda++;
      derecha--;
    }
    
  }
}

void quicksort(int arreglo[], int arreglo2[], int izquierda, int derecha) {
  if (izquierda < derecha) {
    int indiceParticion = particion(arreglo, arreglo2, izquierda, derecha);
    quicksort(arreglo, arreglo2, izquierda, indiceParticion);
    quicksort(arreglo, arreglo2, indiceParticion + 1, derecha);
  }
}


int* orden(int **Matriz_C, int E){
    int* orden = (int*) malloc(E*sizeof(int));
    int cant_choques[E];
    int choques;
    for (int i =0; i<E; i++){
        choques =0;
        for (int j=0; j<E; j++){
            if(Matriz_C[i][j]==1){
                choques++;
            }
        }
        orden[i] = i;
        cant_choques[i]=choques;
    }
    quicksort(cant_choques, orden,0,E-1);
    return orden;

}


void write_solution(int*sol, char file[],int E){
    char buffer [100];
    strcpy(buffer, file);
    strcat(buffer, ".sol");
    FILE * fp;
    char str1 [100] = "instancias/resultados/" ; 
    strcat(str1, buffer);
    fp = fopen(str1, "wt");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    for(int i=0; i<E; i++){
        fprintf(fp,"%d %d \n",i+1, sol[i]);

    }
    fclose(fp);

}

void write_timeslots(int c, char file[]){
    char buffer [100];
    strcpy(buffer, file);
    strcat(buffer, ".res");
    FILE * fp;
    char str1 [100] = "instancias/resultados/" ; 
    strcat(str1, buffer);
    fp = fopen(str1, "wt");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    fprintf(fp,"%d \n",c);

    fclose(fp);

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
    int* o1= orden(C, E);
    int *sol = inicializar_sol(E);
    int fin[E];
    int ciclo = 0;
    while (ciclo != 1){
        if(a==b){
            ciclo=1;
        }
        c = floor((a+b)/2);
        printf("a:%d \tb: %d \tc: %d\n",a,b,c);
        struct Dominios *Dominios = Gen_Dom(c,E);
        sol = ForwardChecking(sol, C, Dominios, o1,E,c);
        if (sol[0] !=0){
            b = c-1;
            for (int aux=0; aux<E; aux++){
                fin[aux]=sol[aux];
            }
            char snum [20];
            sprintf(snum, "%d", c);
            write_solution(fin,snum,E);
        }else{
            printf("No hay solucion con %d Timeslots \n", c);
            free(sol);
            a = c+1;
            sol = inicializar_sol(E);
        }
        
        
    }
    write_solution(fin, name_Archivo,E);
    write_timeslots(a, name_Archivo);
    
    return 0 ; 
}
