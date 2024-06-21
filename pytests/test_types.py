import lib.rime_api as rime_api


def test_traits():
    traits = rime_api.RimeTraits()
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
