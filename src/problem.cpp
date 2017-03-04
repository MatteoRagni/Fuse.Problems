/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* Copyright (c) 2017 - Matteo Ragni, University of Trento                   */
/*                                                                           */
/* Permission is hereby granted, free of charge, to any person               */
/* obtaining a copy of this software and associated documentation            */
/* files (the "Software"), to deal in the Software without                   */
/* restriction, including without limitation the rights to use,              */
/* copy, modify, merge, publish, distribute, sublicense, and/or sell         */
/* copies of the Software, and to permit persons to whom the                 */
/* Software is furnished to do so, subject to the following                  */
/* conditions:                                                               */
/*                                                                           */
/* The above copyright notice and this permission notice shall be            */
/* included in all copies or substantial portions of the Software.           */
/*                                                                           */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES           */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                  */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT               */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,              */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING              */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR             */
/* OTHER DEALINGS IN THE SOFTWARE.                                           */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "problem.h"
#include "problemShared.hpp"
#include "fuseproblem.h"

/* Exception Log function */
#define ERROR_LOG(m) std::cout << "EXCEPTION RAISED: " << m << endl;

template <class FP> void Problem<FP>::init() {
  size_t (*fncSize_f)();
  size_t (*fncSize_p)();
  size_t (*fncSize_x)();
  Function *fncPtr;

  if (!(access(dl_file.c_str(), F_OK) != -1))
    throw("Cannot find library");

  if (!(hdl = dlopen(dl_file.c_str(), RTLD_LAZY)))
    throw(dlerror());

  fncSize_f = (size_t(*)())dlsym(hdl, "FunctionSizeF");
  fncSize_x = (size_t(*)())dlsym(hdl, "FunctionSizeX");
  fncSize_p = (size_t(*)())dlsym(hdl, "FunctionSizeP");
  fncPtr = (Function *)dlsym(hdl, "FunctionArray");

  if (!fncSize_f || !fncSize_x || !fncSize_p)
    throw("Cannot find FunctionSize symbol");

  if (!fncPtr)
    throw("Cannot find FunctionArray symbol");

  f_s = fncSize_f();
  x_s = fncSize_x();
  p_s = fncSize_p();

  x.resize(x_s, 99e4);
  for (auto i = x.begin(); i != x.end(); ++i)
    x_file += to_string(*i) + "\n";
  p.resize(p_s, 55e5);
  for (auto i = p.begin(); i != p.end(); ++i)
    p_file += to_string(*i) + "\n";

  for (size_t i = 0; i < f_s; i++) {
    f.push_back(fncPtr[i]);
    paths.insert(pair<string, int>("/y" + to_string(i), i));
  }

  paths.insert(pair<string, int>("/x", static_cast<int>(FunctionOutput::xo)));
  paths.insert(pair<string, int>("/p", static_cast<int>(FunctionOutput::po)));
  paths.insert(
      pair<string, int>("/info", static_cast<int>(FunctionOutput::info)));
  paths.insert(pair<string, int>("/", static_cast<int>(FunctionOutput::root)));

  // Info file generation
  info_file = "---\nlib: " + dl_file + "\nxsize: " + to_string(x_s) +
              "\npsize: " + to_string(p_s) + "\nfsize: " +
              to_string(f_s);
  update();
}

template <class FP> void Problem<FP>::close() {
  if (hdl)
    dlclose(hdl);
}

/*                _      _
 *  _  _ _ __  __| |__ _| |_ ___
 * | || | '_ \/ _` / _` |  _/ -_)
 *  \_,_| .__/\__,_\__,_|\__\___|
 *      |_|
  * * * * * * * * * * * * * * * * * * */
template <class FP> void Problem<FP>::update() {
  y_file.clear();

  for (auto i = f.begin(); i != f.end(); ++i) {
    string s("");
    try {
      (*i)(reinterpret_cast<void *>(&y), reinterpret_cast<void *>(&x),
           reinterpret_cast<void *>(&p));
    } catch (...) {
      ERROR_LOG("Function throwed expression");
    }

    for (auto j = y.begin(); j != y.end(); ++j) {
      s += to_string(*j) + "\n";
    }
    y_file.push_back(s);
  }
}

/*             _ _
 * __ __ ___ _(_) |_ ___
 * \ V  V / '_| |  _/ -_)
 *  \_/\_/|_| |_|\__\___|
 * * * * * * * * * * * * * */
