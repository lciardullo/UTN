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

bool validateUsername(char username[50]);
bool validateDni(int dni);
Usuario signUp();

bool validateUsername(char username[50]){
    Usuario user2;
    bool encontrado = false;

    // Abrir el archivo en modo lectura binaria
    FILE* archivo = fopen("archivo.dat", "rb");
    if (archivo != nullptr) {
        while (fread(&user2, sizeof(Usuario), 1, archivo) == 1) {
            if (strcmp(user2.username,username) == 0) {
                encontrado = true;
                break; // Se encontró el DNI, no necesitamos seguir buscando
            }
        }

        fclose(archivo);
    }

    if (encontrado) {
        cout << "El Username " << username << " está en uso." << endl;}

    return encontrado;
}

bool validateDni(int dni) {
    Usuario user2;
    bool encontrado = false;

    // Abrir el archivo en modo lectura binaria
    FILE* archivo = fopen("archivo.dat", "rb");
    if (archivo != nullptr) {
        while (fread(&user2, sizeof(Usuario), 1, archivo) == 1) {
            if (user2.dni == dni) {
                encontrado = true;
                break; // Se encontró el DNI, no necesitamos seguir buscando
            }
        }

        fclose(archivo);
    }

    if (encontrado) {
        cout << "El DNI " << dni << " está en uso." << endl;}

    return encontrado;
}

Usuario signUp(){
    Usuario user;
    bool nombreValido = false;
    bool dniValido = false;
    bool usernameValido = false;
    
    
    //Valido que el nombre sea valido
    do{
        cout<<"Ingrese su nombre completo: ";
        cin.ignore();
        cin.getline(user.name, 50);

        nombreValido = true;
        for(int i = 0; i < strlen(user.name) ; i++){
            if (!isalpha(user.name[i]) && !isspace(user.name[i])){
                cout << "     El siguiente caracter no es valido en un nombre: " << user.name[i] << endl;
                nombreValido = false;
                break;
            }
        }
    } while(!nombreValido);
    
    do{
        cout<<"Ingrese su DNI: ";
        cin >> user.dni;


        dniValido = validateDni(user.dni);
    }while(dniValido);
        

    do{
        cout<<"Ingrese su Usuario: ";
        cin.ignore();
        cin>>user.username;

        nombreValido = true;
        for(int i = 0; i < strlen(user.username) ; i++){
            if (isspace(user.username[i])){
                cout << "     El usuario no debe contener espacios " << user.username[i] << endl;
                nombreValido = false;
                break;
            }
        }
        usernameValido = validateUsername(user.username);
    } while(!nombreValido || usernameValido);


    do{
        cout<<"Ingrese el código de logueo: ";
        cin>>user.loginCode;

        nombreValido = true;
        for(int i = 0; i < strlen(user.loginCode) ; i++){
            if (isalpha(user.loginCode[i]) || isspace(user.loginCode[i])){
                cout << "     El código no debe contener letras ni espacios: " << user.loginCode[i] << endl;
                nombreValido = false;
                break;
            }
        }
    } while(!nombreValido);

    user.balance = 10000;

    const char* nombreArchivo = "archivo.dat";
    FILE* archivo = fopen(nombreArchivo, "ab");
    if (archivo == NULL) {
        cout << "Error al abrir el archivo." << endl;
        return user;
    }
    fwrite(&user, sizeof(Usuario), 1, archivo);
    fclose(archivo);

    return user;
}




