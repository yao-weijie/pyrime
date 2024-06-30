#include <pybind11/pybind11.h>
#include <rime_api.h>

#include <cstring>

namespace py = pybind11;

#define STRUCT_INIT(Type) def(py::init([]() { RIME_STRUCT(Type, var) return var; }))

#define DEF_STR_PROPERTY(Type, prop)                       \
    def_property(                                          \
        #prop, [](const Type& self) { return self.prop; }, \
        [](Type& self, const char* s) { self.prop = strdup(s); })

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
}
