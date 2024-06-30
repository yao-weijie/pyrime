#include <pybind11/pybind11.h>
#include <rime_api.h>

#include <cstring>

namespace py = pybind11;

#define STRUCT_STR_PROPERTY(Type, prop)                    \
    .def_property(                                         \
        #prop, [](const Type& self) { return self.prop; }, \
        [](Type& self, const char* s) { self.prop = strdup(s); })

#define STRUCT_INIT(Type)                                     \
    .def(py::init([]() {                                      \
        Type var = {0};                                       \
        var.data_size = sizeof(Type) - sizeof(var.data_size); \
        return var;                                           \
    }))

PYBIND11_MODULE(pyrime, m) {
    m.doc() = "python bind of librime apis";

    // TODO: modules
    py::class_<RimeTraits>(m, "RimeTraits", py::dynamic_attr())          //
        STRUCT_INIT(RimeTraits)                                          //
        STRUCT_STR_PROPERTY(RimeTraits, shared_data_dir)                 //
        STRUCT_STR_PROPERTY(RimeTraits, user_data_dir)                   //
        STRUCT_STR_PROPERTY(RimeTraits, distribution_name)               //
        STRUCT_STR_PROPERTY(RimeTraits, distribution_code_name)          //
        STRUCT_STR_PROPERTY(RimeTraits, distribution_version)            //
        STRUCT_STR_PROPERTY(RimeTraits, app_name)                        //
            .def_readwrite("min_log_level", &RimeTraits::min_log_level)  //
        STRUCT_STR_PROPERTY(RimeTraits, log_dir)                         //
        STRUCT_STR_PROPERTY(RimeTraits, prebuilt_data_dir)               //
        STRUCT_STR_PROPERTY(RimeTraits, staging_dir)                     //
        ;
}
