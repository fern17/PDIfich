#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

//Convierte un unsigned int a String
std::string uintToStr(unsigned int value) {
    std::stringstream ss;
    ss<<value;
    std::string ret_val;
    ss>>ret_val;
    return ret_val;
}

//El folder es solo el ejemplo, por ejemplo cave, woods
std::vector<std::string> getSeeds(const char * folder, const char * _tipo) {
    //Vector de nombre de archivos
    std::vector<std::string> ret_val;
    //Base para los archivos
    std::string base(folder);
    //tipo de los archivos
    std::string tipo(_tipo);
    //Componemos la base
    base = "seeds/" + base + '/' + base;

    //Para cada archivo
    for (unsigned int i = 1; i < 5; i++) {
        //Nombre del archivo actual
        std::string current = base + uintToStr(i) + "." + tipo;
        //Intentamos leerlo
        std::ifstream file (current.c_str());
        if (not file.is_open()) {
            //Si no lo encontramos, le ponemos un Not Found
            ret_val.push_back("NOT_FOUND");
        } else {
            //Si lo encontramos, agregamos el nombre
            ret_val.push_back(current);
            file.close();
        }
    }
    return ret_val;
}

