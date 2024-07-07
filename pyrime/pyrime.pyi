# pyrime stub file, used for lsp auto complection
# for more info, please refer help()

RimeSessionId = int
intBool = 0 | 1

class RimeTraits:
    """
    readable and writable rime struct,
    when the string properties are not set, the properties are None in python.
    """

    shared_data_dir: str
    user_data_dir: str
    distribution_name: str
    distribution_code_name: str
    distribution_version: str
    app_name: str
    min_log_level: int = 0 | 1 | 2 | 3
    log_dir: str
    prebuilt_data_dir: str
    staging_dir: str

class RimeComposition:
    """rime composition, read only struct"""

    length: int
    cursor_pos: int
    sel_start: int
    sel_end: int
    preedit: str | None

class RimeCandidate:
    """rime candidate, read only struct"""

    text: str | None
    comment: str | None

class RimeMenu:
    """rime menu, read only struct"""

    page_size: int
    page_no: int
    is_last_page: int = 0 | 1
    highlighted_candidate_index: int
    num_candidates: int
    candidates: list[RimeCandidate]
    select_keys: str | None

class RimeCommit:
    """rime commit, read only struct"""

    text: str | None

class RimeContext:
    """rime context, read only struct"""

    composition: RimeComposition
    menu: RimeMenu
    commit_text_preview: str
    # select_labels: list[str]

class RimeStatus:
    """rime status, read only struct"""

    schema_id: str | None
    schema_name: str | None
    is_disabled: intBool
    is_composing: intBool
    is_ascii_mode: intBool
    is_full_shape: intBool
    is_simplified: intBool
    is_traditional: intBool
    is_ascii_punct: intBool

class RimeSchemaListItem:
    schema_id: str | None
    name: str | None

def get_version() -> str: ...

# setup and finalize
def setup(traits: RimeTraits) -> None: ...
def initialize(traits: RimeTraits) -> None: ...
def finalize() -> None: ...

# maintenance
def start_maintenance(full_check: intBool) -> intBool: ...
def is_maintenancing() -> intBool: ...
def join_maintenance_thread() -> intBool: ...

## deployment
def deployer_initialize(traits: RimeTraits) -> None: ...
def prebuild() -> intBool: ...
def deploy() -> intBool: ...
def deploy_schema(schema_file: str) -> intBool: ...
def deploy_config_file(file_name: str, version_key: str) -> intBool: ...

# session
def create_session() -> RimeSessionId: ...
def find_session(session_id: RimeSessionId) -> intBool: ...
def destroy_session(session_id: RimeSessionId) -> intBool: ...
def cleanup_stale_sessions() -> None: ...
def cleanup_all_sessions() -> None: ...

# input
def get_input(session_id: RimeSessionId) -> str | None: ...
def set_input(session_id: RimeSessionId, input: str) -> intBool: ...
def simulate_key_sequence(session_id: RimeSessionId, key_sequence: str) -> intBool: ...
def process_key(session_id: RimeSessionId, keycode: int, mask: int) -> intBool: ...
def page_down(session_id: RimeSessionId) -> intBool: ...
def page_up(session_id: RimeSessionId) -> intBool: ...

## return True if there is unread commit text
def commit_composition(session_id: RimeSessionId) -> intBool: ...
def clear_composition(session_id: RimeSessionId) -> None: ...

# output
def get_context(session_id: RimeSessionId, context: RimeContext) -> intBool: ...
def free_context(context: RimeContext) -> intBool: ...
def get_context_(session_id: RimeSessionId) -> RimeContext: ...
def get_commit(session_id: RimeSessionId, commit: RimeCommit) -> intBool: ...
def free_commit(commit: RimeCommit) -> intBool: ...
def get_commit_(session_id: RimeSessionId) -> str | None: ...
def get_status(session_id: RimeSessionId, status: RimeStatus) -> intBool: ...
def free_status(status: RimeStatus) -> intBool: ...
def get_status_(session_id: RimeSessionId) -> RimeStatus: ...
def get_candidate_list(session_id: RimeSessionId) -> list[str]: ...

# runtime options
# TODO: property
def set_option(session_id: RimeSessionId, option: str, value: intBool) -> None: ...
def get_option(session_id: RimeSessionId, option: str) -> intBool: ...

# schema
def get_current_schema(
    session_id: RimeSessionId, buffer_size: int = 128
) -> str | None: ...
def select_schema(session_id: RimeSessionId, schema_id: str) -> intBool: ...
def get_schema_list() -> list[RimeSchemaListItem]: ...
