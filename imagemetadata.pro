TEMPLATE=subdirs
SUBDIRS=lib plugin tests
plugin.depends = lib

OTHER_FILES += rpm/imagemetadata.spec
