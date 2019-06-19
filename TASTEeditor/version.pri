GIT_APP=git
win32 {
    !contains($$PATH, "git") {
        GIT_APP=$$shell_quote("C:/Program Files/Git/bin/git.exe")
        warning("GIT_APP defaulted to: "$$GIT_APP)
    }
}

COMMIT_HASH = $$system($$shell_path($$GIT_APP) --git-dir $$shell_path($$_PRO_FILE_PWD_/../.git) describe --always)
DEFINES += VS_BUILD_HASH=\\\"$$COMMIT_HASH\\\"
