Import('env')

AddOption('--arrascodecov',
          dest='arrascodecov',
          type='string',
          action='store',
          help='Build with codecov instrumentation')

if GetOption('arrascodecov') != None:
    env['CXXFLAGS'].append('-prof_gen:srcpos,threadsafe')
    env['CXXFLAGS'].append('-prof_dir%s' % GetOption('arrascodecov'))
    env.CacheDir(None)

env.Tool('component')
env.Tool('dwa_install')
env.Tool('dwa_run_test')
env.Tool('dwa_utils')
env['DOXYGEN_LOC'] = '/rel/third_party/doxygen/1.8.11/bin/doxygen'
env.Tool('dwa_doxygen')
env['DOXYGEN_CONFIG']['DOT_GRAPH_MAX_NODES'] = '200'
env['CPPCHECK_LOC'] = '/rel/third_party/cppcheck/1.71/cppcheck'
env.Tool('cppcheck')
env.Tool('python_sdk')

env.Tool('vtune')

# For SBB integration - SBB uses the keyword "restrict", so the compiler needs to enable it. 
if "icc" in env['COMPILER_LABEL']:
    env['CXXFLAGS'].append('-restrict')
env.Replace(USE_OPENMP = [])


from dwa_sdk import DWALoadSDKs
DWALoadSDKs(env)
#Set optimization level for debug -O0
#icc defaults to -O2 for debug and -O3 for opt/opt-debug
if env['TYPE_LABEL'] == 'debug':
    env.AppendUnique(CCFLAGS = ['-O0'])
    if 'icc' in env['COMPILER_LABEL'] and '-inline-level=0' not in env['CXXFLAGS']:
        env['CXXFLAGS'].append('-inline-level=0')

# don't error on writes to static vars
if 'icc' in env['COMPILER_LABEL'] and '-we1711' in env['CXXFLAGS']:
    env['CXXFLAGS'].remove('-we1711')

# For Arras, we've made the decision to part with the studio standards and use #pragma once
# instead of include guards. We disable warning 1782 to avoid the related (incorrect) compiler spew.
if 'icc' in env['COMPILER_LABEL'] and '-wd1782' not in env['CXXFLAGS']:
    env['CXXFLAGS'].append('-wd1782')       # #pragma once is obsolete. Use #ifndef guard instead.  

env.DWASConscriptWalk(topdir='#mcrt_messages', ignore=[])
env.DWASConscriptWalk(topdir='#doc', ignore=[])

env.DWAResolveUndefinedComponents([])
env.DWAFillInMissingInitPy()
env.DWAFreezeComponents()

# Set default target
env.Default(env.Alias('@install'))


