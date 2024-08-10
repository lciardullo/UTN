#include <iostream>
#include <cstdio>
#include <cstring>
#include <conio.h> 

using namespace std;

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
    int fecha; //ddmmaaaa
};


bool signIn(char username[50], char loginCode[50], Usuario &user);
Transaccion transaccion1(Transaccion operacion);
int menu();
void cargarValores(Transaccion &t, bool tipo);
void modificarBalance(Transaccion t, int &balance);
void listarTransacciones(Transaccion t);

int main(){
    char username[50];
    char loginCode[50];
    int opcion;
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
    
    cout<< "Balance actual de la cuenta es de $ "<<user.balance <<endl;

    cout<<"El usuario es: "<<t.name <<endl <<endl;
    
    do{
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
                listarTransacciones(t);
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

void modificarBalance(Transaccion t, int &balance){
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
        }

    }


    archivo = fopen(nombreArchivo, "ab");
    fwrite(&operacion, sizeof(Transaccion), 1, archivo);
    fclose(archivo);
    return idValidation;
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
        cout<<"Presione cualquier letra para volver al menu.";
        getch();
        cout<<endl;
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

