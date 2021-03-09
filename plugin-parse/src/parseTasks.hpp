#ifndef PARSETASK_HPP
#define PARSETASK_HPP

#include <string.h>
#include <vector>

int parseTask(std::string &filename, std::vector<double> &t);
double getTime(int jobId, std::vector<double> &t);

#endif