#include <iostream>
#include <cstdio>
#include <cstring> 

using namespace std;

struct Usuario{
    char name[50];
    char username[50];
    int dni;
    char loginCode[50];
    int balance;
};

struct Transaccion{
    int id;
    int monto;
    bool ingreso;
    int fecha; //ddmmaaaa
};


bool signIn(char username[50], char loginCode[50], Usuario &user);
Transaccion transaccion1(Transaccion operacion);
int menu();
void cargarValores(Transaccion &t);

int main(){
    char username[50];
    char loginCode[50];
    int opcion;
    int flag = 0;
    Usuario user;
    Transaccion t = {0, 0, true, 0};

    
    do{
        if (flag == 1){
            cout << "Usuario o contraseña incorrecta"<< endl;
        }
        cout << "Ingrese su Usuario: ";
        cin >> username; 

        cout << "Ingrese el código de logueo: ";
        cin >> loginCode;

        flag = 1; 
    }while(!signIn(username, loginCode, user));
    
    cout<< "Balance actual de la cuenta es de $ "<< user.balance <<endl;

    do{
        opcion = menu();
        switch(opcion){
            case 1:
                cargarValores(t);
                transaccion1(t);
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                cout<<"Valor incorrecto"<< endl;
                break;
        }
    }while(opcion!=5);
    

    return 0;
}

void cargarValores(Transaccion &t){
    int flag = 0;
    int mes, año;

    do{
        if(flag){
            cout<< "El monto no puede ser negativo: ";
            flag = 0;
        }
        else{
            cout<< "Ingrese el monto: ";
        }
        cin>> t.monto;
        flag = 1;
    }while(t.monto<= 0);

    flag = 0;
    
    do{
        if(flag==1){
            cout<<"La fecha es incorrecta: "<< endl;    
            cin>> t.fecha;
            flag = 0;
        }
        else{
            cout<<"Ingrese la fecha: "<< endl;
            cin>> t.fecha;
        }
        mes = t.fecha % 1000000;
        mes = mes / 10000;
        año = t.fecha % 10000; 
        if ((t.fecha / 1000000 > 31) || (t.fecha / 1000000 <= 0) || (mes>12) || (mes<=0) || ( año>9999)|| (año<=0)) {
            flag = 1;
        } 

    }while(flag);   
}

Transaccion transaccion1(Transaccion operacion){
    Transaccion idValidation;
    const char* nombreArchivo = "transacciones.dat";
    
    FILE * archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        operacion.id = 1;
        fclose(archivo);
    }
    else{
        fseek(archivo, -sizeof(Transaccion), SEEK_END);
        if(fread(&idValidation, sizeof(Transaccion), 1, archivo) == 1){
            operacion.id=idValidation.id + 1;
            fclose(archivo);
        }
    }


    archivo = fopen(nombreArchivo, "ab");
    fwrite(&operacion, sizeof(Transaccion), 1, archivo);
    fclose(archivo);
    return idValidation;
}

bool signIn(char username[50],char loginCode[50], Usuario &user) {
    bool ingresoValido = false;

    FILE* archivo = fopen("archivo.dat", "rb");
    if (archivo != NULL) {
        while (fread(&user, sizeof(Usuario), 1, archivo) == 1) {
            if (strcasecmp(user.username, username) == 0) {
                if (strcasecmp(user.loginCode, loginCode) == 0) {
                    ingresoValido = true;
                }
                break;
            }
        }
        fclose(archivo);
    }
    return ingresoValido;
}

int menu(){
    int opcion = 0;
    cout<<"1. Ingresar dinero"<< endl;
    cout<<"2. Egresar Dinero"<< endl;
    cout<<"3. Listar transacciones"<<endl;
    cout<<"4. Eliminar transaccion"<<endl;
    cout<<"5. Salir"<<endl;
    cout << "Seleccione una opción: ";
    cin>>opcion;

    return opcion;
}

