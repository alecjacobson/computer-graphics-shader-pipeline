#ifndef LAST_MODIFICATION_TIME_H
#define LAST_MODIFICATION_TIME_H
#include <string>
double last_modification_time(const std::string & path);

// Implementation

#include <sys/stat.h>
#include <sys/time.h>
#include <ctime>
#include <fcntl.h>

double last_modification_time(const std::string & path)
{
  struct stat s;
  struct timespec t = {0,0};
  if (stat(path.c_str(), &s) < 0) { return -1; }
#ifdef __APPLE__
  t = s.st_mtimespec;
#else // Linux?
  t = s.st_mtim;
#endif
  return double(t.tv_sec) + double(t.tv_nsec)*1e-9;
}

#endif
