TEMPLATE = subdirs

CONFIG += no_docs_target

SUBDIRS += auto

OTHER_FILES += ../.github/workflows/build.yml

prepareRecursiveTarget(run-tests)
QMAKE_EXTRA_TARGETS += run-tests
