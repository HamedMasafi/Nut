TEMPLATE = subdirs

SUBDIRS += \
    tst_basic \
    tst_benckmark  \
    tst_datatypes \
    tst_phrases \
    tst_properties \
    tst_quuid \
    tst_generators \
    tst_upgrades \
    tst_json \
    tst_datetime

cmake.CONFIG += no_run-tests_target
prepareRecursiveTarget(run-tests)
QMAKE_EXTRA_TARGETS += run-tests
