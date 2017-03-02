#include "problem.h"

#define PROBLEM_CLASS(o) reinterpret_cast<Problem<PROBLEM_PRECISION> *>(o)

#ifdef __cplusplus

template <class FP> void Problem<FP>::init() {
  x.resize(2, 0);
  p.resize(2, 0);

  x_s = 2;
  p_s = 2;

  f.push_back(fA_test);
  f.push_back(fB_test);
}

template <class FP> void Problem<FP>::close() {}

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
      (*i)(reinterpret_cast<void*>(this));
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

size_t CProblem_read(CProblem self, int index, char *buf, size_t size, size_t offset) {
  return PROBLEM_CLASS(self)->read(index, buf, size, offset);
}
