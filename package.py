# -*- coding: utf-8 -*-
import os, sys

unittestflags = (['@run_all', '--unittest-xml']
                 if os.environ.get('BROKEN_CUSTOM_ARGS_UNITTESTS') else [])

name = 'mcrt_messages'

if 'early' not in locals() or not callable(early):
    def early(): return lambda x: x

@early()
def version():
    """
    Increment the build in the version.
    """
    _version = '10.2'
    from rezbuild import earlybind
    return earlybind.version(this, _version)

description = 'Messages for mcrt computation'

authors = [
    'PSW Rendering and Shading',
    'moonbase-dev@dreamworks.com',
    'Toshi.Kato@dreamworks.com'
]

help = ('For assistance, '
        "please contact the folio's owner at: moonbase-dev@dreamworks.com")

if 'cmake' in sys.argv:
    build_system = 'cmake'
    build_system_pbr = 'cmake_modules'
else:
    build_system = 'scons'
    build_system_pbr = 'bart_scons-10'

variants = [
    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2020.3', 'icc-19.0.5.281.x.2'],
    ['os-CentOS-7', 'opt_level-debug', 'refplat-vfx2020.3', 'icc-19.0.5.281.x.2'],
    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2020.3', 'gcc-6.3.x.2'],
    ['os-CentOS-7', 'opt_level-debug', 'refplat-vfx2020.3', 'gcc-6.3.x.2'],

    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2021.0', 'gcc-9.3.x.1'],
    ['os-CentOS-7', 'opt_level-debug', 'refplat-vfx2021.0', 'gcc-9.3.x.1'],
    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2021.0', 'clang-13'],

    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2022.0', 'gcc-9.3.x.1'],
    ['os-CentOS-7', 'opt_level-debug', 'refplat-vfx2022.0', 'gcc-9.3.x.1'],

    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2019.3', 'gcc-6.3.x.2'],
    ['os-CentOS-7', 'opt_level-debug', 'refplat-vfx2019.3', 'gcc-6.3.x.2'],
]

conf_rats_variants = [
    ['os-CentOS-7', 'opt_level-optdebug', 'refplat-vfx2020.3', 'icc-19.0.5.281.x.2'],
    ['os-CentOS-7', 'opt_level-debug', 'refplat-vfx2020.3', 'icc-19.0.5.281.x.2'],
]

scons_targets = ['@install'] + unittestflags
sconsTargets = {
    'refplat-vfx2019.3': scons_targets,
    'refplat-vfx2020.3': scons_targets,
    'refplat-vfx2021.0': scons_targets,
    'refplat-vfx2022.0': scons_targets,
}

requires = [
    'arras4_core'
]

private_build_requires = [
    build_system_pbr,
    'python_lib-2.7'
]

if build_system is 'cmake':
    def commands():
        prependenv('CMAKE_MODULE_PATH', '{root}/lib64/cmake')
        prependenv('CMAKE_PREFIX_PATH', '{root}')
        prependenv('LD_LIBRARY_PATH', '{root}/lib64')
else:
    def commands():
        prependenv('LD_LIBRARY_PATH', '{root}/lib')

uuid = 'bae0bfc6-d58a-430b-b17d-c028a10412a5'

config_version = 0
