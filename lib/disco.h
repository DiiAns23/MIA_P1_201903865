#ifndef DISK_H
#define DISK_H
#include <vector>

#include <string>
#include <algorithm>
#include "../lib/disco.h"
#include "../lib/Estructuras.h"
#include "../lib/scanner.h"
#include "../lib/Estructuras.h"

using namespace std;

class Disk
{
public:
    Disk();
    void mkdisk(vector<string> tokens);
    void makeDisk(string s, string f, string u, string p);
    void rmdisk(vector<string> context);
};

#endif // END OF DECLARATION