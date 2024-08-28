#include <iostream>
#include <cstdio>
#include <cstring>
#include <conio.h> 

using namespace std;

#define AÑO_ACTUAL 2024
#define AÑO_INICIO 1900

struct Usuario{
    char name[50];
    char username[50];
    int dni;
    char loginCode[50];
    int balance;
};

struct Transaccion{
    char name[50];
    int id;
    int monto;
    bool ingreso;
    int fecha; //aaaammdd
};


bool signIn(char username[50], char loginCode[50], Usuario &user);
Transaccion transaccion1(Transaccion operacion);
int menu();
void cargarValores(Transaccion &t, bool tipo);
void modificarBalance(Transaccion t, int &balance);
void listarTransacciones(Transaccion t);
void borrarTransaccion(int id);
int cantidadTransacciones();
void ordenarArchivo();

int main(){
    char username[50];
    char loginCode[50];
    int opcion;
    int id = 0;
    int flag = 0;
    Usuario user;
    Transaccion t = {"",0, 0, true, 0};

    
    do{
        if (flag == 1){
            cout << "Usuario o contraseña incorrecta"<< endl;
        }
        cout << "Ingrese su Usuario: ";
        cin>>t.name; 

        cout << "Ingrese el código de logueo: ";
        cin >> loginCode;

        flag = 1; 
    }while(!signIn(t.name, loginCode, user));
    


    
    do{
        cout<<endl <<"Balance $ "<<user.balance <<"   Usuario: "<<user.username <<endl <<endl;
        opcion = menu();
        switch(opcion){
            case 1:
                cargarValores(t, true);
                transaccion1(t);
                modificarBalance(t, user.balance);
                break;
            case 2:
                cargarValores(t, false);
                transaccion1(t);
                modificarBalance(t, user.balance);
                break;
            case 3:
                ordenarArchivo();
                listarTransacciones(t);
                break;
            case 4:
                cout<<"Ingrese el id a eliminar: ";
                cin>>id;
                borrarTransaccion(id);
                break;
            case 5:
                break;
            default:
                cout<<"Valor incorrecto"<< endl;
                break;
        }
    }while(opcion!=5);
    

    return 0;
}

int cantidadTransacciones(){
    Transaccion cantidad;
    int contador = 0;
    const char* nombreArchivo = "transacciones.dat";

    FILE * archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivo);
    }
    else{

        while(fread(&cantidad, sizeof(Transaccion), 1, archivo) == 1){
            contador++;
        }
        fclose(archivo);
    }
    return contador;
}

void ordenarArchivo() {
    int len = cantidadTransacciones();
    Transaccion actual;
    Transaccion despues;

    const char* nombreArchivo = "transacciones.dat";

    FILE *archivo = fopen(nombreArchivo, "r+b");
    if (archivo == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }
    for (int i = 0; i < len - 1; i++) {
        fseek(archivo, 0, SEEK_SET);

        for (int j = 0; j < len - i - 1; j++) {
            fread(&actual, sizeof(Transaccion), 1, archivo);
            fread(&despues, sizeof(Transaccion), 1, archivo);

            if (actual.id > despues.id) {
                fseek(archivo, -sizeof(Transaccion), SEEK_CUR);
                fseek(archivo, -sizeof(Transaccion), SEEK_CUR);

                fwrite(&despues, sizeof(Transaccion), 1, archivo);
                fwrite(&actual, sizeof(Transaccion), 1, archivo);

                fseek(archivo, -sizeof(Transaccion), SEEK_CUR);
            } else {
                fseek(archivo, -sizeof(Transaccion), SEEK_CUR);
            }
        }
    }
    fclose(archivo);
}

void borrarTransaccion(int id){
    Transaccion tId, aux;

    FILE* archivo = fopen("transacciones.dat", "rb+");
    if (archivo != NULL) {
        while (fread(&tId, sizeof(Transaccion), 1, archivo) == 1) {
            cout<<tId.id;
            if (tId.id == id) {
                tId = {"Borrado"};
                fseek(archivo, -sizeof(Transaccion), SEEK_CUR);
                fwrite(&tId, sizeof(Transaccion), 1, archivo);
                break;
            }
        }
        fclose(archivo);
    }
}
void modificarBalance(Transaccion t, int &balance){
    Usuario user;

    if(t.ingreso){
        balance = balance + t.monto;
    }
    else{
        if(balance<t.monto){
            cout<<"Saldo insuficiente"<< endl;
        }
        else{
            balance = balance - t.monto;
        }
    }
    cout<< "Su saldo actual es de: "<< balance<< endl<< endl;


    FILE* archivo = fopen("archivo.dat", "rb+");
    if (archivo != NULL) {
        while (fread(&user, sizeof(Usuario), 1, archivo) == 1) {
            if (strcasecmp(user.username, t.name) == 0) {
                user.balance = balance;
                fseek(archivo, -sizeof(Usuario), SEEK_CUR);
                fwrite(&user, sizeof(Usuario), 1, archivo);
                break;
            }
        }
        fclose(archivo);
    }

}

void cargarValores(Transaccion &t, bool tipo){
    int flag = 0;
    int mes, año;
    t.ingreso = tipo;

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
            cout<<"Ingrese la fecha: ";
            cin>> t.fecha;
        }
        int año = t.fecha / 10000;
        int mes = (t.fecha / 100) % 100;
        int dia = t.fecha % 100;

        if ((año> AÑO_ACTUAL) || (año < AÑO_INICIO) || (mes>12) || (mes<=0) || ( dia<=0)|| (dia>30)) {
            flag = 1;
            cout<<año<<endl<<mes<<endl<<dia;
        } 

    }while(flag);   
}

Transaccion transaccion1(Transaccion operacion){
    Transaccion idValidation;
    int maxMonto = 0;
    const char* nombreArchivo = "transacciones.dat";
    
    FILE * archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        operacion.id = 1;
    }
    else{       
        while (fread(&idValidation, sizeof(Transaccion), 1, archivo)) {
            if (idValidation.id > maxMonto) {
                maxMonto = idValidation.id;  // Actualizamos el máximo si encontramos uno mayor
            }
        }
        fclose(archivo);
    }

    operacion.id = maxMonto + 1;
    archivo = fopen(nombreArchivo, "ab");
    fwrite(&operacion, sizeof(Transaccion), 1, archivo);
    fclose(archivo);
    return operacion;
}

void listarTransacciones(Transaccion t){
    Transaccion idValidation;

    const char* nombreArchivo = "transacciones.dat";

    FILE * archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivo);
    }
    else{
        fseek(archivo, 0, SEEK_SET);
        while(fread(&idValidation, sizeof(Transaccion), 1, archivo) == 1){
            if(strcasecmp(idValidation.name, t.name) == 0){
                cout<<endl <<" El id de la transacción es: "<< idValidation.id<< endl;
                if(idValidation.ingreso){
                    cout<<" El monto ingresado fue de: "<< idValidation.monto<< endl;
                }
                else{
                    cout<<" El monto extraido fue de: "<< idValidation.monto<< endl;
                }
                cout<<" La fecha en que se realizo la transaccion: "<< idValidation.fecha<< endl;
            }
        }
        fclose(archivo);
    }
    cout<<endl<<endl;
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
    cout<<"     1. Ingresar dinero"<< endl;
    cout<<"     2. Egresar Dinero"<< endl;
    cout<<"     3. Listar transacciones"<<endl;
    cout<<"     4. Eliminar transaccion"<<endl;
    cout<<"     5. Salir"<<endl;
    cout << "Seleccione una opción: ";
    cin>>opcion;

    return opcion;
}

