#include <X11/keysym.h>
#include <X11/keysymdef.h>
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

static RimeApi* api = rime_get_api();

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
        .def("__str__", &fmt_commit);

    py::class_<RimeContext>(m, "RimeContext", py::dynamic_attr())
        .STRUCT_INIT(RimeContext)
        .def_readonly("composition", &RimeContext::composition)
        .def_readonly("menu", &RimeContext::menu)
        .def_readonly("commit_text_preview", &RimeContext::commit_text_preview)
        // TODO: select_labels
        .def("__del__", [](RimeContext& self) { api->free_context(&self); })
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
        .def("__del__", [](RimeStatus& self) { api->free_status(&self); })
        .def("__str__", &fmt_status);

    py::class_<RimeSchemaListItem>(m, "RimeSchemaListItem")
        .def(py::init<>())
        .def_readonly("schema_id", &RimeSchemaListItem::schema_id)
        .def_readonly("name", &RimeSchemaListItem::name)
        .def("__del__", [](RimeSchemaList& self) { api->free_schema_list(&self); })
        .def("__str__", [](const RimeSchemaListItem& self) {
            return py::str("{} {}\n").format(self.schema_id, self.name);
        });

    // setup and finalize
    m.def("setup", api->setup, py::arg("traits"));
    m.def("initialize", api->initialize, py::arg("traits"));
    m.def("finalize", api->finalize);
    // maintenance
    m.def("start_maintenance", api->start_maintenance, py::arg("full_check") = 0);
    m.def("is_maintenance_mode", api->is_maintenance_mode);
    m.def("join_maintenance_thread", api->join_maintenance_thread);

    // deployment
    m.def("deployer_initialize", api->deployer_initialize, py::arg("traits"));
    m.def("prebuild", api->prebuild);
    m.def("deploy", api->deploy);
    m.def("deploy_schema", api->deploy_schema, py::arg("schema_file"));
    m.def("deploy_config_file", api->deploy_config_file, py::arg("file_name"),
          py::arg("version_key"));

    // session management
    m.def("create_session", api->create_session);
    m.def("find_session", api->find_session, py::arg("session_id"));
    m.def("destroy_session", api->destroy_session, py::arg("session_id"));
    m.def("cleanup_stale_sessions", api->cleanup_stale_sessions);
    m.def("cleanup_all_sessions", api->cleanup_all_sessions);

    // input
    m.def("set_input", api->set_input, py::arg("session_id"), py::arg("input"));
    m.def("simulate_key_sequence", api->simulate_key_sequence, py::arg("session_id"),
          py::arg("key_sequence"));
    m.def("process_key", api->process_key, py::arg("session_id"), py::arg("keycode"),
          py::arg("mask") = 0);
    m.def("commit_composition", api->commit_composition, py::arg("session_id"));
    m.def("clear_composition", api->clear_composition, py::arg("session_id"));
    m.def("page_down",
          [](const RimeSessionId session_id) { return api->process_key(session_id, XK_Next, 0); });
    m.def("page_up",
          [](const RimeSessionId session_id) { api->process_key(session_id, XK_Prior, 0); });

    // output
    m.def("get_context", api->get_context, py::arg("session_id"), py::arg("context"));
    m.def("free_context", api->free_context, py::arg("context"));
    m.def("_get_context", [](RimeSessionId session_id) -> RimeContext {
        RIME_STRUCT(RimeContext, context);
        api->get_context(session_id, &context);
        return context;
    });
    m.def("get_commit", api->get_commit, py::arg("session_id"), py::arg("commit"));
    m.def("free_commit", api->free_commit, py::arg("commit"));
    m.def("_get_commit", [](RimeSessionId session_id) -> char* {
        RIME_STRUCT(RimeCommit, commit);
        char* text;
        if (api->get_commit(session_id, &commit)) {
            text = strdup(commit.text);
            api->free_commit(&commit);
            return text;
        } else {
            return NULL;
        }
    });
    m.def("get_status", api->get_status, py::arg("session_id"), py::arg("status"));
    m.def("free_status", api->free_status, py::arg("status"));
    m.def("_get_status", [](RimeSessionId session_id) -> RimeStatus {
        RIME_STRUCT(RimeStatus, status);
        api->get_status(session_id, &status);
        return status;
    });
    // Accessing candidate list
    m.def("get_candidate_list", [](RimeSessionId session_id) -> py::list {
        py::list cand_list;
        RimeCandidateListIterator it;
        api->candidate_list_begin(session_id, &it);

        while (1) {
            if (api->candidate_list_next(&it)) {
                cand_list.append(strdup(it.candidate.text));
            } else {
                break;
            }
        }
        return cand_list;
    });

    // runtime options
    m.def("set_option", api->set_option, py::arg("session_id"), py::arg("option"),
          py::arg("value"));
    m.def("get_option", api->get_option, py::arg("session_id"), py::arg("option"));
    // TODO: property test
    m.def("set_property", api->set_property, py::arg("session_id"), py::arg("prop"),
          py::arg("value"));
    m.def(
        "get_property",
        [](RimeSessionId session_id, const char* prop, size_t buffer_size = 128) -> char* {
            char* value = new char[buffer_size];
            if (api->get_property(session_id, prop, value, buffer_size)) {
                return value;
            } else {
                delete[] value;
                return NULL;
            }
        },
        py::arg("session_id"), py::arg("prop"), py::arg("buffer_size") = 128);
    m.def(
        "get_current_schema",
        [](RimeSessionId session_id, size_t buffer_size = 128) -> char* {
            char* schema_id = new char[buffer_size];
            if (api->get_current_schema(session_id, schema_id, buffer_size)) {
                return schema_id;
            } else {
                delete[] schema_id;
                return NULL;
            }
        },
        py::arg("session_id"), py::arg("buffer_size") = 128);
    m.def("select_schema", api->select_schema, py::arg("session_id"), py::arg("schema_id"));
    m.def("get_schema_list", []() -> py::list {
        py::list py_scheme_list;
        RimeSchemaList schema_list;
        if (api->get_schema_list(&schema_list)) {
            for (int i = 0; i < schema_list.size; i++) {
                py_scheme_list.append(schema_list.list[i]);
            }
        }
        // api->free_schema_list(&schema_list); // ownership taken by python?
        return py_scheme_list;
    });
}
