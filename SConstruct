# sconstruct
import os

# configurables

libs = ['sfml-graphics','sfml-window', 'sfml-system', 'sfml-audio', 'jsoncpp']
lib_paths = ['/usr/lib/x86_64-linux-gnu/']
include_paths = ['#/include', '/usr/include']
cpp_flags = ['-Wall', '-Werror']
cxx_flags = ['-std=c++14']
DEBUG = True

target_path = os.getcwd() + '/bin/'
Export('target_path')

# rest

def create_env(vars):
  env = Environment(variables = vars) # 7.2 tutorial
  fill_env_flags(env)
  add_special_methods(env)
  return env

def create_vars():
  vars = Variables() # 12.2.1 tutorial
  vars.Add('DEBUG', 'Set to 1 for DEBUG', DEBUG)

  return vars

def fill_env_flags(env):
  env.Append(CCFLAGS=cpp_flags) # 7.2.9-11 tutorial
  env.Append(CCFLAGS=cxx_flags)
  
  env.Append(CCFLAGS=['-g'] if env['DEBUG'] else ['-O2'])
  env.Append(CPPPATH=include_paths)

  env.Append(LIBS=libs)
  env.Append(LIBPATH=lib_paths)

def add_special_methods(env):
    boost_libs = [] if env['CC'] == 'cl' else ['boost_unit_test_framework']
    boost_defines = [] if env['CC'] == 'cl' else ['BOOST_TEST_DYN_LINK']

    def run_unit_test(env, target, source):
        import subprocess
        app = str(source[0].abspath)
        if not subprocess.call(app):
            open(str(target[0]),'w').write("PASSED\n")
        else:
            return 1

    def build_and_run_test(env, src, lib, depLibs = []):
        lib_name = str(lib[0]).replace('.', '_')
        test = env.Program(lib_name + "_unit_tests",
                           src,
                           LIBS=[lib] + depLibs + boost_libs,
                           CPPDEFINES=boost_defines)
        env.Command(lib_name + "_test_passed.txt", test, run_unit_test)

    env.AddMethod(build_and_run_test, "BoostTests")

# initial config
def initial():
  Decider('MD5-timestamp') # 6.1.3 tutorial

# build exec
def build():
  SConscript('src/SConscript', duplicate=0) # 16.1 tutorial

initial()

vars = create_vars()
env = create_env(vars)
Export('env') # 16.5.3 tutorial

build()
