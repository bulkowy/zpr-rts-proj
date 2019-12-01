# sconstruct

# configurables

libs = []
lib_paths = []
cpp_flags = ['-Wall', '-Werror']
cxx_flags = ['-std=c++14']
DEBUG = True


# rest

def create_env(vars):
  env = Environment(variables = vars) # 7.2 tutorial
  Export('env') # 16.5.3 tutorial
  fill_env_flags(env)
  return env

def create_vars():
  vars = Variables() # 12.2.1 tutorial
  vars.Add('DEBUG', 'Set to 1 for DEBUG', DEBUG)

  return vars

def fill_env_flags(env):
  env.Append(CCFLAGS=cpp_flags) # 7.2.9-11 tutorial
  env.Append(CCFLAGS=cxx_flags)
  
  env.Append(CCFLAGS=['-g'] if env['DEBUG'] else ['-O2'])
  env.Append(CPPPATH=['#/src'])

# initial config
def initial():
  Decider('MD5-timestamp') # 6.1.3 tutorial

# build exec
def build():
  program = SConscript('src/SConscript', duplicate=0) # 16.1 tutorial
  env.Install('./', program) # 13.1 tutorial

initial()

vars = create_vars()
env = create_env(vars)

build()
