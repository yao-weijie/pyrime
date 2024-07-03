#include "rime_wrapper.hpp"

#include <rime_api.h>

#define K_PgUp 0xff55
#define K_PgDn 0xff56

// used for debug and visualization
// actually, these fmt functions can be over-written in python file
py::str fmt_traits(const RimeTraits& traits) {
    return py::str("shared_data_dir:        {}\n").format(traits.shared_data_dir) +
           py::str("user_data_dir:          {}\n").format(traits.user_data_dir) +
           py::str("distribution_name:      {}\n").format(traits.distribution_name) +
           py::str("distribution_code_name: {}\n").format(traits.distribution_code_name) +
           py::str("app_name:               {}\n").format(traits.app_name) +
           py::str("min_log_level:          {}\n").format(traits.min_log_level) +
           py::str("log_dir:                {}\n").format(traits.log_dir) +
           py::str("prebuilt_data_dir:      {}\n").format(traits.prebuilt_data_dir) +
           py::str("staging_dir:            {}\n").format(traits.staging_dir);
}

py::str fmt_candidate(const RimeCandidate& candidate) {
    return py::str("{}\n").format(candidate.text);
}

py::str fmt_composition(const RimeComposition& composition) {
    return py::str("composition: {}\n").format(composition.preedit);
}
py::str fmt_menu(const RimeMenu& menu) {
    auto s = py::str("page_no: {}, page_size: {}\n").format(menu.page_no, menu.page_size);
    for (int i = 0; i < menu.num_candidates; i++) {
        s += py::str("{} ").format(i) + fmt_candidate(menu.candidates[i]);
    }
    return s;
}

py::str fmt_context(const RimeContext& context) {
    return py::str("commit_preview: {}\n").format(context.commit_text_preview)  //
           + fmt_composition(context.composition)                               //
           + fmt_menu(context.menu);
}
py::str fmt_commit(const RimeCommit& commit) { return py::str("commit: {}\n").format(commit.text); }
py::str fmt_status(const RimeStatus& status) {
    return py::str("schema_id:      {}\n").format(status.schema_id) +
           py::str("schema_name:    {}\n").format(status.schema_name) +
           py::str("is_disabled:    {}\n").format(status.is_disabled) +
           py::str("is_composing:   {}\n").format(status.is_composing) +
           py::str("is_ascii_mode:  {}\n").format(status.is_ascii_mode) +
           py::str("is_full_shape:  {}\n").format(status.is_full_shape) +
           py::str("is_simplified:  {}\n").format(status.is_simplified) +
           py::str("is_traditional: {}\n").format(status.is_traditional) +
           py::str("is_ascii_punct: {}\n").format(status.is_ascii_punct);
}
