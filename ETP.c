#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


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
    char id_alumno_prev [20] = "";
    char id_alumno_act [20];
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

struct Dominios* Gen_Dom(int cant_timeslots, int cant_examenes){
    struct Dominios* Dom =(struct Dominios*) calloc(cant_examenes, sizeof(Dominio));
    for (int h =0; h < cant_examenes; h++){
        Dom[h].Dom = (int*)calloc(cant_timeslots,sizeof(int*));
        for (int g=0; g<cant_timeslots; g++){
            Dom[h].Dom[g]=g+1;
        }
        Dom[h].cantidad = cant_timeslots;
    }
    return Dom;
}

struct Dominios* Copy_Dom(int i,int c, int E, struct Dominios* Dom){
    struct Dominios* D = (struct Dominios*) calloc(E, sizeof(Dominio));
    for (int j = 0; j<E; j++){
        D[j].Dom = (int*) calloc(c, sizeof(int*));
        for (int n=0; n<c; n++){
            D[j].Dom[n]=Dom[j].Dom[n];
        }
        D[j].cantidad=Dom[j].cantidad;
    }
    return D;
}

int Check_error(int E, struct Dominios* Dom, int i){
    int bin=0;
    for (int j=0;j<E;j++){
        if(Dom[j].cantidad==0){
            bin=1;
            break;
        }
    }
    return bin;
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

void free_Dom(struct Dominios* D, int E){
    for (int n=0; n<E; n++){
        free(D[n].Dom);
        
    }
    free(D);
}

int SelectValueFC(int i, int E, struct Dominios* Dom, int** M_Conflic, int c, int* orden, unsigned int* check){
    int select=0;
    int empty_dom;
    while(Dom[orden[i]].cantidad != 0){
        struct Dominios* Copy_1 = Copy_Dom(i,c, E, Dom);
        
        select = pop(&Dom[orden[i]], 0);
        
        empty_dom =0;                   // boolean 0 == False
        for (int k = i+1; k < E; k++){
             if (M_Conflic[orden[i]][orden[k]]==1){    //checkea si el valor a instanciar con el otro valor tienen un conflicto
                for(int posi=0; posi<Dom[orden[k]].cantidad; posi++){
                    *check=*check+1;                      //si es así se van a checkquear
                    if (select == Dom[orden[k]].Dom[posi]){
                        delete(&Dom[orden[k]], posi);
                    };
                }
                
            }
            if (Dom[orden[k]].cantidad == 0){
                empty_dom=1;
                
            }
        }
        if (empty_dom == 1){
            for(int f =i+1; f<E; f++){
                if (M_Conflic[orden[i]][orden[f]]==1){
                    for (int e=0; e<Copy_1[orden[f]].cantidad; e++){
                    Dom[orden[f]].Dom[e]=Copy_1[orden[f]].Dom[e];
                    }
                Dom[orden[f]].cantidad=Copy_1[orden[f]].cantidad;
                }
            }
            free_Dom(Copy_1,E);
        }
        else{
            free_Dom(Copy_1, E);
            return select;
        }
        
    }
    
    return 0;
}

int* ForwardChecking (int *sol,int** M_Confl, struct Dominios* Dom, int* orden, int E, int c){
    clock_t begin = clock(); 
    int x_i;
    int i=0;
    unsigned int check=0;    //contador de la cantidad de chequeos realizados
    struct Dominios* Copy[E];
    for(int f=0; f<E;f++){
        Copy[E]=0;
    }
    while(i<E && i>=0){
        
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
        if(time_spent/60 >2){
            i=-1;
            break;
        }
        Copy[i] = Copy_Dom(i,c, E, Dom);
        
        x_i = SelectValueFC(i, E, Dom, M_Confl, c, orden, &check);
       
       
       //write_DOM(Dom,E,x_i,i, 0); 
        if (x_i==0){      //estamos en un fallo, por lo que es necesario retroceder
            sol[orden[i]]= x_i;
          
            i--;       //se rZZetrocede a la variable anterior
            if(i<0){
                break;
            }
            int j=i;
            while(M_Confl[orden[i]][orden[j]]!=1){
                j--;
                if(j<0){
                    j=i;
                    break;
                }
            }
            i=j;
            
            
            for(int f =i+1; f<E; f++){                       //se restablecen los dominios a como estaban antes de instanciar la variable x_i  
                    for (int e=0; e<Copy[i][orden[f]].cantidad; e++){
                        Dom[orden[f]].Dom[e]=Copy[i][orden[f]].Dom[e];
                        }
                    Dom[orden[f]].cantidad=Copy[i][orden[f]].cantidad;
            }
           ;
           free_Dom(Copy[i],E);
            
            
         
        }
        else{
            sol[orden[i]]= x_i;
            i++;
        }
       
    }
    
    printf("Con %d Timeslots se realizaron %d chequeos \n",c,check);
    
    if (i<=0){      //no se pudo encontrar una solución factible.
        sol[0]=0;  //se modifica la solucion dando a entender que no se encontró una solucion factible
            
        return sol;
    }
    else{
        for(int f=0; f<E;f++){
            free_Dom(Copy[f],E);  //se libera la memoria
        }
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

int* orden(int **Matriz_C, int E){ //orden de instanciación de las variables desde la que presenta más confictos hacía abajo
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

int* orden_comun(int **Matriz_C, int E){ //orden de instanciacion de las variables de manera ascendente (1,2,3,4,5...E)
    int* orden = (int*) calloc(E,sizeof(int));
    for (int i=0; i<E; i++){
        orden[i]=i;
    }
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

void write_penalizacion(int p, char file[]){
    char buffer [100];
    strcpy(buffer, file);
    strcat(buffer, ".pen");
    FILE * fp;
    char str1 [100] = "instancias/resultados/" ; 
    strcat(str1, buffer);
    fp = fopen(str1, "wt");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    fprintf(fp,"%d \n",p);

    fclose(fp);

}

int penalizacion(int*solucion, int E, char file[]){
    int dif=0;
    int pen=0;
    int pen_prom=0;
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
    char id_alumno_prev [20] = "";
    char id_alumno_act [20];
    int id_examen_act;
    int cant_alumnos=1;
    int not =0;
    int array[E];
    for (int n=0; n<E;n++){
        array[n]=0;
    }
    while (feof(fp) ==0){
        
        fscanf(fp, "%s %d", id_alumno_act, &id_examen_act);
        
        if (strcmp(id_alumno_act, id_alumno_prev) == 0 || strcmp("", id_alumno_prev) == 0){
            array[id_examen_act-1]=1;
        }
        else{
            cant_alumnos++;
            for(int n =0; n<E; n++){
                if (array[n]==1){
                    for(int q=n+1; q<E; q++){
                        if(array[q]==1){
                        dif = fabs(solucion[n]-solucion[q]);
                        
                        if(dif ==0){
                            not=1;
                        }
                        
                        if(dif ==1){
                           pen=pen+16;
                        }
                        if(dif ==2){
                            pen=pen+8;
                        }
                        if(dif ==3){
                            pen=pen+4;
                        }
                        if(dif ==4){
                            pen=pen+2;
                        }
                        if(dif ==5){
                            pen=pen+1;
                        }
                        }
                    }
                }
            }
            for (int n=0; n<E;n++){
            array[n]=0;
            }   
        }
    
    array[id_examen_act-1]=1;
    strcpy(id_alumno_prev,id_alumno_act);

    }

    fclose(fp);
    if(not ==1){
        printf("ERROR \n");
    }
    pen_prom=pen_prom+pen;
    pen_prom = pen_prom/cant_alumnos;
    return pen_prom;
}

void write_DOM(struct Dominios* D, int E, int elecc, int var, int el){
    FILE * fp;
    fp = fopen("instancias/resultados/Dom.dom", "a+");
    if (fp==NULL){
        fputs("FILE error \n", stderr);
        exit(1);
    }
    fprintf(fp,"Para la variable %d se eligió %d \n", var+1, elecc);
    if(el ==1){
        fprintf(fp,"post select value \n");
    }
    if(el ==2){
        fprintf(fp,"Post Reseteo \n");
    }
    if (el==3){
        fprintf(fp, "SOLUCIONAO \n");
    }
    for(int i=0; i<E; i++){
        fprintf(fp,"Para la variable %d \n", i+1);
        for(int j=0;j<D[i].cantidad; j++){
            fprintf(fp,"%d \t",D[i].Dom[j]);
        }
        fprintf(fp,"\n----------------------------------------- \n");
    }
    

    fclose(fp);

}


int main (){  
    clock_t begin = clock(); 

    char name_Archivo [20];
    printf("Ingrese el nombre del archivo de prueba (debe encontrarse en la carpeta instances): ");
    scanf("%s", name_Archivo);
    int a;                                          //menor valor posible dentro del intervalo de búsqueda
    int E= leer_examenes(name_Archivo);             //cantidad de exámenes
    int **C = crear_matriz(E, name_Archivo, &a);    //matriz de conflictos
    int b = E;                                      //máximo valor posible dentro del intervalo de búsqueda
    int c;
    int* o1;
    int *sol = inicializar_sol(E);                  
    int fin[E];                                     //solucion final, con la menor cantidad de time slots utilizados    
    int ciclo = 0;                                  //binario para detener el while
    int p;                                          //penalizacion promedio de la solucion
    int d=1; 
    printf("1. Orden secuencial de las variables \n2. Orden del examen más instanciado primero \n");
    printf("Ingrese su elección:");
    scanf("%d", &d);
        
    if(d==1){
        o1= orden_comun(C, E);                     //orden de instanciación de las variables
    }
    if(d==2){
        o1= orden(C, E);                     //orden de instanciación de las variables
    }

    while (ciclo != 1){                             
        if(a==b){                                   //encontraron el último número a comparar
            ciclo=1;
        }
        c = floor((a+b)/2);                         //cantidadd de timeslot a utilizar
        printf("------------------------------------------------------------------------ \n");
        printf("a:%d \tb: %d \tc: %d \n",a,b,c);

        struct Dominios *Dominios = Gen_Dom(c,E);     //se generan los dominios con la cantidad de timeslots previamente calculada

        sol = ForwardChecking(sol, C, Dominios, o1,E,c);      //se realiza el FC
        free(Dominios);

        if (sol[0] !=0){            //el FC encontró una solución factible
            b = c-1;                //se descartan todos los números mayores a la cantidad de time slots utilizada
        if(b<a){             
            b=a;
        }
        for (int aux=0; aux<E; aux++){      //se copia la solucion obtenida a la solucion final
            fin[aux]=sol[aux];
        }

        }else{
            printf("No hay solucion con %d Timeslots \n", c);
            free(sol);
            a = c+1;          //al no encontrar solucion se descartan todos los números menores a la cantidad de time slots utilizada
            sol = inicializar_sol(E);
        }

    }   

    p = penalizacion(fin, E, name_Archivo);
    write_solution(fin, name_Archivo,E);
    write_timeslots(a, name_Archivo);
    write_penalizacion(p, name_Archivo);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; 
    printf("tiempo de ejecución: %f  [s]\n",time_spent);
    
    return 0 ; 
}
