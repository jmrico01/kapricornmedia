#if SERVER_HTTPS
#define CPPHTTPLIB_OPENSSL_SUPPORT
#define CPPHTTPLIB_ZLIB_SUPPORT
#endif
#include <httplib.h>

#include <km_common/km_defines.h>
#include <km_common/km_kmkv.h>
#include <km_common/km_lib.h>
#include <km_common/km_os.h>
#include <km_common/km_string.h>

#include "settings.h"

global_var const int HTTP_STATUS_ERROR = 500;

#if SERVER_HTTPS
typedef httplib::SSLServer ServerType;
#else
typedef httplib::Server ServerType;
#endif

int main(int argc, char** argv)
{
#if SERVER_HTTPS
	ServerType server(SERVER_CERT, SERVER_KEY);
#else
	ServerType server;
#endif

	FixedArray<char, PATH_MAX_LENGTH> rootPath = GetExecutablePath(&defaultAllocator_);
	if (rootPath.size == 0) {
		fprintf(stderr, "Failed to get executable path\n");
		return 1;
	}
	rootPath.RemoveLast();
	rootPath.size = rootPath.ToArray().FindLast('/') + 1;
	printf("Root path: %.*s\n", (int)rootPath.size, rootPath.data);

	server.Get("/posts", [](const auto& req, auto& res) {
		// do stuff
		printf("hello\n");
	});

	FixedArray<char, PATH_MAX_LENGTH> imageRootPath = rootPath;
	imageRootPath.RemoveLast();
	uint64 lastSlash = imageRootPath.ToArray().FindLast('/');
	if (lastSlash == imageRootPath.size) {
		fprintf(stderr, "Bad public path, no directory above for images: %.*s\n",
			(int)imageRootPath.size, imageRootPath.data);
		return 1;
	}
	imageRootPath.size = lastSlash + 1;
	imageRootPath.Append(ToString("kapricornmedia-images"));
	imageRootPath.Append('\0');
	if (!server.set_base_dir(imageRootPath.data, "/images")) {
		fprintf(stderr, "server set_base_dir failed on dir %s\n", imageRootPath.data);
		return 1;
	}

	FixedArray<char, PATH_MAX_LENGTH> publicPath = rootPath;
	publicPath.Append(ToString("data/public"));
	publicPath.Append('\0');
	if (!server.set_base_dir(publicPath.data, "/")) {
		fprintf(stderr, "server set_base_dir failed on dir %s\n", publicPath.data);
		return 1;
	}

	const char* host = "localhost";
	printf("Listening on host \"%s\", port %d\n", host, SERVER_PORT);
	if (!server.listen(host, SERVER_PORT)) {
		fprintf(stderr, "server listen failed for host \"%s\", port %d\n", host, SERVER_PORT);
		return 1;
	}

	return 0;
}

#include <cJSON.c>
#define STB_SPRINTF_IMPLEMENTATION
#include <stb_sprintf.h>
#undef STB_SPRINTF_IMPLEMENTATION
#include <xxhash.c>

#undef GAME_SLOW
#undef GAME_INTERNAL
#include <km_common/km_debug.h>
#undef DEBUG_ASSERTF
#undef DEBUG_ASSERT
#undef DEBUG_PANIC
#define DEBUG_ASSERTF(expression, format, ...) if (!(expression)) { \
	fprintf(stderr, "Assert failed:\n"); \
	fprintf(stderr, format, ##__VA_ARGS__); \
	abort(); }
#define DEBUG_ASSERT(expression) DEBUG_ASSERTF(expression, "nothing")
#define DEBUG_PANIC(format, ...) \
	fprintf(stderr, "PANIC!\n"); \
	fprintf(stderr, format, ##__VA_ARGS__); \
	abort();

#define LOG_ERROR(format, ...) fprintf(stderr, format, ##__VA_ARGS__)

#include <km_common/km_kmkv.cpp>
#include <km_common/km_lib.cpp>
#include <km_common/km_memory.cpp>
#include <km_common/km_os.cpp>
#include <km_common/km_string.cpp>

