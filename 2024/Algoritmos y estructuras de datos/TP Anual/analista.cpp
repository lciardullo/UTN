#include <iostream>
#include <cstring>
#include <conio.h>

using namespace std;

#define AÑO_ACTUAL 2024
#define AÑO_INICIO 1900
#define FECHA_ACTUAL 20241212

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

int menu();
int seleccionarMes();
int ingresarAño();
void mostrarMontoMaximo();
void listarCantidadIngresoEgreso(char t[50], int mes, int año);
void seleccionarUsuario(char t[50]);
bool validarCliente(char username[50]);
void listarTransacciones(char t[50]);
int cantidadTransaccionesCliente(char name[50]);
int diaInicial(int fechaActual);
void maximaTransaccionUltimos30Dias();
int cantidadTransacciones(char name[50]);
void ordenarArchivo();


int main(){
    int opcion;
    char t[50];

    do{
        opcion = menu();

        switch(opcion){
            case 1:
                seleccionarUsuario(t);
                ordenarArchivo();
                listarTransacciones(t);
                break;
            case 2:
                seleccionarUsuario(t);
                listarCantidadIngresoEgreso(t, ingresarAño(), seleccionarMes());
                break;
            case 3:
                mostrarMontoMaximo();
                break;
            case 4:
                maximaTransaccionUltimos30Dias();
                break;
            case 5:
                break;
            default:
                cout<<"Valor incorrecto";
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

            if (actual.fecha < despues.fecha) {
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

void maximaTransaccionUltimos30Dias(){
    Transaccion monto;
    Usuario user;
    char userAux[50];
    Transaccion montoMax;
    int contador = 0;
    int contadorAux = 0;
    int flag = 0;
    int fechaActual = FECHA_ACTUAL;
    int fechaUltimos30Dias = diaInicial(fechaActual);

    const char* nombreArchivoGestor = "archivo.dat";
    FILE * archivoGestor= fopen(nombreArchivoGestor, "rb");

    const char* nombreArchivo = "transacciones.dat";
    FILE * archivo = fopen(nombreArchivo, "rb");
    
    if (archivo == NULL) {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivo);
    }
    else if (archivoGestor == NULL)
    {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivoGestor);   
    }
    else{
        fseek(archivo, 0, SEEK_SET);
        while(fread(&user, sizeof(Usuario), 1, archivoGestor) == 1){
            fseek(archivo, 0, SEEK_SET);
            contador = 0;
            while(fread(&monto, sizeof(Transaccion), 1, archivo) == 1){
                if((strcasecmp(monto.name, user.username) == 0) && (monto.ingreso) && (monto.fecha <= fechaActual)&& (monto.fecha >= fechaUltimos30Dias)){
                    contador++;
                }
            }
            if (contador > contadorAux){
                strcpy(userAux,user.username);
                contadorAux = contador;
                flag = 1;
            }
            else if (flag == 0)
            {
                strcpy(userAux,user.username);
                contadorAux = contador;
            }
            
        }
        fclose(archivo);
        fclose(archivoGestor);
        cout<<endl<< "Usuario con mas ingresos es "<< userAux<<" con "<<contadorAux<<" ingresos."<< endl<< endl;
    }

}

void mostrarMontoMaximo(){
    Transaccion monto;
    Usuario user;
    Transaccion montoMax;

    const char* nombreArchivoGestor = "archivo.dat";
    FILE * archivoGestor= fopen(nombreArchivoGestor, "rb");

    const char* nombreArchivo = "transacciones.dat";
    FILE * archivo = fopen(nombreArchivo, "rb");
    
    if (archivo == NULL) {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivo);
    }
    else if (archivoGestor == NULL)
    {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivoGestor);   
    }
    else{
        fseek(archivo, 0, SEEK_SET);
        while(fread(&user, sizeof(Usuario), 1, archivoGestor) == 1){
            montoMax.monto = 0;
            cout<<endl;
            cout<<"Cliente: "<<user.username<<endl;
            fseek(archivo, 0, SEEK_SET);
            while(fread(&monto, sizeof(Transaccion), 1, archivo) == 1){
                if((strcasecmp(monto.name, user.username) == 0) && (montoMax.monto< monto.monto)){
                    montoMax = monto;
                }
            }
            if(montoMax.monto == 0){
                cout<<"El Usuario no tiene transacciones."<< endl;
            }
            else{
                cout<<"Monto maximo: "<<montoMax.monto<<endl;
                cout<<"Fecha de la transaccion: "<<montoMax.fecha<<endl;
            }
        }
        fclose(archivo);
        fclose(archivoGestor);
    }
    
}

int ingresarAño(){
    int año;
    int flag = 0;

    do{
        if (flag == 1){
            cout<<"Año ingresado Invalido"<< endl;
        }
        cout<<"Ingrese el año: ";
        cin>>año;

        flag = 1;
    }while(!(año<=AÑO_ACTUAL) || !(año>=AÑO_INICIO));
    return año;
}

int seleccionarMes(){
    int mes;
    int flag = 0;

    do{
        if (flag == 1 ){
            cout<<"Mes incorrecto, debe ingresar un id del 1 al 12"<< endl;
        }
        cout<<"Ingrese el mes que quiere solicitar (Id)"<<endl;
        cout<<"1. Enero"<<endl;
        cout<<"2. Febrero"<<endl;
        cout<<"3. Marzo"<<endl;
        cout<<"4. Abril"<<endl;
        cout<<"5. Mayo"<<endl;
        cout<<"6. Junio"<<endl;
        cout<<"7. Julio"<<endl;
        cout<<"8. Agosto"<<endl;
        cout<<"9. Septiembre"<<endl;
        cout<<"10. Octubre"<<endl;
        cout<<"11. Noviembre"<<endl;
        cout<<"12. Diciembre"<<endl<<endl;
        cout<<"Id del mes: ";
        cin>>mes;

        flag = 1;
    }while( !(mes >= 1) || !(mes<=12) );

    return mes;
}