template <class FP>
size_t Problem<FP>::write(FunctionInput type, const char *buf) {
  string s(buf);
  vector<FP> n;
  regex rgx("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");

  for (sregex_iterator i = sregex_iterator(s.begin(), s.end(), rgx);
       i != sregex_iterator(); ++i) {
    n.push_back(stod((*i).str()));
  }

  switch (type) {
  case FunctionInput::xi:
    if (n.size() != x_s) {
      return 0;
    }
    x_file = s;
    x = n;
    break;
  case FunctionInput::pi:
    if (n.size() != p_s) {
      return 0;
    }
    p_file = s;
    p = n;
    break;
  }
  update();
  return n.size();
}

/*                  _
 *  _ _ ___ __ _ __| |
 * | '_/ -_) _` / _` |
 * |_| \___\__,_\__,_|
 * * * * * * * * * * * * * */
template <class FP>
int Problem<FP>::read(int type, char *buf, size_t size, size_t offset) {
  // size_t ret = 0;
  string *out;

  switch(static_cast<FunctionOutput>(type)) {
    case FunctionOutput::info:
      out = &info_file;
      break;
    case FunctionOutput::xo:
      out = &x_file;
      break;
    case FunctionOutput::po:
      out = &p_file;
      break;
    default:
      size_t idx = static_cast<size_t>(type);
      if (idx >= 0 && idx < y_file.size())
        out = y_file.data() + idx;
      else
        return 0;
      break;
  }

  size_t out_size = out->size();
  if (offset > out_size)
    return 0;

  if ((offset + size) > out_size) {
    strcpy(buf, (out->substr(offset, out_size - offset)).c_str());
    return static_cast<int>(out_size - offset);
  }

  strcpy(buf, (out->substr(offset, size)).c_str());
  return static_cast<int>(size);
}

template <class FP> int Problem<FP>::pathid(const char *path) {
  string p(path);

  if (paths.find(p) == paths.end())
    return PROBLEM_E_NOFILE;

  return paths[p];
}


template <class FP> size_t Problem<FP>::size(FunctionOutput t) {
  switch(t) {
    case FunctionOutput::xo:
      return x_s;
      break;
    case FunctionOutput::po:
      return p_s;
      break;
    case FunctionOutput::fo:
      return f_s;
      break;
    case FunctionOutput::info:
    case FunctionOutput::root:
    break;
  }
  return 0;
}

template <class FP> size_t Problem<FP>::filesize(FunctionOutput t, int i) {
  switch(t) {
    case FunctionOutput::xo:
      return x_file.size();
      break;
    case FunctionOutput::po:
      return p_file.size();
      break;
    case FunctionOutput::info:
      return info_file.size();
      break;
    case FunctionOutput::fo:
      if ((static_cast<size_t>(i) >= 0) && (static_cast<size_t>(i) < f_s)) {
        return y_file.size();
      }
      break;
    case FunctionOutput::root:
    break;
  }
  return 0;
}

// -------------------------------------------------------------------------------

/**
 * This definition queries the fuse context to get the private data.
 * Once is obtained reinterpret it to a Problem pointer of the initially
 * created instance
 */
//#define get_problem() reinterpret_cast< Problem<PROBLEM_PRECISION>* >(fuse_get_context()->private_data)
#define get_problem() reinterpret_cast< Problem<PROBLEM_PRECISION>* >(problem)

// void fpr_setpath(const char * path) { dl_path = (char *)path; }

//  _      _ _
// (_)_ _ (_) |_
// | | ' \| |  _|
// |_|_||_|_|\__|
void fpr_init(const char * path) {
  Problem<PROBLEM_PRECISION> *pr = new Problem<PROBLEM_PRECISION>(path);
  problem = reinterpret_cast<void*>(pr);
}

//     _        _
//  __| |___ __| |_ _ _ ___ _  _
// / _` / -_|_-<  _| '_/ _ \ || |
// \__,_\___/__/\__|_| \___/\_, |
//                          |__/
void fpr_destroy() {
  delete reinterpret_cast< Problem<PROBLEM_PRECISION>* >(problem);
}


