from env_settings import DEFINES_ENV, WIN32_VCVARSALL

class LibExternal:
	def __init__(self, name, path, compiledNames = None, dllNames = None):
		self.name = name
		self.path = path
		self.compiledNames = compiledNames
		self.dllNames = dllNames

PROJECT_NAME = "kapricornmedia"

COPY_DIRS = {}

DEFINES = [
	"KM_CPP_STD",
	"KM_KMKV_JSON"
]
DEFINES.extend(DEFINES_ENV)

DEPLOY_FILES = []

LIBS_EXTERNAL = [
	LibExternal("cJSON",       "cJSON"),
	LibExternal("cpp-httplib", "cpp-httplib"),
	LibExternal("stb_sprintf", "stb_sprintf-1.06"),
	LibExternal("xxHash",      "xxHash")
]

PATHS = {
	"win32-vcvarsall": WIN32_VCVARSALL
}
