int a_neltos = 0;
int *a_values[100];
char *a_keys[100];

int buscar(char* nombre){
    int index = -1;

    for (int i = 0; i < a_neltos; i++){
        if (strcmp(a_keys[i], nombre)){
            index = i;
            break;
        }
    }
    return index;
}

int main(){
    
}