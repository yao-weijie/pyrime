#include <pybind11/pybind11.h>
#include <rime_api.h>

#include <cstring>

#include "rime_wrapper.hpp"

namespace py = pybind11;

#define STRUCT_INIT(Type) def(py::init([]() { RIME_STRUCT(Type, var) return var; }))

#define DEF_STR_PROPERTY(Type, prop)                       \
    def_property(                                          \
        #prop, [](const Type& self) { return self.prop; }, \
        [](Type& self, const char* s) { self.prop = strdup(s); })

// optional, explicitly convert to python bool True/False
#define DEF_BOOL_PROPERTY_RO(Type, prop) \
    def_property_readonly(#prop, [](const Type& self) { return py::bool_(self.prop); })

PYBIND11_MODULE(pyrime, m) {
    m.doc() = "python bind of librime apis";

    // setup and finalize
    m.def("RimeSetup", &RimeSetup, py::arg("traits"));
    m.def("RimeInitialize", &RimeInitialize, py::arg("traits"));
    m.def("RimeFinalize", &RimeFinalize);
    // maintenance
    m.def("RimeStartMaintenance", &RimeStartMaintenance, py::arg("full_check"));
    m.def("RimeIsMaintenancing", &RimeIsMaintenancing);
    m.def("RimeJoinMaintenanceThread", &RimeJoinMaintenanceThread);
    // deployment
    m.def("RimeDeployerInitialize", &RimeDeployerInitialize, py::arg("traits"));
    m.def("RimePrebuildAllSchemas", &RimePrebuildAllSchemas);
    m.def("RimeDeployWorkspace", &RimeDeployWorkspace);
    m.def("RimeDeploySchema", &RimeDeploySchema, py::arg("schema_file"));
    m.def("RimeDeployConfigFile", &RimeDeployConfigFile, py::arg("file_name"),
          py::arg("version_key"));
    m.def("RimeSyncUserData", &RimeSyncUserData);
    // session management
    m.def("RimeCreateSession", &RimeCreateSession);
    m.def("RimeFindSession", &RimeFindSession, py::arg("session_id"));
    m.def("RimeDestroySession", &RimeDestroySession, py::arg("session_id"));
    m.def("RimeCleanupStaleSessions", &RimeCleanupStaleSessions);
    m.def("RimeCleanupAllSessions", &RimeCleanupAllSessions);
    // input
    m.def("RimeProcessKey", &RimeProcessKey, py::arg("session_id"), py::arg("keycode"),
          py::arg("mask"));
    m.def("RimeCommitComposition", &RimeCommitComposition, py::arg("session_id"));
    m.def("RimeClearComposition", &RimeClearComposition, py::arg("session_id"));
    // input test
    m.def("RimeSimulateKeySequence", &RimeSimulateKeySequence, py::arg("session_id"),
          py::arg("key_sequence"));
    m.def("RimeSetInput", &RimeSetInput, py::arg("session_id"), py::arg("input"));
    // output
    m.def("RimeGetContext", &RimeGetContext, py::arg("session_id"), py::arg("context"));
    m.def("RimeFreeStatus", &RimeFreeStatus, py::arg("context"));
    m.def("RimeGetCommit", &RimeGetCommit, py::arg("session_id"), py::arg("commit"));
    m.def("RimeFreeCommit", &RimeFreeCommit, py::arg("commit"));
    m.def("RimeGetStatus", &RimeGetStatus, py::arg("session_id"), py::arg("status"));
    m.def("RimeFreeStatus", &RimeFreeStatus, py::arg("status"));

    // runtime options
    m.def("RimeSetOption", &RimeSetOption, py::arg("session_id"), py::arg("option"),
          py::arg("value"));
    m.def("RimeGetOption", &RimeGetOption, py::arg("session_id"), py::arg("option"));
    m.def("RimeSetProperty", &RimeSetProperty, py::arg("session_id"), py::arg("prop"),
          py::arg("value"));
    m.def("RimeGetProperty", &RimeGetProperty, py::arg("session_id"), py::arg("prop"),
          py::arg("value"), py::arg("buffer_size"));

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
        .DEF_STR_PROPERTY(RimeTraits, staging_dir)
        .def("__str__", &fmt_traits);

    py::class_<RimeComposition>(m, "RimeComposition", py::dynamic_attr())
        .def(py::init<>())
        .def_readonly("length", &RimeComposition::length)
        .def_readonly("cursor_pos", &RimeComposition::cursor_pos)
        .def_readonly("sel_start", &RimeComposition::sel_start)
        .def_readonly("sel_end", &RimeComposition::sel_end)
        .def_readonly("preedit", &RimeComposition::preedit)
        .def("__str__", &fmt_composition);

    py::class_<RimeCandidate>(m, "RimeCandidate", py::dynamic_attr())
        .def(py::init<>())
        .def_readonly("text", &RimeCandidate::text)
        .def_readonly("comment", &RimeCandidate::comment)
        .def("__str__", &fmt_candidate);

    py::class_<RimeMenu>(m, "RimeMenu", py::dynamic_attr())
        .def(py::init<>())
        .def_readonly("page_size", &RimeMenu::page_size)
        .def_readonly("page_no", &RimeMenu::page_no)
        .def_readonly("is_last_page", &RimeMenu::is_last_page)
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
        .def("__str__", &fmt_menu)
        .def_readonly("select_keys", &RimeMenu::select_keys);

    py::class_<RimeCommit>(m, "RimeCommit", py::dynamic_attr())
        .STRUCT_INIT(RimeCommit)
        .def_readonly("text", &RimeCommit::text)
        .def("__str__", [](const RimeCommit& self) { return py::str("{}\n").format(self.text); });

    py::class_<RimeContext>(m, "RimeContext", py::dynamic_attr())
        .STRUCT_INIT(RimeContext)
        .def_readonly("composition", &RimeContext::composition)
        .def_readonly("menu", &RimeContext::menu)
        .def_readonly("commit_text_preview", &RimeContext::commit_text_preview)
        // TODO: select_labels
        .def("__str__", &fmt_context);

    py::class_<RimeStatus>(m, "RimeStatus", py::dynamic_attr())
        .STRUCT_INIT(RimeStatus)
        .def_readonly("schema_id", &RimeStatus::schema_id)
        .def_readonly("schema_name", &RimeStatus::schema_name)
        .def_readonly("is_disabled", &RimeStatus::is_disabled)
        .def_readonly("is_composing", &RimeStatus::is_composing)
        .def_readonly("is_ascii_mode", &RimeStatus::is_ascii_mode)
        .def_readonly("is_full_shape", &RimeStatus::is_full_shape)
        .def_readonly("is_simplified", &RimeStatus::is_simplified)
        .def_readonly("is_traditional", &RimeStatus::is_traditional)
        .def_readonly("is_ascii_punct", &RimeStatus::is_ascii_punct)
        .def("__str__", &fmt_status);
}