//           _        _   _
//  __ _ ___| |_ __ _| |_| |_ _ _
// / _` / -_)  _/ _` |  _|  _| '_|
// \__, \___|\__\__,_|\__|\__|_|
// |___/
int fpr_getattr(const char *path, struct stat *stbuf) {
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);

  if (path_id < FunctionOutput::root)
    return -ENOENT;

  memset(stbuf, 0, sizeof(struct stat));

  // User space configuration
  stbuf->st_uid = getuid();
  stbuf->st_gid = getgid();
  stbuf->st_atime = stbuf->st_mtime = time(NULL);

  switch(path_id) {
  case FunctionOutput::root:
    stbuf->st_mode = S_IFDIR | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 2;
    break;
  case FunctionOutput::xo:
    stbuf->st_mode = S_IFREG | S_IWUSR | S_IWGRP | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = pr->filesize(FunctionOutput::xo);
    break;
  case FunctionOutput::po:
    stbuf->st_mode = S_IFREG | S_IWUSR | S_IWGRP | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = pr->filesize(FunctionOutput::po);
    break;
  case FunctionOutput::info:
    stbuf->st_mode = S_IFREG | S_IRUSR | S_IRGRP;
    stbuf->st_nlink = 1;
    stbuf->st_size = pr->filesize(FunctionOutput::info);
    break;
  default:
    if (path_id >= 0 && static_cast<size_t>(path_id) < pr->size(FunctionOutput::fo)) {
      stbuf->st_mode = S_IFREG | S_IRUSR | S_IRGRP;
      stbuf->st_nlink = 1;
      stbuf->st_size = pr->filesize(FunctionOutput::fo, path_id);
    }
    break;
  }
  return 0;
}

//                  _    _ _
//  _ _ ___ __ _ __| |__| (_)_ _
// | '_/ -_) _` / _` / _` | | '_|
// |_| \___\__,_\__,_\__,_|_|_|
int fpr_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);
  int f_size = static_cast<int>(pr->size(FunctionOutput::fo));

  if (path_id < FunctionOutput::root)
    return 0;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  switch (path_id) {
  case FunctionOutput::root:
    filler(buf, "x", NULL, 0);
    filler(buf, "p", NULL, 0);
    filler(buf, "info", NULL, 0);
    for (int i = 0; i < f_size; i++) {
      string s("y");
      filler(buf, (s + to_string(i)).c_str(), NULL, 0);
    }
    break;
  case FunctionOutput::info:
    filler(buf, "info", NULL, 0);
    break;
  case FunctionOutput::xo:
    filler(buf, "x", NULL, 0);
    break;
  case FunctionOutput::po:
    filler(buf, "p", NULL, 0);
    break;
  default:
    if (path_id >= 0 && path_id < f_size) {
      string s("y");
      filler(buf, (s + to_string(path_id)).c_str(), NULL, 0);
    }
    break;
  }
  return 0;
}

//  ___ _ __  ___ _ _
// / _ \ '_ \/ -_) ' \
// \___/ .__/\___|_||_|
//     |_|
int fpr_open(const char *path, struct fuse_file_info *fi) {
  (void) fi;
  fi->direct_io = 1;
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);

  if (path_id < FunctionOutput::root)
    return -ENOENT;
  return 0;
}

//                  _
//  _ _ ___ __ _ __| |
// | '_/ -_) _` / _` |
// |_| \___\__,_\__,_|
int fpr_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
  (void) fi;
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);

  if (path_id < FunctionOutput::root)
    return 0;

  return pr->read(path_id, buf, size, offset);
}

//             _ _
// __ __ ___ _(_) |_ ___
// \ V  V / '_| |  _/ -_)
//  \_/\_/|_| |_|\__\___|
int fpr_write(const char *path, const char *buf, size_t size,
                      off_t offset, struct fuse_file_info *fi) {
  (void) fi;
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);

  if(pr->write(static_cast<FunctionInput>(path_id), buf))
    return size;

  return 0;
}

//  _                         _
// | |_ _ _ _  _ _ _  __ __ _| |_ ___
// |  _| '_| || | ' \/ _/ _` |  _/ -_)
//  \__|_|  \_,_|_||_\__\__,_|\__\___|
int fpr_truncate(const char *path, off_t size) {
  Problem<PROBLEM_PRECISION> *pr = get_problem();
  int path_id = pr->pathid(path);
  if (path_id < FunctionOutput::root)
    return -ENOENT;

  return 0;
}
