from pathlib import Path
import pyrime


def get_traits(tmpdir):
    tmpdir = Path(tmpdir)
    user_data_dir = tmpdir / "pyrime"
    user_data_dir.mkdir(exist_ok=True)
    log_dir = tmpdir / "log"
    log_dir.mkdir(exist_ok=True)
    staging_dir = user_data_dir / "build"
    staging_dir.mkdir(exist_ok=True)

    traits = pyrime.RimeTraits()
    traits.shared_data_dir = "/usr/share/rime-data"
    traits.user_data_dir = str(user_data_dir)
    traits.distribution_name = "Rime"
    traits.distribution_code_name = "pyrime"
    traits.distribution_version = "0.1"
    traits.app_name = "pyrime"
    traits.min_log_level = 0
    traits.log_dir = str(log_dir)
    traits.prebuilt_data_dir = "/usr/share/rime-data/build"
    traits.staging_dir = str(staging_dir)

    return traits
