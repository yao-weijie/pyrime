#ifndef RIME_WRAPPER_H_
#define RIME_WRAPPER_H_

#include <pybind11/pybind11.h>
#include <rime_api.h>

namespace py = pybind11;

#define K_PgUp 0xff55
#define K_PgDn 0xff56

py::str fmt_commit(const RimeCommit& commit);
py::str fmt_traits(const RimeTraits& traits);
py::str fmt_candidate(const RimeCandidate& candidate);
py::str fmt_composition(const RimeComposition& composition);
py::str fmt_menu(const RimeMenu& menu);
py::str fmt_context(const RimeContext& context);
py::str fmt_status(const RimeStatus& status);

#endif  // RIME_WRAPPER_H_
