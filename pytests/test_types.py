import lib.pyrime as pyrime


def test_traits():
    # test traits readable and writable
    traits = pyrime.RimeTraits()
    traits.shared_data_dir = "shared_data_dir"
    traits.user_data_dir = "user_data_dir"
    traits.distribution_name = "distribution_name"
    traits.distribution_code_name = "distribution_code_name"
    traits.app_name = "app_name"
    traits.min_log_level = 2
    traits.log_dir = "log_dir"
    traits.prebuilt_data_dir = "prebuilt_data_dir"
    traits.staging_dir = "staging_dir"
    assert traits.shared_data_dir == "shared_data_dir"
    assert traits.user_data_dir == "user_data_dir"
    assert traits.distribution_name == "distribution_name"
    assert traits.distribution_code_name == "distribution_code_name"
    assert traits.app_name == "app_name"
    assert traits.min_log_level == 2
    assert traits.log_dir == "log_dir"
    assert traits.prebuilt_data_dir == "prebuilt_data_dir"
    assert traits.staging_dir == "staging_dir"
    # print(traits)


def test_status():
    status = pyrime.RimeStatus()
    assert isinstance(str(status), str)
    # user created struct instance, the str type attr will be None
    status.schema_id
    # assert status.schema_id is None
    assert status.schema_name is None
    # user created instance, default is 0
    assert status.is_disabled == 0
    assert status.is_composing == 0
    assert status.is_ascii_mode == 0
    assert status.is_full_shape == 0
    assert status.is_simplified == 0
    assert status.is_traditional == 0
    assert status.is_ascii_punct == 0
