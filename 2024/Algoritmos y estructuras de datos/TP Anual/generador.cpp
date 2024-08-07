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
    long int id;
    int monto;
    bool ingreso;
    int fecha; //ddmmaaaa
};


bool signIn(char username[50], char loginCode[50], Usuario &user);

int main(){
    char username[50];
    char loginCode[50];
    int flag = 0;
    Usuario user;
    
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
    

    return 0;
}

Transaccion agregartransaccion(Transaccion operacion){
    
    const char* nombreArchivo = "transacciones.dat";
    FILE* archivo = fopen(nombreArchivo, "ab");
    if (archivo == NULL) {
        cout << "Error al abrir el archivo." << endl;
        return operacion;
    }
    fwrite(&operacion, sizeof(Usuario), 1, archivo);
    fclose(archivo);
    return operacion;
}

bool signIn(char username[50],char loginCode[50], Usuario &user) {
    bool ingresoValido = false;

    FILE* archivo = fopen("archivo.dat", "rb");
    if (archivo != nullptr) {
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

