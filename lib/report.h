#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/disco.h"
#include "../lib/mount.h"

using namespace std;

class Report {
    public:
    Report();

    void generar(vector<string> context, Mount m);
    void mbr(string p, string id);
    void dks(string p, string id);
    private:
    Shared shared;
    Disk disk;
    Mount mount;
};
#endif
