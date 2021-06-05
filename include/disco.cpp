#include "../lib/disco.h"
#include "../lib/Estructuras.h"
#include "../lib/scanner.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace std;

scanner scan;
Disk::Disk(){

}

void Disk::mkdisk(vector<string> tokens){ 
    string size = "";
    string f = "";
    string u = "";
    string path = "";
    bool error = false;
    for (string token:tokens)
    {
        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        if (scan.compare(tk, "f"))
        {
            if (f.empty())
            {
                f = token;
            }else{
                scan.errores("MKDISK","parametro F repetido en el comando"+tk);
            }
               
        }else if(scan.compare(tk, "size")){
            if (size.empty())
            {
                size = token;
            }else{
                scan.errores("MKDISK","parametro SIZE repetido en el comando"+tk);
            }
            
        }else if (scan.compare(tk, "u"))
        {
            if (u.empty())
            {
                u = token;
            }else{
                scan.errores("MKDISK","parametro U repetido en el comando"+tk);
            }
            
        }else if (scan.compare(tk, "path"))
        {
            if (path.empty())
            {
                path = token;
            }else{
                scan.errores("MKDISK","parametro PATH repetido en el comando"+tk);
            }
            
        }else{
            scan.errores("MKDISK","no se esperaba el parametro "+tk);
            error = true;
            break;
        }
    }
    if (f.empty())
    {
        f = "FF";
    }
    if (u.empty())
    {
        u = "M";
    }
    if (error)
    {
        return;
    }
    if (path.empty() && size.empty())
    {
        scan.errores("MKDISK", "se requiere parametro Path y size para este comando");
    }else if(path.empty()){
        scan.errores("MKDISK","se requiere parametro path para este comando");
    }else if (size.empty())
    {
        scan.errores("MKDISK","se requiere parametro size para este comando");
    }else if (!scan.compare(f,"BF") && !scan.compare(f,"FF") && !scan.compare(f,"WF"))
    {
        scan.errores("MKDISK","valores de parametro F no esperados");
    }
    else if (!scan.compare(u,"k") && !scan.compare(u,"m"))
    {
        scan.errores("MKDISK","valores de parametro U no esperados");
    }
    else{
        makeDisk(size,f,u,path);
    }
    
    
}

void Disk::makeDisk(string s, string f, string u, string path){
    Estructura::MBR disco;
    try
    {
        int size = stoi(s);
        if( size <= 0){
            scan.errores("MKDISK","size debe ser mayor a 0");
        }
        if (scan.compare(u,"M"))
        {
            size = 1024*1024*size;
        }else if (scan.compare(u, "K"))
        {
            size = 1024*size;
        }
        f = f.substr(0,1);
        time_t t;
        struct tm *tm;
        char fechayhora[20];
        t = time(nullptr);
        tm = localtime(&t);
        strftime(fechayhora, 20, "%Y/%m/%d %H:%M:%S", tm);
        disco.mbr_tamano = size;
        strcpy(disco.mbr_fecha_creacion, fechayhora);
        disco.disk_fit = toupper(f[0]);
        disco.mbr_disk_signature = rand() % 9999 + 100;

        FILE *validar = fopen(path.c_str(), "r");
        if (validar != NULL)
        {
            scan.errores("MKDISK", "Disco ya existente en la ruta indicada");
            fclose(validar);
            return;
        }
        string path2 = path;
        if (path.substr(0, 1) == "\"")
        {
            path = path.substr(1, path.length() - 2);
        }
        if(!scan.compare(path.substr(path.find_last_of(".") + 1),"dk")){
            scan.errores("MKDISK", "Extensión de archivo no valida");
            return;
        }
        
        try
        {
            FILE *file = fopen(path.c_str(), "w+b");
            if (file!=NULL)
            {
                fwrite("\0", 1, 1, file);
                fseek(file, size - 1, SEEK_SET);
                fwrite("\0", 1, 1, file);
                rewind(file);
                fwrite(&disco, sizeof(Estructura::MBR), 1, file);
                fclose(file);
            }else{
                string comando1 = "mkdir -p \"" + path + "\"";
                string comando2 = "rmdir \"" + path + "\"";
                system(comando1.c_str());
                system(comando2.c_str());
                FILE *file = fopen(path.c_str(), "w+b");
                fwrite("\0",1,1,file);
                fseek(file, size - 1, SEEK_SET);
                fwrite("\0", 1, 1, file);
                rewind(file);
                fwrite(&disco, sizeof(Estructura::MBR),1, file);
                fclose(file);
            }
                    FILE *imprimir = fopen(path.c_str(), "r");
                    if(imprimir!=NULL){
                        Estructura::MBR discoI;
                        fseek(imprimir, 0, SEEK_SET);
                        fread(&discoI,sizeof(Estructura::MBR), 1,imprimir);
                        scan.respuesta("MKDISK","Disco creado exitosamente");
                        std::cout << "*****Nuevo Disco*****" << std::endl;
                        std::cout << "Size: "<< discoI.mbr_tamano << std::endl;
                        std::cout << "date: "<< discoI.mbr_fecha_creacion << std::endl;
                        std::cout << "FIT: "<< discoI.disk_fit << std::endl;
                        std::cout << "DISK_SIGNATURE: "<< discoI.mbr_disk_signature << std::endl;
                        cout << "Bits del MBR: " << sizeof(Estructura::MBR) << endl;
                        std::cout << "PATH"<< path2 << std::endl;
                    }
                    fclose(imprimir);

            
        }
        catch(const std::exception& e)
        {
            scan.errores("MKDISK: ","error al crear el disco");
        }
        
    }
    catch(invalid_argument &e)
    {
        scan.errores("MKDISK: ","size debe ser un entero");

    }
    
    
}

void Disk::rmdisk(vector<string> context){
    string path = "";
    bool error = false;
    if (context.size()==0)
    {
        scan.errores("RMDISK","Se esperaba el path para completar la acción");
    }
    
    for (string token:context)
    {
        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        if (scan.compare(tk, "path"))
        {
            path= token;
        }else{
            path = "";
            scan.errores("RMDISK","No se reconoce este elemento "+tk);
            break;
        }
    }
    if (!path.empty())
    {
        if (path.substr(0, 1) == "\"")
        {
            path = path.substr(1, path.length() - 2);
        }
        try
        {
            FILE *file = fopen(path.c_str(), "r");
            
            if (file != NULL)
            {
                if(!scan.compare(path.substr(path.find_last_of(".") + 1),"dk")){
                    scan.errores("RMDISK", "Extensión de archivo no valida");
                    return;
                }
                fclose(file);
                if (scan.confirmar("¿Desea eliminar el archivo?"))
                {
                    if (remove(path.c_str()) == 0)
                    {
                        scan.respuesta("RMDISK","Disco eliminado correctamente");
                        return;
                    }
                }else{
                    scan.respuesta("RMDISK","Operación cancelada");
                    return;
                }
            }
            scan.errores("RMDISK", "El disco que desea eliminar no existe en la ruta indicada");
        }
        catch(const std::exception& e)
        {
            scan.errores("RMDISK","Error al intentar eliminar el disco");
        }
        
    }
    
}