int cantidadTransaccionesCliente(char name[50]){
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
            if(strcasecmp(cantidad.name, name) == 0){
                contador++;
            }
        }
        fclose(archivo);
    }
    return contador;
}

void listarCantidadIngresoEgreso(char t[50], int año, int mes){
    Transaccion idValidation;
    int cantEgresos = 0, CantIngresos = 0;
    int aux_mes = 0;
    int aux_año = 0;

    const char* nombreArchivo = "transacciones.dat";

    FILE * archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL) {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivo);
    }
    else{
        fseek(archivo, 0, SEEK_SET);
        while(fread(&idValidation, sizeof(Transaccion), 1, archivo) == 1){
            if(strcasecmp(idValidation.name, t) == 0){
                aux_mes = (idValidation.fecha / 100) % 100;
                aux_año = idValidation.fecha / 10000;
                if ((aux_mes == mes) && (aux_año == año)){

                    cout<<endl <<" El id de la transacción es: "<< idValidation.id<< endl;
                    if(idValidation.ingreso){
                        cout<<" El monto ingresado fue de: "<< idValidation.monto<< endl;
                        CantIngresos++;
                    }
                    else{
                        cout<<" El monto extraido fue de: "<< idValidation.monto<< endl;
                        cantEgresos++;
                    }
                    cout<<" Fecha: "<< idValidation.fecha<< endl;
                }    
            }
        }
        fclose(archivo);
        cout<<endl<<"La cantidad de ingresos fue de: "<< CantIngresos<< endl;
        cout<<"La cantidad de egresos fue de:  "<< cantEgresos<< endl;
    }
    cout<<endl<<endl;
}

void seleccionarUsuario(char t[50]){
    int flag = 0;
    do{
        if (flag == 1){
            cout<<"El usuario no existe."<< endl;
        }
        cout << "Ingrese el Usuario: ";
        cin>>t;
        cout<<endl; 

        flag = 1; 
    }while(!validarCliente(t));
}

void listarTransacciones(char t[50]){ // Terminar función preguntar al profe dudas.
    Transaccion transacciones[100];
    Transaccion user;
    int contador = 0, CantIngresos = 0, flag = 0, len = 50;
    char sigo;

    const char* nombreArchivo = "transacciones.dat";
    FILE * archivo = fopen(nombreArchivo, "rb");
    
    if (archivo == NULL)
    {
        cout<<"Error al abrir el archivo"<< endl;
        fclose(archivo);   
    }
    else{
        fseek(archivo, 0, SEEK_SET);
        while(fread(&user, sizeof(Transaccion), 1, archivo) == 1){
            if(strcasecmp(user.name, t) == 0){
                CantIngresos++;
            }
        }
        int cantPag = (CantIngresos / 5) + 1;
        int contadorPag = 0;
        
        cout<<"Pagina "<< contadorPag + 1 << " - "<<cantPag <<endl;
        fseek(archivo, 0, SEEK_SET);
        while(fread(&user, sizeof(Transaccion), 1, archivo) == 1){
            if(strcasecmp(user.name, t) == 0){
                if ((contador % 5 == 0) && (contador != 0)) {
                    cout<<"Queres seguir? (s/n): ";
                    cin>>sigo;
                    do{
                        if (sigo=='n'){
                            flag = 2;
                        }
                        else if (sigo=='s'){
                            contadorPag++;
                            cout<<"Pagina "<< contadorPag  + 1 << " - "<<cantPag <<endl;
                            flag = 0;
                        }else{
                            flag == 1;
                        }                    
                    }while(flag == 1);

                }
                if (user.fecha != 0 && flag != 2){
                    cout<< "ID: "<< user.id<< endl;
                    cout<< "Fecha: "<< user.fecha<< endl;
                    cout<< "Monto: "<< user.monto<< endl;
                    cout<< endl;
                    contador++;
                }
                else{break;}
            }
        }
        fclose(archivo);
    }   
}


bool validarCliente(char username[50]) {
    bool ingresoValido = false;
    Usuario user;
    FILE* archivo = fopen("archivo.dat", "rb");
    if (archivo != NULL) {
        while (fread(&user, sizeof(Usuario), 1, archivo) == 1) {
            if (strcasecmp(user.username, username) == 0) {
                ingresoValido = true;
                break;
            }
        }
        fclose(archivo);
    }
    return ingresoValido;
}

int diaInicial(int fechaActual){
    int nuevaFecha;

    int diasRestar = 30; 
    int aux_day = 0;

    int año = fechaActual / 10000;
    int mes = (fechaActual / 100) % 100;
    int dia = fechaActual % 100;

    aux_day = dia - diasRestar;

    if(aux_day <= 0) {
        mes--;
        if (mes == 0) {  
            mes = 12;
            año--;
        }
    }

    nuevaFecha = año * 10000 + mes * 100 + dia;

    return nuevaFecha;
}

int menu(){
    int opcion = 0;
    cout<<"     1. Listar transacciones de un cliente por fecha(De a 5)"<< endl;
    cout<<"     2. Listar la cantidad de ingresos y egresos por mes de un cliente"<< endl;
    cout<<"     3. Listar la maxima transaccion de todos los clientes"<<endl;
    cout<<"     4. Mostrar el cliente que más ingresos tuvo en los últimos 30 días. "<<endl;
    cout<<"     5. Salir"<<endl;
    cout << "Seleccione una opción: ";
    cin>>opcion;

    return opcion;
}