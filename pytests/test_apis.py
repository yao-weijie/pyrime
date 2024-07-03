import lib.pyrime as pyrime
from config import get_traits


def test(tmpdir):
    print(tmpdir)
    traits = get_traits(tmpdir)
    pyrime.RimeSetup(traits)
    pyrime.RimeInitialize(traits)

    def test_session():
        id = pyrime.RimeCreateSession()
        assert pyrime.RimeFindSession(session_id=id)

        pyrime.RimeDestroySession(id)
        assert not pyrime.RimeFindSession(id)

    def test_maintenance():
        for check in (0, 1):
            if pyrime.RimeStartMaintenance(full_check=check):
                pyrime.RimeJoinMaintenanceThread()

    def test_deploy(traits):
        pyrime.RimeDeployerInitialize(traits)
        pyrime.RimePrebuildAllSchemas()
        pyrime.RimeDeployWorkspace()

    def test_input():
        id = pyrime.RimeCreateSession()
        pyrime.RimeSetInput(id, "haohaohao")

        context = pyrime.RimeContext()
        pyrime.RimeGetContext(id, context)
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

        pyrime.RimeFreeContext(context)  # seems gc by python?
        pyrime.RimeGetContext(id, context)

        status = pyrime.RimeStatus()
        pyrime.RimeGetStatus(id, status)
        assert status.is_composing == 1
        print(status)

        pyrime.RimeCommitComposition(id)
        commit = pyrime.RimeCommit()
        pyrime.RimeGetCommit(id, commit)
        print(commit)

        # after commit
        pyrime.RimeGetContext(id, context)
        assert context.commit_text_preview is None
        assert context.composition.length == 0
        assert context.menu.num_candidates == 0
        print(context)

        pyrime.RimeGetStatus(id, status)
        assert status.is_composing == 0
        # print(status)

        pyrime.RimeDestroySession(id)

    def test_page():
        id = pyrime.RimeCreateSession()
        pyrime.RimeSetInput(id, "haohaohao")
        context0 = pyrime.RimeContext()
        pyrime.RimeGetContext(id, context0)
        print(context0)

        pyrime.RimePageDown(id)
        context1 = pyrime.RimeContext()
        pyrime.RimeGetContext(id, context1)
        assert context1.menu.page_no == 1
        print(context1)

        pyrime.RimePageUp(id)
        pyrime.RimeGetContext(id, context1)
        assert context1.menu.page_no == 0
        # print(context1)

        context = pyrime.RimeContext()
        while pyrime.RimePageDown(id):  # always 1 enen if is_last_page
            pyrime.RimeGetContext(id, context)
            if context.menu.is_last_page == 1:
                break

        assert context.menu.is_last_page == 1
        print("last page no", context.menu.page_no)

        pyrime.RimeDestroySession(id)

    test_session()
    test_maintenance()
    test_deploy(traits)
    test_input()
    test_page()

    pyrime.RimeFinalize()
