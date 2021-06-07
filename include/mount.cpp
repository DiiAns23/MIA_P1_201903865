#include "../lib/mount.h"

#include <iostream>
#include <stdlib.h>
#include "string"
#include <locale>

using namespace std;

Mount::Mount(){}

void Mount::mount(vector<string> context){
    if(context.empty()){
        listmount();
        return;
    }
    vector<string> required = {"name","path"}; //Campos obligatorios
    string path;
    string name;
    for(auto current:context){
        string id = shared.lower(current.substr(0,current.find("=")));
        current.erase(0, id.length()+1);
        if(current.substr(0,1)=="\""){
            current = current.substr(1,current.length()-2);
        }

        if(shared.compare(id,"name")){
            if(count(required.begin(), required.end(), id)){
                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                name = current;
            }
        }else if(shared.compare(id,"path")){
            if(count(required.begin(), required.end(), id)){
                auto itr = find(required.begin(), required.end(), id);
                required.erase(itr);
                path = current;
            }
        }
    }
    if(required.size()!=0){
        shared.handler("MOUNT", " requiere parámetros obligatorios");
        return;
    }
    mount(path,name);
}

void Mount::mount(string p, string n){
    try {
        FILE *validate = fopen(p.c_str(), "r"); //Verifica que exista el disco
        if (validate == NULL) {
            throw runtime_error("disco no existente");
        }

        Structs::MBR disk;          //obtiene los datos del disco
        rewind(validate);
        fread(&disk, sizeof(Structs::MBR), 1, validate);
        fclose(validate);

        Structs::Partition partition = dsk.findby(disk, n, p); //Obtiene las particiones del disco
        if (partition.part_type == 'E') {  //Montar una particion logica?
            vector<Structs::EBR> ebrs = dsk.getlogics(partition, p); //Obtiene las particiones logicas
            if (!ebrs.empty()) {
                Structs::EBR ebr = ebrs.at(0);
                n = ebr.part_name;
                shared.handler("", "se montará una partición lógica");
            } else {
                throw runtime_error("no se puede montar una extendida");
            }
        }

        for (int i = 0; i < 99; i++) {
            if (mounted[i].path == p) {
                for (int j = 0; j < 26; j++) {
                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = alfabeto.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        string re = to_string(i + 1) + alfabeto.at(j);
                        shared.response("MOUNT", "se ha realizado correctamente el mount -id=65" + re);
                        return;
                    }
                }
            }  
        }
        for (int i = 0; i < 99; i++) {
            if (mounted[i].status == '0') {
                mounted[i].status = '1';
                strcpy(mounted[i].path, p.c_str());
                for (int j = 0; j < 26; j++) {
                    if (Mount::mounted[i].mpartitions[j].status == '0') {
                        mounted[i].mpartitions[j].status = '1';
                        mounted[i].mpartitions[j].letter = alfabeto.at(j);
                        strcpy(mounted[i].mpartitions[j].name, n.c_str());
                        string re = to_string(i + 1) + alfabeto.at(j);
                        shared.response("MOUNT", "se ha realizado correctamente el mount -id=65" + re);
                        return;
                    }
                }
            }
        }
    }
    catch (exception &e) {
        shared.handler("MOUNT", e.what());
        return;
    }
}

void Mount::unmount(vector<string> context)
{
    vector<string> required = {"id"};
    string id_;

    for(int i = 0; i<context.size();i++)
    {
        string current = context.at(i);
        string id = current.substr(0,current.find("="));
        current.erase(0,id.length()+1);
        
        if(shared.compare(id,"id")){
            auto itr = find(required.begin(), required.end(), id);
            required.erase(itr);
            id_= current;
        }
    }
    if(required.size()!=0){
        shared.handler("UNMOUNT"," requiere parametros obligatorios -id");
        return;
    }
    unmount(id_);
}

void Mount::unmount(string id){
    shared.response("UNMOUNT", "Aqui se realizara el desmontar disco");
}
void Mount::listmount() {
    cout << "\n SOLO PARA CORROBORRAR LAS MONTADURAS DE PARTICIONES <3" << endl;
    cout << "\n<-------------------------- MOUNTS -------------------------->"
         << endl;
    for (int i = 0; i < 99; i++) {
        for (int j = 0; j < 26; j++) {
            if (mounted[i].mpartitions[j].status == '1') {
                cout << "> 65" << i + 1 << alfabeto.at(j) << ", " << mounted[i].mpartitions[j].name << endl;
            }
        }
    }
}