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

#define PROBLEM_CLASS(o) reinterpret_cast<Problem<PROBLEM_PRECISION> *>(o)

#ifdef __cplusplus

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

  x.resize(x_s, 99e99);
  p.resize(p_s, 50e50);

  for (size_t i = 0; i < f_s; i++) {
    f.push_back(fncPtr[i]);
    paths.insert(pair<string, int>("/y" + to_string(i), i));
  }

  paths.insert(pair<string, int>("/x", static_cast<int>(FunctionOutput::xo)));
  paths.insert(pair<string, int>("/p", static_cast<int>(FunctionOutput::po)));
  paths.insert(
      pair<string, int>("/info", static_cast<int>(FunctionOutput::po)));
  paths.insert(pair<string, int>("/", static_cast<int>(FunctionOutput::root)));

  // Info file generation
  info_file = "---\nlib: " + dl_file + "\nxsize: " + to_string(x_s) + "\n" +
              "\npsize: " + to_string(p_s) + "\n" + "\nfsize: " +
              to_string(f_s) + "\n";
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

  for (auto i = f.begin(); i != f.end(); i++) {
    string s("");
    try {
      (*i)(reinterpret_cast<void *>(&y), reinterpret_cast<void *>(&x),
           reinterpret_cast<void *>(&p));
    } catch (...) {
      ERROR_LOG("Function throwed expression");
    }

    for (auto j = y.begin(); j != y.end(); j++) {
      s += (to_string((*j)) + "\n");
    }
    // cout << s << endl;
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
size_t Problem<FP>::read(int type, char *buf, size_t size, size_t offset) {
  size_t ret = 0;
  string *out;

  // Selecting string to export
  if (static_cast<FunctionOutput>(type) == FunctionOutput::info) {
    out = &info_file;
  } else if (static_cast<FunctionOutput>(type) == FunctionOutput::xo) {
    out = &x_file;
  } else if (static_cast<FunctionOutput>(type) == FunctionOutput::po) {
    out = &p_file;
  } else {
    size_t idx = static_cast<size_t>(type);
    if (idx >= 0 && idx < y_file.size())
      out = &(y_file[idx]);
    else
      return 0;
  }

  size_t out_size = out->size() + 1;
  if (offset > out_size)
    return 0;

  if ((offset + size) > out_size) {
    strcpy(buf, (out->substr(offset, out_size - offset)).c_str());
    return (out_size - offset);
  }

  strcpy(buf, (out->substr(offset, size)).c_str());
  return size;
}

template <class FP> int Problem<FP>::pathid(const char *path) {
  string p(path);

  if (paths.find(p) == paths.end())
    return PROBLEM_E_NOFILE;

  return paths[p];
}

#endif

/*   ___   ___     _            __
 *  / __|_|_ _|_ _| |_ ___ _ _ / _|__ _ __ ___
 * | (_|___| || ' \  _/ -_) '_|  _/ _` / _/ -_)
 *  \___| |___|_||_\__\___|_| |_| \__,_\__\___|
 * * * * * * * * * * * * * * * *  * * * * * * * */

CProblem CProblem_new(const char *dl) {
  string s(dl);
  return reinterpret_cast<void *>(new Problem<PROBLEM_PRECISION>(s));
}

void CProblem_destroy(CProblem self) { delete PROBLEM_CLASS(self); }

size_t CProblem_x_write(CProblem self, const char *buf) {
  return PROBLEM_CLASS(self)->write(FunctionInput::xi, buf);
}

size_t CProblem_p_write(CProblem self, const char *buf) {
  return PROBLEM_CLASS(self)->write(FunctionInput::pi, buf);
}

size_t CProblem_read(CProblem self, int index, char *buf, size_t size,
                     size_t offset) {
  return PROBLEM_CLASS(self)->read(index, buf, size, offset);
}

int CProblem_pathid(CProblem self, const char *path) {
  return PROBLEM_CLASS(self)->pathid(path);
}

size_t CProblem_xsize(CProblem self) { return PROBLEM_CLASS(self)->xsize(); }
size_t CProblem_psize(CProblem self) { return PROBLEM_CLASS(self)->psize(); }
size_t CProblem_fsize(CProblem self) { return PROBLEM_CLASS(self)->fsize(); }

size_t CProblem_xfilesize(CProblem self) {
  return PROBLEM_CLASS(self)->xfilesize();
}
size_t CProblem_pfilesize(CProblem self) {
  return PROBLEM_CLASS(self)->pfilesize();
}
size_t CProblem_ffilesize(CProblem self, int i) {
  return PROBLEM_CLASS(self)->ffilesize(i);
}