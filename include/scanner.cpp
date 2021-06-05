#include "../lib/scanner.h"
#include "../lib/disco.h"
#include <iostream>
#include <stdlib.h>
#include <locale>
#include <cstring>
#include <locale>
#include <fstream>
#include <vector>
#include<cstdlib>
using namespace std;

Disk disco;
scanner::scanner()
{
}
void Clear()
{
    cout << "\x1B[2J\x1B[H";
}

void scanner::start()
{
    system("clear");
    std::cout << "------------------------------INGRESE UN COMANDO------------------------------\n" << std::endl;
    std::cout << "--------------------------------exit para salir-------------------------------\n" << std::endl;
    std::cout << ">>";
    while (true)
    {
        string texto;
        getline(cin, texto);
        Clear();
        if (compare(texto, "exit"))
        {
            break;
        }
        string tk = token(texto);
        texto.erase(0,tk.length()+1);
        vector<string> tks = split_tokens(texto);
        functions(tk, tks);
        std::cout << "\nPresione Enter para continuar...." << std::endl;
        getline(cin,texto);
        Clear();
        std::cout << "------------------------------INGRESE UN COMANDO------------------------------\n" << std::endl;
        std::cout << "--------------------------------exit para salir-------------------------------\n" << std::endl;
        std::cout << ">>";

    }
    
}

void scanner::functions(string token, vector<string> tks)
{
    if (compare(token, "MKDISK"))
    {
        std::cout << "**********MKDISK**********" << std::endl;
        disco.mkdisk(tks);
    }else if(compare(token, "RMDISK")){
        std::cout << "*********RMDISK***********" << std::endl;
        disco.rmdisk(tks);
    }else if(compare(token.substr(0,1),"#")){
        respuesta("COMENTARIO",token);
    }else{
        errores("SYSTEM","El comando ingresado no se reconoce en el sistema \""+token+"\"");
    }
    
}

string scanner::token(string text)
{
    string tkn = "";
    bool terminar = false;
    for (char &c : text)
    {
        if (terminar)
        {
            if (c == ' ' || c == '-')
            {
                break;
            }

            tkn += c;
        }
        else if ((c != ' ' && !terminar))
        {
            if (c=='#')
            {
                tkn=text;
                break;
            }else{
                tkn += c;
                terminar = true;
            }
            
        }
    }
    return tkn;
}

vector<string> scanner::split(string text, string text_split)
{
    vector<string> cadena;
    if (text.empty())
    {
        return cadena;
    }
    
    int n = text.length();
    char char_array[n + 1];
    strcpy(char_array, text.c_str());
    char* point = strtok(char_array, text_split.c_str());
    while (point!=NULL)
    {
        cadena.push_back(string(point));
        point = strtok(NULL, text_split.c_str());
    }
    return cadena;
}

vector<string> scanner::split_tokens(string text){
    vector<string> tokens;
    if (text.empty())
    {
        return tokens;
    }
    text.push_back(' ');
    string token = "";
    int estado = 0;
    for(char &c: text){
        if (estado ==0 && c=='-')
        {
            estado = 1;

        }else if(estado==0 && c=='#'){
            continue;
        }else if(estado!=0){
            if (estado == 1)
            {
                if(c=='='){
                    estado = 2;
                }else if(c == ' '){
                    continue;
                }
            }else if(estado == 2){
                if (c=='\"')
                {
                    estado = 3;
                }else{
                    estado = 4;
                }
                
            }else if(estado == 3){
                if (c=='\"')
                {
                    estado = 4;
                }
            }else if (estado==4 && c=='\"')
            {
                tokens.clear();
                continue;
            }else if (estado ==4 && c==' ')
            {
                estado = 0;
                tokens.push_back(token);
                token = "";
                continue;
            }
            token+=c;
        }
    }
    return tokens;
}

string scanner::upper(string a){
    string up="";
    for(char &a: a){
        up+=toupper(a);
    }
    return up;  
}

bool scanner::compare(string a, string b){
    if (upper(a)==upper(b))
    {
        return true;
    }
    return false;
}

void scanner::errores(string operacion, string mensaje){
    
    cout << "\033[1;41m Error\033"<< "\033[0;31m(" + operacion + ")--> \033[0m"<< mensaje << endl;
}

void scanner::respuesta(string operacion, string mensaje){
    
    cout << "\033[0;42m(" + operacion + ")--> \033[0m"<< mensaje << endl;
}

bool scanner::confirmar(string mensaje){
    std::cout << mensaje << "Confirmar(S), cualquier otra letra para cancelar" << std::endl;
    string respuesta;
    getline(cin,respuesta);
    if (compare(respuesta, "s"))
    {
        return true;
    }
    return false;
    
}

void scanner::funcion_excec(vector<string> tokens){
    string path = "";
    bool error = false;
    for (string token:tokens)
    {
        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        if (compare(tk, "path"))
        {
            path = token;
        }
    }
    if (path.empty())
    {
        errores("EXEC","Se requiere path para este comando");
        return;
    }
    excec(path);
}

void scanner::excec(string path){
        fstream archivo;
    string linea;
    if (!archivo.is_open())
    {
        archivo.open(path,ios::in);
        if (archivo.fail())
        {
            errores("EXEC","Error al abrir el archivo");
        }
        while (!archivo.eof())
        {
            getline(archivo, linea);
            respuesta("EXEC",linea);
            if (compare(linea, "E"))
            {
                break;
            }
            string tk = token(linea);
            linea.erase(0,tk.length()+1);
            vector<string> tks = split_tokens(linea);
            functions(tk, tks);
            std::cout << "**************************" << std::endl;
        }   
    } 
}
