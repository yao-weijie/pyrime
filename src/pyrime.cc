#include <pybind11/pybind11.h>
#include <rime_api.h>

#include <cstring>

namespace py = pybind11;

#define STRUCT_INIT(Type) def(py::init([]() { RIME_STRUCT(Type, var) return var; }))

#define DEF_STR_PROPERTY(Type, prop)                       \
    def_property(                                          \
        #prop, [](const Type& self) { return self.prop; }, \
        [](Type& self, const char* s) { self.prop = strdup(s); })

#define DEF_BOOL_PROPERTY_RO(Type, prop) \
    def_property_readonly(#prop, [](const Type& self) { return py::bool_(self.prop); })

PYBIND11_MODULE(pyrime, m) {
    m.doc() = "python bind of librime apis";

    // TODO: modules
    py::class_<RimeTraits>(m, "RimeTraits", py::dynamic_attr())
        .STRUCT_INIT(RimeTraits)
        .DEF_STR_PROPERTY(RimeTraits, shared_data_dir)
        .DEF_STR_PROPERTY(RimeTraits, user_data_dir)
        .DEF_STR_PROPERTY(RimeTraits, distribution_name)
        .DEF_STR_PROPERTY(RimeTraits, distribution_code_name)
        .DEF_STR_PROPERTY(RimeTraits, distribution_version)
        .DEF_STR_PROPERTY(RimeTraits, app_name)
        .def_readwrite("min_log_level", &RimeTraits::min_log_level)
        .DEF_STR_PROPERTY(RimeTraits, log_dir)
        .DEF_STR_PROPERTY(RimeTraits, prebuilt_data_dir)
        .DEF_STR_PROPERTY(RimeTraits, staging_dir);

    py::class_<RimeComposition>(m, "RimeComposition", py::dynamic_attr())
        .def(py::init<>())
        .def_readonly("length", &RimeComposition::length)
        .def_readonly("cursor_pos", &RimeComposition::cursor_pos)
        .def_readonly("sel_start", &RimeComposition::sel_start)
        .def_readonly("sel_end", &RimeComposition::sel_end)
        .def_readonly("cursor_pos", &RimeComposition::cursor_pos)
        .def_readonly("preedit", &RimeComposition::preedit);

    py::class_<RimeCandidate>(m, "RimeCandidate", py::dynamic_attr())
        .def(py::init<>())
        .def_readonly("text", &RimeCandidate::text)
        .def_readonly("comment", &RimeCandidate::comment);

    py::class_<RimeMenu>(m, "RimeMenu", py::dynamic_attr())
        .def(py::init<>())
        .def_readonly("page_size", &RimeMenu::page_size)
        .def_readonly("page_size", &RimeMenu::page_no)
        .DEF_BOOL_PROPERTY_RO(RimeMenu, is_last_page)
        .def_readonly("highlighted_candidate_index", &RimeMenu::highlighted_candidate_index)
        .def_readonly("num_candidates", &RimeMenu::num_candidates)
        // TODO: candidates
        .def_property_readonly("candidates",
                               [](const RimeMenu& self) {
                                   py::list candidates;
                                   for (int i = 0; i < self.num_candidates; i++) {
                                       candidates.append(self.candidates[i]);
                                   }
                                   return candidates;
                               })
        .def_readonly("select_keys", &RimeMenu::select_keys);

    py::class_<RimeCommit>(m, "RimeCommit", py::dynamic_attr())
        .STRUCT_INIT(RimeCommit)
        .def_readonly("text", &RimeCommit::text);

    py::class_<RimeContext>(m, "RimeContext", py::dynamic_attr())
        .STRUCT_INIT(RimeContext)
        .def_readonly("composition", &RimeContext::composition)
        .def_readonly("menu", &RimeContext::menu)
        .def_readonly("commit_text_preview", &RimeContext::commit_text_preview)
        // TODO: select_labels
        ;

    py::class_<RimeStatus>(m, "RimeStatus", py::dynamic_attr())
        .STRUCT_INIT(RimeStatus)
        .def_readonly("schema_id", &RimeStatus::schema_id)
        .def_readonly("schema_name", &RimeStatus::schema_name)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_disabled)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_composing)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_ascii_mode)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_full_shape)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_simplified)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_traditional)
        .DEF_BOOL_PROPERTY_RO(RimeStatus, is_ascii_punct);
}
