import lib.pyrime as pyrime
from config import get_traits


def test(tmpdir):
    print(tmpdir)
    traits = get_traits(tmpdir)
    pyrime.setup(traits)
    pyrime.initialize(traits)

    def test_session():
        id = pyrime.create_session()
        assert pyrime.find_session(session_id=id)

        pyrime.destroy_session(id)
        assert not pyrime.find_session(id)

    def test_maintenance():
        for check in (0, 1):
            if pyrime.start_maintenance(full_check=check):
                pyrime.join_maintenance_thread()

    def test_deploy(traits):
        pyrime.deployer_initialize(traits)
        pyrime.prebuild()
        pyrime.deploy()

    def test_input():
        id = pyrime.create_session()
        pyrime.set_input(id, "haohaohao")

        cand_list = pyrime.get_candidate_list(id)
        assert isinstance(cand_list, list)
        for cand in cand_list:
            print(cand)
            pass

        context = pyrime.RimeContext()
        pyrime.get_context(id, context)
        assert isinstance(context.menu, pyrime.RimeMenu)
        assert isinstance(context.composition, pyrime.RimeComposition)
        assert context.menu.page_no == 0
        assert (
            len(context.menu.candidates)
            == context.menu.num_candidates
            == context.menu.page_size
            == 5
        )
        candidates = context.menu.candidates
        for i in range(context.menu.num_candidates):
            assert isinstance(candidates[i], pyrime.RimeCandidate)

        print(context)

        pyrime.free_context(context)  # seems gc by python?
        pyrime.get_context(id, context)

        status = pyrime.RimeStatus()
        pyrime.get_status(id, status)
        assert status.is_composing == 1
        print(status)

        pyrime.commit_composition(id)
        commit = pyrime.RimeCommit()
        pyrime.get_commit(id, commit)
        print(commit)

        # after commit
        pyrime.get_context(id, context)
        assert context.commit_text_preview is None
        assert context.composition.length == 0
        assert context.menu.num_candidates == 0
        print(context)

        pyrime.get_status(id, status)
        assert status.is_composing == 0
        # print(status)

        pyrime.destroy_session(id)

    def test_output_():
        id = pyrime.create_session()
        pyrime.set_input(id, "haohaohao")
        context = pyrime._get_context(id)
        assert isinstance(context, pyrime.RimeContext)
        print(context)

        status = pyrime._get_status(id)
        assert isinstance(status, pyrime.RimeStatus)
        assert status.is_composing == 1

        pyrime.commit_composition(id)
        commit = pyrime._get_commit(id)
        assert isinstance(commit, str)

        status = pyrime._get_status(id)
        assert isinstance(status, pyrime.RimeStatus)
        assert status.is_composing == 0
        print(status)

    def test_page():
        id = pyrime.create_session()
        pyrime.set_input(id, "haohaohao")
        context0 = pyrime.RimeContext()
        pyrime.get_context(id, context0)
        print(context0)

        pyrime.page_down(id)
        context1 = pyrime.RimeContext()
        pyrime.get_context(id, context1)
        assert context1.menu.page_no == 1
        print(context1)

        pyrime.page_up(id)
        pyrime.get_context(id, context1)
        assert context1.menu.page_no == 0
        # print(context1)

        context = pyrime.RimeContext()
        while pyrime.page_down(id):  # always 1 enen if is_last_page
            pyrime.get_context(id, context)
            if context.menu.is_last_page == 1:
                break

        assert context.menu.is_last_page == 1
        print("last page no", context.menu.page_no)

        pyrime.destroy_session(id)

    def test_schema_list():
        id = pyrime.create_session()
        current_schema_id = pyrime.get_current_schema(id)
        print(current_schema_id)

        schema_list = pyrime.get_schema_list()
        schema_id_list = [schema.schema_id for schema in schema_list]
        assert current_schema_id in schema_id_list
        for i, schema in enumerate(schema_list):
            print(schema, end="")
            assert pyrime.select_schema(id, schema.schema_id)
            curr_id = pyrime.get_current_schema(id)
            assert curr_id == schema.schema_id

        pyrime.select_schema(id, current_schema_id)

        pyrime.destroy_session(id)

    test_session()
    test_maintenance()
    test_deploy(traits)
    test_input()
    test_output_()
    test_page()
    test_schema_list()

    pyrime.finalize()
